
// ����������ͼDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "����������ͼDemo.h"
#include "����������ͼDemoDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C����������ͼDemoDlg �Ի���



C����������ͼDemoDlg::C����������ͼDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C����������ͼDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C����������ͼDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// C����������ͼDemoDlg ��Ϣ�������

BOOL C����������ͼDemoDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	if ((fp = fopen("E:\\����\\�Ŷ���Ŀ\\���Ի��ı�����ת��ϵͳ\\����\\΢��Ĭ��.wav", "rb")) == NULL) {
		MessageBox(_T("ERROR : File open failed !"), _T("ERROR"), MB_ICONHAND | 0);
	}
	File.Give_FP(fp);
	if (File.Read_File() == false) {
		exit(-1);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C����������ͼDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C����������ͼDemoDlg::OnPaint()
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
	pWnd = GetDlgItem(IDC_STATIC);
	pWnd->GetWindowRect(rc);
	CDC* pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1);
	//==============================================================================================
}


//===================================================================================================
void C����������ͼDemoDlg::StartDraw(CRect rc, CDC* pdc, int flag)
{
	width = rc.Width();
	height = rc.Height();
  
	CDC MemDC;                                                                 //���ȶ���һ����ʾ�豸����   
	CBitmap MemBitmap;                                                         //����һ��λͼ����
	MemDC.CreateCompatibleDC(NULL);                                            //���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                      //���潨��һ������Ļ��ʾ���ݵ�λͼ
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                         //��λͼѡ�뵽�ڴ���ʾ�豸��//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	MemDC.FillSolidRect(0, 0, width, height, RGB(255, 255, 255));              //���ñ���ɫ��λͼ����ɾ�

	//newBrush.CreateSolidBrush(RGB(255, 255, 255));                           //������ɫ�»�ˢ
	//pOldBrush = MemDC.SelectObject(&newBrush);                                 
	//MemDC.Rectangle(rc);                                                     //�Ժ�ɫ��ˢΪ��ͼ�ؼ�����ɫ���γɺ�ɫ����
	//MemDC.SelectObject(pOldBrush);                                           //�ָ��ɻ�ˢ
	//newBrush.DeleteObject();                                                 //ɾ���»�ˢ
	newPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));                             //����ʵ�Ļ��ʣ��ֶ�Ϊ1����ɫΪ��ɫ
	pOldPen = MemDC.SelectObject(&newPen);                                     //ѡ���»��ʣ������ɻ��ʵ�ָ�뱣�浽pOldPen

	//����ͼ��
	Plot(rc, &MemDC, 1);

	MemDC.SelectObject(pOldPen);                                               //�ָ��ɻ���
	newPen.DeleteObject();                                                     //ɾ���»���									  
	pdc->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);                   //���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	//��ͼ��ɺ������
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void C����������ͼDemoDlg::Plot(CRect rc, CDC* pdc, int flag)
{
	pdc->MoveTo(0, 0);
	for (int i = 0; i < width; ++i) {
		pdc->LineTo(i, (height - 150)*File.Get_Data(i + 10000) / 65535 + 75);
	}
}

//===================================================================================================



//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C����������ͼDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool Wav_File::Conversion_Tuple(void)
{
	try
	{
		dataMax = 0;
		dataMin = 1000000;
		dataNumber = dataSize * 8 / sampleBits;
		dataTuple = (long*)malloc(sizeof(long)*dataNumber);
		long tempNum = 0;
		for (int i = 0; i < dataNumber; ++i) {
			for (int j = 0; j < sampleBits / 8; ++j) {
				tempNum = MAKEWORD(data[i*sampleBits / 8 + j], tempNum);
			}
			dataTuple[i] = tempNum;
			tempNum = 0;
			if (dataMax < dataTuple[i]) {
				dataMax = dataTuple[i];
			}
			if (dataMin > dataTuple[i]) {
				dataMin = dataTuple[i];
			}
		}
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", 0);
		return false;
	}
	return true;
}

bool Wav_File::Read_File(void)
{
	try
	{
		fread(id_RIFF, sizeof(char), 4, fp);                                   //��ȡ'RIFF'
		if (strcmp(id_RIFF, "RIFF")) {
			throw invalid_argument("ERROR : File not RIFF file");
		}
		id_RIFF[4] = 0;                                                        //ĩβ����
		fread(&fileSize, sizeof(unsigned long), 1, fp);                        //��ȡ�ļ��Ĵ�С
		fread(id_WAVE, sizeof(char), 4, fp);                                   //��ȡ'WAVE'
		id_WAVE[4] = 0;
		if (strcmp(id_WAVE, "WAVE")) {
			throw invalid_argument("ERROR : File not WAVE file");
		}
		fread(id_FMT, sizeof(char), 4, fp);                                    //��ȡ'FMT'
		id_FMT[3] = 0;
		fread(&formatLength, sizeof(unsigned long), 1, fp);                    //
		fread(&formatTag, sizeof(short), 1, fp);                               //��ȡ�ļ���ǩ
		fread(&channelsNumber, sizeof(short), 1, fp);                          //��ȡͨ����Ŀ
		fread(&sampleRate, sizeof(unsigned long), 1, fp);                      //��ȡ����Ƶ��
		fread(&secondBytes, sizeof(unsigned long), 1, fp);                     //��ȡÿ��������
		fread(&chunkAlign, sizeof(short), 1, fp);                              //��ȡ�����
		fread(&sampleBits, sizeof(short), 1, fp);                              //��ȡ������С
		if (formatLength > 16) {
			fread(&appendInformation, sizeof(short), 1, fp);                   //��ȡ������Ϣ
		}
		fread(id_DATA, sizeof(char), 4, fp);                                   //��ȡ'DATA'
		id_DATA[4] = 0;
		fread(&dataSize, sizeof(unsigned long), 1, fp);                        //��ȡ���ݴ�С
		data = (char*)malloc(sizeof(char)*dataSize);                           //�������ݵĴ洢�ռ�
		fread(data, sizeof(char), dataSize, fp);                               //��ȡ����
	}
	catch (invalid_argument &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", 0);
		return false;
	}
	MessageBox(NULL, _T("TIP : File read ok !"), _T("TIP"), MB_ICONASTERISK);
	if (Conversion_Tuple() == true) {
		MessageBox(NULL, _T("TIP : Data change OK !"), _T("TIP"), MB_ICONASTERISK);
	}
	else {
		return false;
	}

	return true;
}

long Wav_File::Get_Data(unsigned long Number)
{
	if (Number >= dataNumber) {
		MessageBox(NULL, _T("ERROR : Over list"), _T("ERROR"), MB_ICONHAND);
		return -1;
	}
	else {
		return dataTuple[Number];
	}
}