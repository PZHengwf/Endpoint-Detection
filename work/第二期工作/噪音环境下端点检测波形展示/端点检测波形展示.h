
// �˵��Ⲩ��չʾ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C�˵��Ⲩ��չʾApp:
// �йش����ʵ�֣������ �˵��Ⲩ��չʾ.cpp
//

class C�˵��Ⲩ��չʾApp : public CWinApp
{
public:
	C�˵��Ⲩ��չʾApp();

	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C�˵��Ⲩ��չʾApp theApp;