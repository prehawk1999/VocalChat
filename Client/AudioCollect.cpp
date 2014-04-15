#include "stdafx.h"
#include "AudioCollect.h"
#include "ChatClient1Dlg.h"

#define TEMP_BUFFER				256
#define RECORD_BUFFER			400

void CALLBACK waveInProc( HWAVEIN hwi,  UINT uMsg,  DWORD_PTR dwInstance,  DWORD_PTR wParam,  DWORD_PTR lParam);
void CALLBACK waveOutProc( HWAVEOUT hwi,  UINT uMsg,  DWORD_PTR dwInstance,  DWORD_PTR wParam,  DWORD_PTR lParam);
void CALLBACK _RecordProc( HWAVEIN hwi,  UINT uMsg,  DWORD_PTR dwInstance,  DWORD_PTR wParam,  DWORD_PTR lParam);//new
void CALLBACK _PlayProc( HWAVEOUT hwo,  UINT uMsg,  DWORD_PTR dwInstance,  DWORD_PTR wParam,  DWORD_PTR lParam);//new
DWORD WINAPI _RecordThread(LPVOID lpParam);
DWORD WINAPI _PlayThread(LPVOID lpParam);
DWORD WINAPI _SendClips(LPVOID lpParam);
DWORD WINAPI _RecvClips(LPVOID lpParam);

CAudioCollect::CAudioCollect(void)
{
	m_pRecordInfo	= new WavInfo;
	m_pPlayInfo		= new WavInfo;

	// Allocate memory for wave header
	m_pRecordInfo->pWaveHdr1 = reinterpret_cast<PWAVEHDR>(malloc (sizeof (WAVEHDR))) ;
	m_pRecordInfo->pWaveHdr2 = reinterpret_cast<PWAVEHDR>(malloc (sizeof (WAVEHDR)));
	m_pPlayInfo->pWaveHdr1 = reinterpret_cast<PWAVEHDR>(malloc (sizeof (WAVEHDR))) ;
	m_pPlayInfo->pWaveHdr2 = reinterpret_cast<PWAVEHDR>(malloc (sizeof (WAVEHDR))) ;

	//allocate buffer memory
	m_pRecordInfo->pBuffer1 = (PBYTE)malloc(TEMP_BUFFER);
	m_pRecordInfo->pBuffer2 = (PBYTE)malloc(TEMP_BUFFER);
	m_pPlayInfo->pBuffer1 = (PBYTE)malloc(TEMP_BUFFER);
	m_pPlayInfo->pBuffer2 = (PBYTE)malloc(TEMP_BUFFER);

	//bind the wavehdr with a buffer
	m_pRecordInfo->pWaveHdr1->dwBufferLength			= TEMP_BUFFER;
	m_pRecordInfo->pWaveHdr1->lpData						= (LPTSTR)m_pRecordInfo->pBuffer1;
	m_pRecordInfo->pWaveHdr1->dwBytesRecorded		= 0;
	m_pRecordInfo->pWaveHdr1->dwFlags						= 0;
	m_pRecordInfo->pWaveHdr1->dwLoops						= 1;
	m_pRecordInfo->pWaveHdr1->dwUser						= 0;
	m_pRecordInfo->pWaveHdr1->reserved						= 0;
	m_pRecordInfo->pWaveHdr1->lpNext						= NULL;

	m_pRecordInfo->pWaveHdr2->dwBufferLength			= TEMP_BUFFER;
	m_pRecordInfo->pWaveHdr2->lpData						= (LPTSTR)m_pRecordInfo->pBuffer2;
	m_pRecordInfo->pWaveHdr2->dwBytesRecorded		= 0;
	m_pRecordInfo->pWaveHdr2->dwFlags						= 0;
	m_pRecordInfo->pWaveHdr2->dwLoops						= 1;
	m_pRecordInfo->pWaveHdr2->dwUser						= 0;
	m_pRecordInfo->pWaveHdr2->reserved					= 0;
	m_pRecordInfo->pWaveHdr2->lpNext						= NULL;

	// Allocate memory for save buffer
	m_pRecordInfo-> pSaveBuffer = (PBYTE)malloc (1) ;	
	m_pPlayInfo-> pSaveBuffer = (PBYTE)malloc (1) ;	
	
	m_pRecordInfo->dwDataLength = 0;
	m_pPlayInfo->dwDataLength	= 0;

	//���������ռ��Ͳ��Ÿ�ʽ
	m_wavefmt.wFormatTag			= WAVE_FORMAT_PCM;
	m_wavefmt.nChannels				=1;
	m_wavefmt.nSamplesPerSec		=11025;
	m_wavefmt.nAvgBytesPerSec		=11025;
	m_wavefmt.nBlockAlign			=1;
	m_wavefmt.wBitsPerSample		=8;
	m_wavefmt.cbSize				=0;

	//�����������
	m_hWaveIn	= NULL;
	m_hWaveOut	= NULL;
}

CAudioCollect::~CAudioCollect(void)
{
	delete m_pPlayInfo;
	delete m_pRecordInfo;
}

void CAudioCollect::SetMainDlg(CChatClient1Dlg* pDlg)
{
	m_pDlg = pDlg;
}



void CALLBACK _RecordProc( HWAVEIN hwi,  UINT uMsg,  DWORD_PTR dwInstance,  DWORD_PTR wParam,  DWORD_PTR lParam)
{
	MMRESULT			mmr;
	CAudioCollect*		AuCol			= (CAudioCollect*)dwInstance;
	PWAVEHDR			hdrIn			= (PWAVEHDR)wParam;
	int					nextClip		= (AuCol->nAudioIn + 1)% BLOCK_SIZE ;
	switch (uMsg)
	{
	case WIM_DATA:
		if(AuCol->m_WaveInClips[nextClip].dwLength == 0)//��һ���顱�ǿյ�,���豸����������ݸ��Ƶ���ǰ����
		{
			AuCol->m_WaveInClips[AuCol->nAudioIn].pData
				= (PBYTE)realloc(AuCol->m_WaveInClips[AuCol->nAudioIn].pData,
				hdrIn->dwBytesRecorded/* + AuCol->m_WaveInClips[AuCol->nAudioIn].dwLength*/);
			CopyMemory ((AuCol->m_WaveInClips[AuCol->nAudioIn].pData/* + AuCol->m_WaveInClips[AuCol->nAudioIn].dwLength*/), 
				hdrIn->lpData,
				hdrIn->dwBytesRecorded) ;//(*destination,*resource,nLen);
			AuCol->m_WaveInClips[AuCol->nAudioIn].dwLength /*+*/= hdrIn->dwBytesRecorded;
		}
		else//����������ǰ����
		{
		}
		AuCol->nAudioIn		=	nextClip;
		if(AuCol->bStart)
			mmr = waveInAddBuffer(hwi, (PWAVEHDR)wParam, sizeof(WAVEHDR));
		break;
	case WIM_CLOSE:
		if(AuCol->bStart)
			for(int i=0; i<BUFFER_IN_SIZE; i++)
			{
				waveInUnprepareHeader(hwi, AuCol->pWaveHdrIn[i], sizeof(WAVEHDR) );
				free(AuCol->pWaveHdrIn[i]->lpData);
			}
		break;
	default:
		break;
	}
}

void CALLBACK _PlayProc( HWAVEOUT hwo,  UINT uMsg,  DWORD_PTR dwInstance,  DWORD_PTR wParam,  DWORD_PTR lParam)
{
	CAudioCollect*		AuCol			= (CAudioCollect*)dwInstance;
	PWAVEHDR			hdrOut			= (PWAVEHDR)wParam;
	int					nextClip		= (AuCol->nAudioOut + 1)% BLOCK_SIZE ;

	switch (uMsg)
	{
	case WOM_DONE:
		//���õ�ǰ�������
		free(AuCol->m_WaveOutClips[AuCol->nAudioOut].pData);
		AuCol->m_WaveOutClips[AuCol->nAudioOut].pData			= reinterpret_cast<PBYTE>(malloc(1));
		AuCol->m_WaveOutClips[AuCol->nAudioOut].dwLength		= 0;

			
		//while(AuCol->m_WaveOutClips[AuCol->nAudioOut].dwLength == 0){}//����ȵ���һ�������ݲ��������˾�
			
		//�����ż�����ָ����һ����, ����һ�����ݸ��Ƶ��豸����
		AuCol->nAudioOut		= (AuCol->nAudioOut + 1) % BLOCK_SIZE;
		hdrOut->lpData          =	(LPTSTR)AuCol->m_WaveOutClips[AuCol->nAudioOut].pData ;
		hdrOut->dwBufferLength  =	AuCol->m_WaveOutClips[AuCol->nAudioOut].dwLength ;
			

		//��ʼ������һ��
		if(AuCol->bStart)//��ֹwaveOutReset����
		{
			waveOutPrepareHeader (hwo,hdrOut,sizeof(WAVEHDR));
			waveOutWrite(hwo,hdrOut,sizeof(WAVEHDR));
		}

		break;
	case WOM_CLOSE:	
		if(AuCol->bStart)
			for(int i=0; i<BUFFER_OUT_SIZE; i++)
			{
				waveOutUnprepareHeader(hwo, AuCol->pWaveHdrOut[i], sizeof(WAVEHDR) );
				free(AuCol->pWaveHdrOut[i]->lpData);
			}
		break;
	}

}

DWORD WINAPI _SendClips(LPVOID lpParam)
{
	CAudioCollect*		AuCol			= (CAudioCollect*)lpParam;
	//��ϢԤ��д
	MsgInfo				msg_send;
	if(AuCol->m_tkType == P2P_CHAT)
	{
		msg_send.type = P2P_TALK;
	}
	else
	{
		msg_send.type = P2A_TALK;
	}
	msg_send.sourceID	= AuCol->m_tkID;
	msg_send.CltID		= AuCol->m_tkID2;
	while(AuCol->bStart)
	{
		if(AuCol->m_WaveInClips[AuCol->nSend].dwLength != 0)//���ͼ�������ǰָ��Ŀ�������
		{
			ZeroMemory(&(msg_send.textBuffer), sizeof(msg_send.textBuffer));
			CopyMemory(msg_send.textBuffer, 
				AuCol->m_WaveInClips[AuCol->nSend].pData, 
				AuCol->m_WaveInClips[AuCol->nSend].dwLength);
			AuCol->m_pDlg->m_Comm.SendMsg(msg_send);
			//AuCol->RecvWaveClips(msg_send);//test

			//��д�÷��Ͱ������õ�ǰ�������
			free(AuCol->m_WaveInClips[AuCol->nSend].pData);
			AuCol->m_WaveInClips[AuCol->nSend].pData		= reinterpret_cast<PBYTE>(malloc(1));
			AuCol->m_WaveInClips[AuCol->nSend].dwLength	= 0;
		}
		AuCol->nSend = (AuCol->nSend + 1) % BLOCK_SIZE;//ָ����һ��
	}

	return 0;
}


//��msg���ݷŽ�clip�����Թ�����
void CAudioCollect::RecvWaveClips(MSG_INFO msg_play)
{
	if(m_WaveOutClips[nRecv].dwLength == 0)//���ռ�������ǰָ��Ŀ�û������
	{
		m_WaveOutClips[nRecv].pData
			= (PBYTE)realloc(m_WaveOutClips[nRecv].pData,
			RECORD_BUFFER);
		CopyMemory(m_WaveOutClips[nRecv].pData,
			msg_play.textBuffer, 
			RECORD_BUFFER);//�п��ܳ���
		m_WaveOutClips[nRecv].dwLength = RECORD_BUFFER;
	}
	nRecv = (nRecv + 1) % BLOCK_SIZE;

}

//DWORD WINAPI _RecvClips(LPVOID lpParam)
//{
//	CAudioCollect*		AuCol			= (CAudioCollect*)lpParam;
//
//	while(AuCol->bStart)
//	{
//
//
//	}
//
//	return 0;
//}


//��ʼ������¼������, clip����, ������Ļ���ֻ��ÿ��������1 ���ڴ�
void CAudioCollect::InitAudioCollect(MSG_TYPE type, int id, int id2)
{
	m_tkType			= type;
	m_tkID				= id;
	m_tkID2				= id2;

	//�߳̾��
	thWaveIn			= NULL;
	thWaveOut			= NULL;
	
	//�豸���
	m_hWaveIn			= NULL;
	m_hWaveOut			= NULL;

	//���������ռ��Ͳ��Ÿ�ʽ
	m_wavefmt.wFormatTag			= WAVE_FORMAT_PCM;
	m_wavefmt.nChannels				=1;
	m_wavefmt.nSamplesPerSec		=11025;
	m_wavefmt.nAvgBytesPerSec		=11025;
	m_wavefmt.nBlockAlign			=1;
	m_wavefmt.wBitsPerSample		=8;
	m_wavefmt.cbSize				=0;

	//����������ÿһ"��"������
	for(int i=0; i<BLOCK_SIZE; i++)
	{
		m_WaveInClips[i].dwLength		= 0;
		m_WaveInClips[i].pData			= reinterpret_cast<PBYTE>(malloc(1));

		m_WaveOutClips[i].dwLength		= 0;
		m_WaveOutClips[i].pData			= reinterpret_cast<PBYTE>(malloc(1));
	}
	nSend				 = 0;
	nRecv				 = 0;
	nAudioIn			 = 0;
	nAudioOut			 = 0;


	//׼�������������ͷ, ������Ҫ�󶨵Ļ�����, �������ʹ�ö໺��. ���豸����, ��ʼ��ʱֻΪ�豸��������㹻����ڴ�
	for(int i=0; i<BUFFER_IN_SIZE; i++)
	{
		pWaveHdrIn[i]	= reinterpret_cast<PWAVEHDR>(malloc (sizeof (WAVEHDR))) ;
		pBufferIn[i]	= reinterpret_cast<PBYTE>(malloc(RECORD_BUFFER));
		pWaveHdrIn[i]->lpData           = (LPTSTR)pBufferIn[i];   
		pWaveHdrIn[i]->dwBufferLength   = RECORD_BUFFER;   
		pWaveHdrIn[i]->dwBytesRecorded  = 0;   
		pWaveHdrIn[i]->dwUser           = 0;   
		pWaveHdrIn[i]->dwFlags          = 0;   
		pWaveHdrIn[i]->dwLoops          = 1;   
		pWaveHdrIn[i]->lpNext           = 0;   
		pWaveHdrIn[i]->reserved         = 0;   
	}
	for(int i=0; i<BUFFER_OUT_SIZE; i++)
	{
		pWaveHdrOut[i]	= reinterpret_cast<PWAVEHDR>(malloc (sizeof (WAVEHDR))) ;
		pBufferOut[i]	= reinterpret_cast<PBYTE>(malloc(RECORD_BUFFER));
		pWaveHdrOut[i]->lpData           = (LPTSTR)pBufferOut[i];   
		pWaveHdrOut[i]->dwBufferLength   = RECORD_BUFFER;   
		pWaveHdrOut[i]->dwBytesRecorded  = 0;   
		pWaveHdrOut[i]->dwUser           = 0;   
		pWaveHdrOut[i]->dwFlags          = 0;   
		pWaveHdrOut[i]->dwLoops          = 1;   
		pWaveHdrOut[i]->lpNext           = 0;   
		pWaveHdrOut[i]->reserved         = 0;   
	}


}

void CAudioCollect::UnInitAudioCollect()//seems useless
{
	//����������ÿһ"��"������
	for(int i=0; i<BLOCK_SIZE; i++)
	{
		m_WaveInClips[i].dwLength		= 0;
		m_WaveInClips[i].pData			= reinterpret_cast<PBYTE>(malloc(1));

		m_WaveOutClips[i].dwLength		= 0;
		m_WaveOutClips[i].pData			= reinterpret_cast<PBYTE>(malloc(1));
	}
	nSend				 = 0;
	nRecv				 = 0;
	nAudioIn			 = 0;
	nAudioOut			 = 0;
}

		

///////////////////////////////////////
//				¼������
//					 ��
//      ��¼���豸�� waveInOpen   
//                   ��   
//Ϊ¼���豸׼�����棺 waveInPrepareHeader   
//                   ��   
//Ϊ�����豸���ӻ��棺 waveInAddBuffer   
//                   ��   
//          ����¼���� waveInStart   
////////////////////////////////////////
//				��������
//					 ��
//        ������豸�� waveOutOpen   
//                     ��   
//  Ϊ����豸׼�����棺 waveOutPrepareHeader   
//                     ��   
//д���ݵ�����豸���棺 waveOutWrite   

void CAudioCollect::Start()
{
	MMRESULT											mmr;
	bStart		= true;
	bReset		= false;
	thSend		= CreateThread(0, 0, _SendClips, this, 0, 0);
	CloseHandle(thSend);

	//thWaveIn	= CreateThread(0, 0, _RecordThread, this, 0, 0);
	//thWaveOut	= CreateThread(0, 0, _PlayThread, this, 0, 0);
	//CloseHandle(thWaveIn);
	//CloseHandle(thWaveOut);

	//¼������
	if( waveInOpen(&m_hWaveIn, WAVE_MAPPER, &m_wavefmt, (DWORD_PTR)_RecordProc, (DWORD_PTR)this, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
	{
		TRACE("������������!");
	}
	for(int i=0; i<BUFFER_IN_SIZE; i++)
	{
		mmr = waveInPrepareHeader(m_hWaveIn, pWaveHdrIn[i], sizeof(WAVEHDR) );
		mmr = waveInAddBuffer(m_hWaveIn, pWaveHdrIn[i], sizeof(WAVEHDR) );
	}
	mmr = waveInStart(m_hWaveIn);

	//���Ų���
	if( waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_wavefmt, (DWORD_PTR)_PlayProc, (DWORD_PTR)this, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) 
	{
		TRACE("�����������!");
	}
	for(int i=0; i<BUFFER_OUT_SIZE; i++)
	{
		mmr = waveOutPrepareHeader (m_hWaveOut, pWaveHdrOut[i], sizeof (WAVEHDR)) ;
		mmr = waveOutWrite (m_hWaveOut, pWaveHdrOut[i], sizeof (WAVEHDR)) ;
	}
	//::Sleep(100);
	//MSG				msg;
	//msg.message = WM_QUIT;
	//PostThreadMessage(GetThreadId(thWaveIn), msg.message, 0, 0);
}

void CAudioCollect::Stop()
{
	bStart = false;
	bReset = true;
	waveOutReset(m_hWaveOut);
	waveOutClose(m_hWaveOut);
	waveInReset(m_hWaveIn);
	waveInClose(m_hWaveIn);
}




/*-----------------------------------------------------------------------------------------------------------------------------------------------*/