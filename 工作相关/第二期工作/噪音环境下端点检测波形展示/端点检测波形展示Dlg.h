
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

	CSliderCtrl ctrlSlider_threshold_h_EngChart;
	CSliderCtrl ctrlSlider_threshold_l_EngChart;
	CSliderCtrl ctrlSlider_threshold_h_CepsDistance;
	CSliderCtrl ctrlSlider_threshold_l_CepsDistance;
	CSliderCtrl ctrlSlider_threshold_h_EngDevia;
	CSliderCtrl ctrlSlider_threshold_l_EngDevia;
	CSliderCtrl ctrlSlider_threshold_h_SpectrumEnt;
	CSliderCtrl ctrlSlider_threshold_l_SpectrumEnt;

	CSliderCtrl ctrlSlider_interim_StateLength;
	CSliderCtrl ctrlSlider_burst_StateLength;

	bool repaint;                                                              //�ж��Ƿ�Ϊ�ػ�ͼ��
	CString fileName;                                                          //����·��

	HANDLE hdWrite;                                                            //���ڱ���DOS���ڵľ��

	static const double DEVIATION;                                             //�����ƫ��ֵ��Χ
	vector<double> showMaxData, showMinData;                                   //�����ȡ������
	vector<bool> viabilityMaxData, viabilityMinData;                           //�������ݵĿ�����

	void getTempVoiceData(void);                                               //��ȡҪ���������
	void screenShowData(vector<double> showData, vector<bool> viabilityData);  //����:��������ݣ����������ݣ�����min����ֵ�����ڼ��㷶Χ��ʾ�� �������������ɸѡ��ƫ��ֵ��30%���ڵ�����
public:
	void Repaint();
	void StartDraw(CRect rc, CDC* pdc, int flag, int f);
	void Plot1(CRect rc, CDC* pdc, int flag);
	void Plot2(CRect rc, CDC* pdc, int flag);
	void Plot3(CRect rc, CDC* pdc, int flag);
	void Plot4(CRect rc, CDC* pdc, int flag);
	void Plot5(CRect rc, CDC* pdc, int flag);
	afx_msg void OnBnClickedCommand1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult);
};
