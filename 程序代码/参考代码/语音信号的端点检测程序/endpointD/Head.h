//�����ļ��ĸ�ʽ��
//�ļ�����	*.dat
//�ļ���ʽ��	���������ļ�
//�ļ��ṹ������������ʽ����ÿ�仰
//    �ֽ�����0��sizeof(dataHEAD)	���ݣ�structure dataHEAD
//	______________________�ļ�ͷ______________________
/*
typedef struct 
{
	long		Version;		//�汾�ţ�����Ϊ 20001120
	char		Name[15];       	// ����
	char		Sex[4];         	// �Ա�
	char		Age[15];		// ����
	char 		HomeTown[30];  		//�������ͣ�����"�Ϻ���ͨ��"
	char		Date[24]; 		// ����
	char		Group[10];		//00 ��� 
	short		nSentenceNum;		//������
	long		SampleRate;		// ������
	char		ParameterType[10];	//�������ͣ�WAVE��
	short		SampleType;		//��������(1=char;2=short;3=long;4=float;5=double) 
	char		Reservation[76];	// ����λ
} dataHEAD;
*/
//    �ֽ�����198��199		���ݣ��ļ�����������������short ��


//****
//---------------------- �����ṹ --------------------------------------

//INDEX �� 200 �ֽڿ�ʼ�������ṹΪ��
/*
typedef struct {
	long	Index;
	long	Length;
} dataADDRESS;
*/
//����Length��ʾ�þ仰���������ֽ���




//************************
//�����ļ��ĸ�ʽΪ

//�ļ�����	*mfcc.dat  ( *lpcc.dat)
//�ļ���ʽ��	���������ļ�
//�ļ��ṹ������������ʽ����ÿ�仰
//    �ֽ�����0��sizeof(dataHEAD)	���ݣ�structure dataHEAD
//	______________________�ļ�ͷ______________________


typedef struct 
{
	long		Version;		//�汾�ţ�����Ϊ 20001120
	char		Name[15];       	// ����
	char		Sex[4];         	// �Ա�
	char		Age[15];		// ����
	char 		HomeTown[30];  		//�������ͣ�����"�Ϻ���ͨ��"
	char		Date[24]; 		// ����
	char		Group[10];		//00 ��� 
	short		nSentenceNum;		//������
	long		SampleRate;		// ������
	char		ParameterType[10];	//�������ͣ�MFCC/LPCC��
	short		SampleType;		//��������(1=char;2=short;3=long;4=float;5=double) 
	short		Feature_Order;		//����ά��
	char		Reservation[74];	// ����λ
} dataHEAD;

//    �ֽ�����198��199		���ݣ��ļ�����������������short ��


//---------------------- �����ṹ --------------------------------------

//INDEX �� 200 �ֽڿ�ʼ�������ṹΪ��

typedef struct {
	long	Index;
	long	FrameAmount;
} dataADDRESS;
//����FrameAmount��ʾ�þ仰��������֡��




