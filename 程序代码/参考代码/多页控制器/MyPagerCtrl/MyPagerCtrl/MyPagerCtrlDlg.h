
// MyPagerCtrlDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ImgButton.h"
#include "PagerCtrlEx.h"

// CMyPagerCtrlDlg �Ի���
class CMyPagerCtrlDlg : public CDialog, public CPagerNotify
{
// ����
public:
	CMyPagerCtrlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYPAGERCTRL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	virtual void OnGotoPager(int nIndex)
	{
		CString str;
		str.Format(_T("��%dҳ"), nIndex);
		//AfxMessageBox(str);
	}

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPagerCtrlEx m_Pager;
};
