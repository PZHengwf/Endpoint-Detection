
// Ƶ�ײ�������չʾ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CƵ�ײ�������չʾApp: 
// �йش����ʵ�֣������ Ƶ�ײ�������չʾ.cpp
//

class CƵ�ײ�������չʾApp : public CWinApp
{
public:
	CƵ�ײ�������չʾApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CƵ�ײ�������չʾApp theApp;