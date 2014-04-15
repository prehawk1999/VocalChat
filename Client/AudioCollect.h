#pragma once

#include "mmsystem.h"   
#include "Msg.h"
#pragma comment(lib,"Winmm.lib")  
#define BLOCK_SIZE					16
#define BUFFER_IN_SIZE				2
#define BUFFER_OUT_SIZE				2

typedef struct WAVEHEAD_INFO
{
	DWORD							dwDataLength;
	PBYTE							pBuffer1, pBuffer2, pSaveBuffer;  //pSaveBuffer才是真正存放音频的地方
	PWAVEHDR						pWaveHdr1, pWaveHdr2; //上面两个buffer分别与这两个头结构进行绑定, 用于waveInAddbuffer
}WavInfo, *pWavInfo;


typedef struct WAVE_BUFFER
{
	DWORD							dwLength;
	PBYTE							pData;
}WaveClip, *pWaveClip;

class CChatClient1Dlg;
class CAudioCollect
{
public:
	CAudioCollect(void);
	~CAudioCollect(void);

	/*----------old----------*/
public:
	void StartRecording();
	void StopRecording();
	void StartPlaying();
	void StopPlaying();
	/*------------------------*/



	/*---------new------------*/
public:
	CChatClient1Dlg*				m_pDlg;

	HWAVEIN							m_hWaveIn;
	HWAVEOUT						m_hWaveOut;
	WAVEFORMATEX					m_wavefmt;
	HANDLE							thWaveIn, thWaveOut, thSend;
	int								nAudioIn, nAudioOut, nSend, nRecv;
	WaveClip						m_WaveInClips[BLOCK_SIZE], m_WaveOutClips[BLOCK_SIZE];
	bool							bStart, bReset;

	PBYTE							pBufferIn[BUFFER_IN_SIZE], pBufferOut[BUFFER_OUT_SIZE];
	PWAVEHDR						pWaveHdrIn[BUFFER_IN_SIZE], pWaveHdrOut[BUFFER_OUT_SIZE];

	//消息对象信息
	MSG_TYPE					m_tkType;
	int							m_tkID, m_tkID2;

	void SetMainDlg(CChatClient1Dlg* pDlg);
	void InitAudioCollect(MSG_TYPE type, int id, int id2);
	void UnInitAudioCollect();
	void SendWaveClips();
	void RecvWaveClips(MSG_INFO msg_play);
	void Start();
	void Stop();
	/*-------------------------*/

public:
	pWavInfo						m_pRecordInfo, m_pPlayInfo;

	PBYTE							pBuffer1, pBuffer2, pBuffer3, pBuffer4, pSaveBuffer, pPlayBuffer, pNewBuffer;
	PWAVEHDR						pWaveHdr1, pWaveHdr2, pWaveHdr3, pWaveHdr4;
	DWORD							dwDataLength;

	
};

