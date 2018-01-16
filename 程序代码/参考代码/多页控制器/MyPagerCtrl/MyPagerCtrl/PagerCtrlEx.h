
/////////////////////////////////////////////////////////////////////////////
// Name:        PAGERCTRLEX.H
// Purpose:     ��ҳ�ؼ�
// Author:      yfqvip@163.com
// Modified by: 
// Created:     2011/2/15 16:16
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef __PAGERCTRLEX_H__
#define __PAGERCTRLEX_H__
#include "ImgButton.h"
#include "Label.h"

enum ButtonItem
{
	ITEM_FIRST,
	ITEM_PREV,
	ITEM_NEXT,
	ITEM_LAST
};

class CPagerNotify
{
public:
	// ������0��ʼ
	virtual void OnGotoPager(int nIndex){}
};

class CPagerCtrlEx : public CWnd
{
	DECLARE_DYNCREATE(CPagerCtrlEx)
public:
	CPagerCtrlEx(void);
	~CPagerCtrlEx(void);
	virtual BOOL Create(const CPoint& pt, CWnd* pParentWnd);
	void SetNotifyManager(CPagerNotify *pNotify);
	// ������1��ʼ
	void SetProperty(int nPageTotal,int nCurrentIndex);
	void SetBitmaps(ButtonItem eItem,int nNormal = 0, int nHot = 0, int nPressed = 0, int nDisable = 0);
	void SetBorderColor(COLORREF col);
private:
	void SetPageText();
	void GotoPager(int nPage);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	CWnd*			m_pParentWnd;
	CImgButton		m_btnFirst;
	CImgButton		m_btnPrev;
	CImgButton		m_btnNext;
	CImgButton		m_btnLast;
	CLabel			m_lblPanel;			/*< ����򸸿ؼ� */
	CEdit			m_txtGoto;
	CLabel			m_lblText;
	CPagerNotify*	m_pNotify;
	int				m_nPageTotal;		/*< �������㿪ʼ */
	int				m_nCurrentIndex;	/*< �������㿪ʼ */
	COLORREF		m_BorderColor; 
};

#endif //__PAGERCTRLEX_H__