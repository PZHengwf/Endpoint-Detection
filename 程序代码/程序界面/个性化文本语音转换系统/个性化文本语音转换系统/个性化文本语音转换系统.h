
// ���Ի��ı�����ת��ϵͳ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C���Ի��ı�����ת��ϵͳApp:
// �йش����ʵ�֣������ ���Ի��ı�����ת��ϵͳ.cpp
//

class C���Ի��ı�����ת��ϵͳApp : public CWinApp
{
public:
	C���Ի��ı�����ת��ϵͳApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C���Ի��ı�����ת��ϵͳApp theApp;