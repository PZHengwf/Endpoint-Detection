
// �˵��Ⲩ��չʾDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�˵��Ⲩ��չʾ.h"
#include "�˵��Ⲩ��չʾDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double C�˵��Ⲩ��չʾDlg::DEVIATION = 0.3;                             //ƫ��ֵ��ʼ��(ֻ����Cpp�г�ʼ��)

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C�˵��Ⲩ��չʾDlg �Ի���




C�˵��Ⲩ��չʾDlg::C�˵��Ⲩ��չʾDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C�˵��Ⲩ��չʾDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C�˵��Ⲩ��չʾDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, ctrlSlider_threshold_h_EngChart);          //��ʼ������
	DDX_Control(pDX, IDC_SLIDER2, ctrlSlider_threshold_l_EngChart);          //��ʼ������
	DDX_Control(pDX, IDC_SLIDER3, ctrlSlider_threshold_h_CepsDistance);      //��ʼ������
	DDX_Control(pDX, IDC_SLIDER4, ctrlSlider_threshold_l_CepsDistance);      //��ʼ������
	DDX_Control(pDX, IDC_SLIDER5, ctrlSlider_threshold_h_EngDevia);          //��ʼ������
	DDX_Control(pDX, IDC_SLIDER6, ctrlSlider_threshold_l_EngDevia);          //��ʼ������
	DDX_Control(pDX, IDC_SLIDER7, ctrlSlider_threshold_h_SpectrumEnt);       //��ʼ������
	DDX_Control(pDX, IDC_SLIDER8, ctrlSlider_threshold_l_SpectrumEnt);       //��ʼ������

	DDX_Control(pDX, IDC_SLIDER9, ctrlSlider_interim_StateLength);           //��ʼ������
	DDX_Control(pDX, IDC_SLIDER10, ctrlSlider_burst_StateLength);            //��ʼ������
}

BEGIN_MESSAGE_MAP(C�˵��Ⲩ��չʾDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMMAND1, &C�˵��Ⲩ��չʾDlg::OnBnClickedCommand1)
	ON_BN_CLICKED(IDC_BUTTON1, &C�˵��Ⲩ��չʾDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider8)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER9, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider9)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER10, &C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider10)
	ON_BN_CLICKED(IDC_BUTTON2, &C�˵��Ⲩ��չʾDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// C�˵��Ⲩ��չʾDlg ��Ϣ�������

BOOL C�˵��Ⲩ��չʾDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ִ�г�ʼ������

	ctrlSlider_threshold_h_EngChart.SetRange(0, 100);
	ctrlSlider_threshold_l_EngChart.SetRange(0, 100);
	ctrlSlider_threshold_h_CepsDistance.SetRange(0, 100);
	ctrlSlider_threshold_l_CepsDistance.SetRange(0, 100);
	ctrlSlider_threshold_h_EngDevia.SetRange(0, 100);
	ctrlSlider_threshold_l_EngDevia.SetRange(0, 100);
	ctrlSlider_threshold_h_SpectrumEnt.SetRange(0, 100);
	ctrlSlider_threshold_l_SpectrumEnt.SetRange(0, 100);

	ctrlSlider_interim_StateLength.SetRange(5, 30);
	ctrlSlider_burst_StateLength.SetRange(5, 30);

	ctrlSlider_threshold_h_EngChart.SetPos(80);
	ctrlSlider_threshold_l_EngChart.SetPos(20);
	ctrlSlider_threshold_h_CepsDistance.SetPos(80);
	ctrlSlider_threshold_l_CepsDistance.SetPos(20);
	ctrlSlider_threshold_h_EngDevia.SetPos(80);
	ctrlSlider_threshold_l_EngDevia.SetPos(20);
	ctrlSlider_threshold_h_SpectrumEnt.SetPos(80);
	ctrlSlider_threshold_l_SpectrumEnt.SetPos(20);

	ctrlSlider_interim_StateLength.SetPos(10);
	ctrlSlider_burst_StateLength.SetPos(5);

	CString str1 = (CString)("0.80");
	SetDlgItemText(IDC_EDIT2, str1);
	SetDlgItemText(IDC_EDIT4, str1);
	SetDlgItemText(IDC_EDIT6, str1);
	SetDlgItemText(IDC_EDIT8, str1);
	CString str2 = (CString)("0.20");
	SetDlgItemText(IDC_EDIT3, str2);
	SetDlgItemText(IDC_EDIT5, str2);
	SetDlgItemText(IDC_EDIT7, str2);
	SetDlgItemText(IDC_EDIT9, str2);

	SetDlgItemText(IDC_EDIT10, (CString)("10"));
	SetDlgItemText(IDC_EDIT11, (CString)("5"));

	ShowWindow(SW_FORCEMINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	repaint = false;
	fp = NULL;
	File = NULL;
	/*
	if(AllocConsole()){                                                        //��������һ��DOS����������ʾ����
		SetConsoleTitleW(_T("���������¶˵��Ⲩ��չʾ���ݵ���"));              //����DOS���ڵı���
		hdWrite = GetStdHandle(STD_OUTPUT_HANDLE);                             //��ȡ���ھ������������
	}
	*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C�˵��Ⲩ��չʾDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C�˵��Ⲩ��չʾDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	if (repaint) {                                                           //repaint��Ϊ�ػ��ʶ�����״�ִ��ʱ�ػ津����ʧ�ܣ��ȿ�ʼ�ػ�ʱ�����ػ�
		Repaint();                                                           //�ػ�����Ҫ�Ĳ���
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C�˵��Ⲩ��չʾDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C�˵��Ⲩ��չʾDlg::Repaint()
{
	CPaintDC dc(this);                                                       //׼������
	CWnd* pWnd;
	CRect rc;
	CDC* pdc;

	for (int i = 0; i < 5; ++i) {                                            //�ֱ��5�������ͼ
		pWnd = GetDlgItem(IDC_STATIC1 + i);
		pWnd->GetWindowRect(rc);
		pdc = pWnd->GetDC();
		StartDraw(rc, pdc, 1, i + 1);                                        //����ĳһ�����е�ͼ��
	}
}

void C�˵��Ⲩ��չʾDlg::StartDraw(CRect rc, CDC* pdc, int flag, int Flag)
{
	width = rc.Width();
	height = rc.Height();

	CDC MemDC;                                                                 //���ȶ���һ����ʾ�豸����   
	CBitmap MemBitmap;                                                         //����һ��λͼ����
	MemDC.CreateCompatibleDC(NULL);                                            //���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                      //���潨��һ������Ļ��ʾ���ݵ�λͼ
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                         //��λͼѡ�뵽�ڴ���ʾ�豸��//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	MemDC.FillSolidRect(0, 00, width, height, RGB(255, 255, 255));             //���ñ���ɫ��λͼ����ɾ�

	MemDC.Rectangle(1, 1, width - 1, height - 2);
	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));                             //����ʵ�Ļ��ʣ��ֶ�Ϊ1����ɫΪ��ɫ
	pOldPen = MemDC.SelectObject(&newPen);                                     //ѡ���»��ʣ������ɻ��ʵ�ָ�뱣�浽pOldPen

	switch (Flag)                                                              //����ͼ��
	{
	case 1:
		Plot1(rc, &MemDC, 1);
		break;
	case 2:
		pdc->Rectangle(0, 0, width, height);
		Plot2(rc, &MemDC, 1);
		break;
	case 3:
		pdc->Rectangle(0, 0, width, height);
		Plot3(rc, &MemDC, 1);
		break;
	case 4:
		pdc->Rectangle(0, 0, width, height);
		Plot4(rc, &MemDC, 1);
		break;
	case 5:
		pdc->Rectangle(0, 0, width, height);
		Plot5(rc, &MemDC, 1);
		break;
	default:
		break;
	}

	MemDC.SelectObject(pOldPen);                                               //�ָ��ɻ���
	newPen.DeleteObject();                                                     //ɾ���»���

	if (Flag == 1 || Flag == 3) {                                              //���ƺ�����
		MemDC.MoveTo(0, height / 2);                                           //���Ὺͷ
		MemDC.LineTo(width, height / 2);                                       //�����β
		for (int i = 0; i < width / 100; ++i) {                                //���Ʊ��
			MemDC.MoveTo((i + 1) * 100, height / 2);
			MemDC.LineTo((i + 1) * 100, height / 2 - 10);
		}
	}

	pdc->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);                   //���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ

	MemBitmap.DeleteObject();                                                  //��ͼ��ɺ������
	MemDC.DeleteDC();
}

void C�˵��Ⲩ��չʾDlg::Plot1(CRect rc, CDC* pdc, int flag)
{
	/*
	int max, min;
	double highMax, highMin;
	double tempNumberMax, tempNumberMin;

	for (int i = 0; i < width; ++i) {                                          //�������������ͼ��
		max = min = i*File->Get_dataNumber() / width;
		for (double j = i*File->Get_dataNumber() / width; j < (i + 1)*File->Get_dataNumber() / width; ++j) { //�������ط�Χ֮��
			if (File->Get_DataDouble((int)j) > File->Get_DataDouble(max)) {
				max = (int)j;
			}
			if (File->Get_DataDouble((int)j) < File->Get_DataDouble(min)) {
				min = (int)j;
			}
		}
		tempNumberMax = File->Get_DataDouble(max);
		tempNumberMin = File->Get_DataDouble(min);

		highMax = height/2 - (height/2)*tempNumberMax;
		highMin = height/2 - (height/2)*tempNumberMin;

		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
	*/
	this->getTempVoiceData();
	this->screenShowData(showMaxData, viabilityMaxData);
	this->screenShowData(showMinData, viabilityMinData);
	double highMax, highMin;
	int topMax, topMin, bottomMax, bottomMin;
	topMax = topMin = bottomMax = bottomMin = 0;

	for (int i = 0; i < width; ++i) {
		if (viabilityMaxData[i] && showMaxData[i] > showMaxData[topMax]) {
			topMax = i;
		}
		if (viabilityMaxData[i] && showMaxData[i] < showMaxData[topMin]) {
			topMin = i;
		}
		if (viabilityMinData[i] && showMinData[i] > showMinData[bottomMax]) {
			bottomMax = i;
		}
		if (viabilityMinData[i] && showMinData[i] < showMinData[bottomMin]) {
			bottomMin = i;
		}
	}
	double unitTop = fabs(fabs(showMaxData[topMax]) - fabs(showMaxData[topMin]));
	double unitBottom = fabs(fabs(showMinData[bottomMax]) - fabs(showMinData[bottomMin]));

	vector<int> highTop, highBottom;
	for (int i = 0; i < width; ++i) {
		highMax = height / 2 - (fabs(showMaxData[i]) - fabs(showMaxData[topMin])) * (height / 2 / unitTop);
		highMin = height / 2 + (fabs(showMinData[i]) - fabs(showMinData[bottomMax])) * (height / 2 / unitBottom);
		if (!viabilityMaxData[i]) {
			highMax = height / 2;
		}
		if (!viabilityMinData[i]) {
			highMin = height / 2;
		}
		highTop.push_back((int)highMin);
		highBottom.push_back((int)highMax);
	}

	for (int i = 0; i < width; ++i) {
		pdc->MoveTo(i, highTop[i]);
		pdc->LineTo(i, highBottom[i]);
	}

	for (unsigned long i = 0; i < File->Get_voiceNumber(); ++i) {
		int num = File->Get_voiceNumber();
		VoiceParagraph tempPoint = File->Get_dataVoicePoint(i);
		int begin = (int)(tempPoint.begin / (File->Get_FrameNumber() / width));
		int end = (int)(tempPoint.end / (File->Get_FrameNumber() / width));
		pdc->MoveTo(begin, 1);
		pdc->LineTo(begin, height);
		pdc->MoveTo(end, 1);
		pdc->LineTo(end, height);
		pdc->DrawText((CString)("begin"), CRect(begin + 10, height - 20, begin + 50, height), 0);
		pdc->DrawText((CString)("end"), CRect(end + 10, height - 20, end + 50, height), 0);
	}

}

void C�˵��Ⲩ��չʾDlg::Plot2(CRect rc, CDC* pdc, int flag)
{
	pdc->MoveTo(0, (int)height);
	for (int i = 0; i < width; ++i) {                                          //�������������ͼ��
		int max = 0;
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //�������ط�Χ֮��
			if (File->Get_DataCepsDistance((unsigned long)j) > File->Get_DataCepsDistance(max)) {
				max = (int)j;
			}
		}
		double tempNumberMax = File->Get_DataCepsDistance(max);
		double highMax = height - height*tempNumberMax;
		pdc->LineTo(i, (int)highMax);
	}
}
void C�˵��Ⲩ��չʾDlg::Plot3(CRect rc, CDC* pdc, int flag)
{
	int max, min;
	double highMax, highMin;
	double tempNumberMax, tempNumberMin;

	pdc->MoveTo(0, (int)height / 2);
	for (int i = 0; i < width; ++i) {                                          //�������������ͼ��
		max = min = 0;
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //�������ط�Χ֮��
			if (File->Get_DataEngChart((unsigned long)j) > File->Get_DataEngChart(max)
				&& File->Get_DataEngChart((unsigned long)j) <= 1
				&& File->Get_DataEngChart((unsigned long)j) >= -1
				) {
				max = (int)j;
			}
			if (File->Get_DataEngChart((unsigned long)j) < File->Get_DataEngChart(min)
				&& File->Get_DataEngChart((unsigned long)j) <= 1
				&& File->Get_DataEngChart((unsigned long)j) >= -1
				) {
				min = (int)j;
			}
		}
		tempNumberMax = File->Get_DataEngChart(max);
		tempNumberMin = File->Get_DataEngChart(min);

		highMax = height / 2 - (height / 2)*tempNumberMax;
		highMin = height / 2 - (height / 2)*tempNumberMin;

		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}
void C�˵��Ⲩ��չʾDlg::Plot4(CRect rc, CDC* pdc, int flag)
{
	int max, min;
	double highMax, highMin;
	double tempNumberMax, tempNumberMin;

	pdc->MoveTo(0, (int)height / 2);
	for (int i = 0; i < width; ++i) {                                          //�������������ͼ��
		max = min = 0;
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //�������ط�Χ֮��
			if (File->Get_DataEngDevia((unsigned long)j) > File->Get_DataEngDevia(max)) {
				max = (int)j;
			}
			if (File->Get_DataEngDevia((unsigned long)j) < File->Get_DataEngDevia(min)) {
				min = (int)j;
			}
		}
		tempNumberMax = File->Get_DataEngDevia(max);
		tempNumberMin = File->Get_DataEngDevia(min);

		tempNumberMax *= 10;
		tempNumberMin *= 10;

		tempNumberMax = tempNumberMax - (int)tempNumberMax;
		tempNumberMin = tempNumberMin - (int)tempNumberMin;

		highMax = height - height*tempNumberMax;
		highMin = height - height*tempNumberMin;

		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}
void C�˵��Ⲩ��չʾDlg::Plot5(CRect rc, CDC* pdc, int flag)
{
	pdc->MoveTo(0, height);
	for (int i = 0; i < width; ++i) {                                          //�������������ͼ��
		int max = 0, min = 0;
		max = min = (int)(i*File->Get_FrameNumber() / width);
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //�������ط�Χ֮��
			if (File->Get_DataSpectrumEnt((unsigned long)j) > File->Get_DataSpectrumEnt(max)) {
				max = (int)j;
			}
			if (File->Get_DataSpectrumEnt((unsigned long)j) < File->Get_DataSpectrumEnt(min)) {
				min = (int)j;
			}
		}
		double tempNumberMax = File->Get_DataSpectrumEnt(max);
		double tempNumberMin = File->Get_DataSpectrumEnt(min);
		double highMax = height / 2 - (height / 2)*tempNumberMax;
		double highMin = height / 2 - (height / 2) * tempNumberMin;
		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}

void C�˵��Ⲩ��չʾDlg::OnBnClickedCommand1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CFileDialog opendlg(TRUE, _T("*.wav"), _T("*.wav"), OFN_OVERWRITEPROMPT, _T("�����ļ�(*.wav*;)|*.wav*||"), NULL);   //���ļ�ѡ���
	if (opendlg.DoModal() == IDOK)
	{
		fileName = opendlg.GetPathName();                                      //��ȡѡ����ļ���
	}
	SetDlgItemTextW(IDC_EDIT1, fileName);
}


void C�˵��Ⲩ��չʾDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	int nameLen = WideCharToMultiByte(CP_ACP, 0, fileName, -1, NULL, 0, NULL, NULL);
	char *fileNameChar = new char[nameLen + 1];
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameChar, nameLen, NULL, NULL);     //��CStringתΪchar*
	char *fileNameTemp = new char[nameLen + 20];

	int index = 0;                                                             //���ڱ������ļ�������
	for (int i = 0; i < nameLen + 1; ++i) {                                    //����'\'Ϊ'\\'�����ļ��е�·���ָ���Ϊ'\'���޷�׼ȷ��λ
		fileNameTemp[index++] = fileNameChar[i];
		if (fileNameChar[i] == '\\') {
			fileNameTemp[index++] = '\\';
		}
	}
	fileNameTemp[index] = 0;                                                   //��ֹԭ��û�ܱ������ļ�������

	SetWindowText(_T("�˵��Ⲩ��չʾ   | Reading file | Please waiting a minute!"));

	if ((fp = fopen(fileNameTemp, "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}
	File = new Wav_File_Work(fp);

	fclose(fp);
	delete[] fileNameChar;
	delete[] fileNameTemp;

	SetWindowText(_T("�˵��Ⲩ��չʾ   | Beging painting | Please waiting a minute!"));

	repaint = true;                                                            //��ʾ������ʼ�ػ�
	InvalidateRect(NULL, true);                                                //���ô�����Ч
	UpdateWindow();                                                            //�����ػ棬����WM_PAINT

	showMaxData.clear();
	showMinData.clear();
	viabilityMaxData.clear();
	viabilityMinData.clear();

	SetWindowText(_T("�˵��Ⲩ��չʾ"));
}

void C�˵��Ⲩ��չʾDlg::getTempVoiceData(void)                               //��ȡҪ���������
{
	int max, min;
	for (int i = 0; i < width; ++i) {                                          //�����ݷ�Χ��Ϊ��ȶΣ��ڶ����ҵ�Max��min
		max = min = i*File->Get_dataNumber() / width;
		for (double j = i*File->Get_dataNumber() / width;
			j < (i + 1)*File->Get_dataNumber() / width; ++j) {                 //�������ط�Χ֮��
			if (File->Get_DataDouble((int)j) > File->Get_DataDouble(max)) {
				max = (int)j;
			}
			if (File->Get_DataDouble((int)j) < File->Get_DataDouble(min)) {
				min = (int)j;
			}
		}
		this->showMaxData.push_back(File->Get_DataDouble(max));
		this->showMinData.push_back(File->Get_DataDouble(min));
		this->viabilityMaxData.push_back(true);
		this->viabilityMinData.push_back(true);
	}
}

void C�˵��Ⲩ��չʾDlg::screenShowData(vector<double> showData, vector<bool> viabilityData)    //����:��������ݣ����������ݣ�����min����ֵ�����ڼ��㷶Χ��ʾ�� �������������ɸѡ��ƫ��ֵ��30%���ڵ�����
{
	vector<double> deviationData;                                            //���ڱ�������ƫ��ֵ
	bool flag = true;                                                        //��Ϊһ����־����־��û��ɸѡ�ų����µ�����

	while (true) {
		int viabilityNumber = 0;                                             //���浱ǰ���е����ݵĸ��������ڼ���avg
		double sum = 0;
		for (int i = 0; i < (int)showData.size(); ++i) {
			if (viabilityData[i] == false) {                                 //�������Ϊ��Ϊ��������
				continue;
			}
			else {
				sum += showData[i];                                          //����������ݵĺ�
				++viabilityNumber;
			}
		}
		double avg = sum / viabilityNumber;

		for (int i = 0; i < (int)showData.size(); ++i) {
			if (viabilityData[i] == false) {
				deviationData.push_back(0);
			}
			else {                                                           //�������Ϊ��������
				deviationData.push_back(fabs(showData[i] / avg - 1));        //����ƫ��ֵ��ʽ
			}
		}

		flag = true;
		for (int i = 0; i < (int)showData.size(); ++i) {
			if (viabilityData[i] == false) {
				continue;
			}
			else {
				if (deviationData[i] > DEVIATION) {                          //����ƫ��ֵ��Ĳ���
					viabilityData[i] = false;                                //��ɸѡ�ų��ˣ�������Ϊfalse
					flag = false;
				}
			}
		}

		deviationData.clear();                                               //���ƫ��ֵ��Ϊ�´�ɸѡ��׼��

		if (flag) {                                                          //û��ɸѡ�����ݣ������˳�ѭ��
			break;
		}
	}
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_h_EngChart.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT2, str);
	Wav_File_Work::threshold_h_EngChart = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_l_EngChart.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT3, str);
	Wav_File_Work::threshold_l_EngChart = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_h_CepsDistance.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT4, str);
	Wav_File_Work::threshold_h_CepsDistance = trueData;

	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_l_CepsDistance.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT5, str);
	Wav_File_Work::threshold_l_CepsDistance = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_h_EngDevia.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT6, str);
	Wav_File_Work::threshold_h_EngDevia = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_l_EngDevia.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT7, str);
	Wav_File_Work::threshold_l_EngDevia = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_h_SpectrumEnt.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT8, str);
	Wav_File_Work::threshold_h_SpectrumEnt = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_threshold_l_SpectrumEnt.GetPos(); //��û���ĵ�ǰλ��
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT9, str);
	Wav_File_Work::threshold_l_SpectrumEnt = trueData;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_interim_StateLength.GetPos(); //��û���ĵ�ǰλ��
	CString str = _T("");
	str.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDIT10, str);
	Wav_File_Work::interim_StateLength = nPos;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = ctrlSlider_burst_StateLength.GetPos(); //��û���ĵ�ǰλ��
	CString str = _T("");
	str.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDIT11, str);
	Wav_File_Work::burst_StateLength = nPos;
	*pResult = 0;
}


void C�˵��Ⲩ��չʾDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(this->fp == NULL){
		MessageBoxA(NULL, "ERROR : NO File !", "ERROR", MB_ICONHAND);
	}else{
		File->SaveNewWav();
		MessageBoxA(NULL, "TIP : Voice created ok !", "TIP", MB_ICONASTERISK);
	}
}
