
// ChatClient1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChatClient1App:
// �йش����ʵ�֣������ ChatClient1.cpp
//

class CChatClient1App : public CWinApp
{
public:
	CChatClient1App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChatClient1App theApp;