//***�궨��

//һЩ�����Ķ���
#define		PI					3.14159265358979f
#define		M_PI				PI
#define		TPI					6.28318530717959     /* PI*2 */

//һЩ������ԭʼ�����йصĶ���
#define		SAMPLE_FREQUENCY	8000.0					//������HZ   
#define		FRAME_LEN			256						//ÿ֡�Ĳ�������
#define		OVERLAP_NUM			FRAME_LEN/2				//֡���ص��Ĳ�������		
#define		FRAME_STEP			(FRAME_LEN-OVERLAP_NUM)	//֡�Ƶĵ���
#define     LPCCFIX_FRAME		192
//һЩ������Ԥ�����йصĶ���
#define		PRE_EMPHASIS_FACTOR	0.98f	//Ԥ����ϵ��
#define		ZEROPASS_NOISE_FACT	1.5f	//��������������Ӧ
#define		ADAPTIVE_FACTOR		0.8f	//��������Ӧ�ļ�Ȩϵ��
//�˵���
#define		LOW_FREQ_AREA		100						//����Ƶ������ʱȡ�ĵ�Ƶ
#define		HIGH_FREQ_AREA		600					//����Ƶ������ʱȡ�ĸ�Ƶ

//һЩ�������йصĶ���
/****************************
MFCC���õĶ���
****************************/
#define		FFT_ORDER			8						//FFT�任����
#define		FFT_NUM				256						//FFT�任����
#define		MAX_FREQ_POINT		(FFT_NUM / 2)

//����ģʽ�Ķ���
#define		MFCC				0x0001				//ʹ��MFCC����
#define		LPCC				0x0002				//ʹ��LPCC����

#define		VAD					0x0004              //VAD �о�  
#define		CMS					0x0008              //���׾�ֵ��
#define		PREEMPHASIS			0x0010
#define		HIGHPASS			0x0020				//�Ƿ�ʹ�ø�ͨ
#define		DELTFEATURE			0x0040				//ʹ��һ�ײ������
#define		ENERGY				0x0080
#define		DELTENERGY			0x0100				//ʹ��һ�ײ������
#define		DDELTENERGY			0x0200				//ʹ�ö��ײ������
