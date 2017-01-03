
// �˵��Ⲩ��չʾDlg.h : ͷ�ļ�
//

#pragma once
#include "Wav_File_Handle.h"

// C�˵��Ⲩ��չʾDlg �Ի���
class C�˵��Ⲩ��չʾDlg : public CDialogEx
{
// ����
public:
	C�˵��Ⲩ��չʾDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	FILE *fp;
	Wav_File_Work *File;
	
	CPen newPen;                                                               //���ڴ����»���   
	CPen *pOldPen;                                                             //���ڴ�žɻ���   
	CBrush newBrush;                                                           //���ڴ����»�ˢ   
	CBrush *pOldBrush;                                                         //���ڴ�žɻ�ˢ   
	int width;
	int height;

public:
	void StartDraw(CRect rc, CDC* pdc, int flag,int f);
	void Plot1(CRect rc, CDC* pdc, int flag);
	void Plot2(CRect rc, CDC* pdc, int flag);
	void Plot3(CRect rc, CDC* pdc, int flag);
};
