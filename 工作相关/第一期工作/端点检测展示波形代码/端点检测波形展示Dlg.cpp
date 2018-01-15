
// �˵��Ⲩ��չʾDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�˵��Ⲩ��չʾ.h"
#include "�˵��Ⲩ��չʾDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

BEGIN_MESSAGE_MAP(C�˵��Ⲩ��չʾDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
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

	ShowWindow(SW_FORCEMINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if((fp=fopen("΢��Ĭ��.wav", "rb"))==NULL){
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}
	File = new Wav_File_Work(fp);

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

	//==============================================================================================
	CPaintDC dc(this);
	CWnd* pWnd;
	CRect rc;
	CDC* pdc;

	pWnd = GetDlgItem(IDC_STATIC1);
	pWnd->GetWindowRect(rc);
	pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1, 1);

	pWnd = GetDlgItem(IDC_STATIC2);
	pWnd->GetWindowRect(rc);
	pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1, 2);

	pWnd = GetDlgItem(IDC_STATIC3);
	pWnd->GetWindowRect(rc);
	pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1, 3);
	//==============================================================================================
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C�˵��Ⲩ��չʾDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C�˵��Ⲩ��չʾDlg::StartDraw(CRect rc, CDC* pdc, int flag,int Flag)
{
	width = rc.Width();
	height = rc.Height();

	CDC MemDC;                                                                 //���ȶ���һ����ʾ�豸����   
	CBitmap MemBitmap;                                                         //����һ��λͼ����
	MemDC.CreateCompatibleDC(NULL);                                            //���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                      //���潨��һ������Ļ��ʾ���ݵ�λͼ
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                         //��λͼѡ�뵽�ڴ���ʾ�豸��//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	MemDC.FillSolidRect(0, 00, width, height, RGB(255, 255, 255));             //���ñ���ɫ��λͼ����ɾ�

	MemDC.Rectangle(1,1,width-1,height-1);
	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));                             //����ʵ�Ļ��ʣ��ֶ�Ϊ1����ɫΪ��ɫ
	pOldPen = MemDC.SelectObject(&newPen);                                     //ѡ���»��ʣ������ɻ��ʵ�ָ�뱣�浽pOldPen

	//����ͼ��
	switch (Flag)
	{
	case 1:
		Plot1(rc, &MemDC, 1);
		break;
	case 2:
		pdc->Rectangle(0,0,width,height);
		Plot2(rc, &MemDC, 1);
		break;
	case 3:
		pdc->Rectangle(0,0,width,height);
		Plot3(rc, &MemDC, 1);
	}
	MemDC.SelectObject(pOldPen);                                               //�ָ��ɻ���
	newPen.DeleteObject();                                                     //ɾ���»���
	if(Flag==1){
		MemDC.MoveTo(0,height/2);
		MemDC.LineTo(width,height/2);
		for(int i=0;i<width/100;++i){
			MemDC.MoveTo((i+1)*100,height/2);
			MemDC.LineTo((i+1)*100,height/2-10);
		}
		for(unsigned long i=0;i<File->Get_voiceNumber();++i){
			int num=File->Get_voiceNumber();
			VoiceParagraph tempPoint=File->Get_dataVoicePoint(i);
			int begin=(int)(tempPoint.begin/(File->Get_dataNumber() / width));
			int end=(int)(tempPoint.end/(File->Get_dataNumber() / width));
			MemDC.MoveTo(begin,1);
			MemDC.LineTo(begin,height);
			MemDC.MoveTo(end,1);
			MemDC.LineTo(end,height);
			MemDC.DrawText((CString)("begin"),CRect(begin+10,height-20,begin+50,height),0);
			MemDC.DrawText((CString)("end"),CRect(end+10,height-20,end+50,height),0);
		}
	}else{
		for(int i=0;i<width/100;++i){
			MemDC.MoveTo((i+1)*100,height);
			MemDC.LineTo((i+1)*100,height-10);
		}
	}

	pdc->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);                   //���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	//��ͼ��ɺ������
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void C�˵��Ⲩ��չʾDlg::Plot1(CRect rc, CDC* pdc, int flag)
{
	for (int i = 0; i < width; ++i) {
		int max = 0;
		int min = 0;
		for (double j = i*File->Get_dataNumber() / width; j < (i + 1)*File->Get_dataNumber() / width; ++j){
			if (File->Get_DataDouble((int)j) > File->Get_DataDouble(max)) {
				max = (int)j;
			}
			if (File->Get_DataDouble((int)j) < File->Get_DataDouble(min)) {
				min = (int)j;
			}
		}
		double tempNumberMax=File->Get_DataDouble(max);
		double tempNumberMin=File->Get_DataDouble(min);
		double highMax=height/2+(height/2)*tempNumberMax;
		double highMin=height/2+(height/2)*tempNumberMin;
		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}
void C�˵��Ⲩ��չʾDlg::Plot2(CRect rc, CDC* pdc, int flag)
{
	for (int i = 0; i < width; ++i) {
		double sum=0;
		for (double j = i*File->Get_dataEZNumber() / width; j < (i + 1)*File->Get_dataEZNumber() / width; ++j){
			sum+=File->Get_DataEnergy((unsigned long)j);
		}
		sum=sum/(File->Get_dataEZNumber() / width);
		double tempNumber=sum;
		double high=tempNumber*height/18;
		pdc->MoveTo(i, height);
		pdc->LineTo(i, (int)(height-high));
	}
}
void C�˵��Ⲩ��չʾDlg::Plot3(CRect rc, CDC* pdc, int flag)
{
	for (int i = 0; i < width; ++i) {
		int max = 0;
		for (double j = i*File->Get_dataEZNumber() / width; j < (i + 1)*File->Get_dataEZNumber() / width; ++j){
			if (File->Get_DataZCR((unsigned long)j) > File->Get_DataZCR(max)) {
				max = (int)j;
			}
		}
		double tempNumber=File->Get_DataZCR(max);
		double high=tempNumber*height/45;
		pdc->MoveTo(i, height);
		pdc->LineTo(i, (int)(height-high));
	}
}