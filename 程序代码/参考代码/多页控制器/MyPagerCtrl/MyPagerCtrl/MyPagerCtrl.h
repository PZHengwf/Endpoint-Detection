
// MyPagerCtrl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CMyPagerCtrlApp:
// �йش����ʵ�֣������ MyPagerCtrl.cpp
//

class CMyPagerCtrlApp : public CWinApp
{
public:
	CMyPagerCtrlApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyPagerCtrlApp theApp;

