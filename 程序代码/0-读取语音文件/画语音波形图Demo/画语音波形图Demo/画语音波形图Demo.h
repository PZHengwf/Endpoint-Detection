
// ����������ͼDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C����������ͼDemoApp: 
// �йش����ʵ�֣������ ����������ͼDemo.cpp
//

class C����������ͼDemoApp : public CWinApp
{
public:
	C����������ͼDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C����������ͼDemoApp theApp;