/***************************************************************

���ܣ�		���ӳ������MFCC������ȡ�ĺ���
			�޸ļ�¼��	2001.7.31  ���� д����������
****************************************************************/

//MFCC��ͷ�ļ�,�������������ڴ˴�,
#include "cst_lib.h"

//�����Զ������͵Ķ���
typedef struct
	{
		float real ;
		float image ;
	} COMPLEX ;								//�����ṹ

void StartEnd(short *sData, long lDataLength, 
			  long *lStartPnt, long *lEndPnt,
			  long lSampleRate);


//��������///////////////////////////
//��ȡһ�仰������
void Comp_Feature(int OptionMode, short *sData, int FrameNumber, short &nStartFrame, short &nEndFrame);


//FFT
void	FFT(COMPLEX *x , int m);		
//FFT�ķ���							
void	FFTAm(COMPLEX *x , float *y, long m);		
//һ֡��MFCC��������ȡ
void	Comp_Mfcc(float *fOneFrameData, float &fAreaEnergy);//!!!
//��������Ƶ������
void	AreaFreqComp(float fFFTAm[MAX_FREQ_POINT], float &fAreaEnergy);






