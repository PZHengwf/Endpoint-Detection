/*******************************************************************
****	�ļ���		mfcc_xhg.cpp								****
****	���ܣ�		����MFCC���õ���һЩ��������				****
**** 			 �������										****
****	�����01.08.20, ����		          				****
//MFCC������ȡʵ�ֵ�����������������Ҫ��˵������
//MFCC_xhg.h��, ������ֻ��Ҫ����MFCC_xhg.h����
*******************************************************************/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "cst_lib.h"
#include "mfcc_xhg.h"
#include "basop.h"
#include "endPoint_xhg.h"
#include "16to8.h"

extern FSDEF fs;

/*=====================================================
�������ƣ�	StartEnd
�������ܣ�	��һ�仰���������ݽ��ж˵��� 
�������ʣ�	
���������	*sData һ�仰���׵�ַ
			lDataLength	һ�仰�ĳ��ȣ����������
			*lStartPnt	������ʼ����
			*lEndPnt	���ؽ�������
			lSampleRate	������
���������	
��ע��		
����˼·��
====================================================*/
void StartEnd(short *sData, long lDataLength, 
			  long *lStartPnt, long *lEndPnt,
			  long lSampleRate)
{
	long	FrameTotalNumber;//һ�仰�е�֡������
	int	lDataLength_8000, i;
	short	*sdata_8000=NULL;
	short	nStartFrame, nEndFrame;
	int OptionMode=0;
	    
    
		lDataLength_8000=lDataLength;
		sdata_8000=sData;
	FrameTotalNumber=(long)((sizeof(short)*lDataLength_8000- (sizeof(short)*FRAME_LEN))/(sizeof(short)*FRAME_STEP)) + 1;

	OptionMode = OptionMode | MFCC | PREEMPHASIS  ;
	Comp_Feature(OptionMode, sdata_8000, FrameTotalNumber, nStartFrame, nEndFrame);
//-------------------------------------------------------------	
	for(i = 0; i<lDataLength-1; i++)      //added by xiaogang 2003.9.2
	{
        if ( (sdata_8000[i+1]-sdata_8000[i]) == 1)
		{
			(*lStartPnt) = i/2;
		    break;
		}
        (*lStartPnt) = 0;
	}
	for(i=0 ; i<lDataLength-1; i++)
	{
        if ( (sdata_8000[i]-sdata_8000[i+1]) == 1)
		{
			(*lEndPnt) = i/2;
		    break;
		}
		(*lEndPnt) = 0;
	}
   
	if(((*lStartPnt)>=(*lEndPnt)) || (((*lEndPnt)-(*lStartPnt)) < 5)  )
	{
	     (*lStartPnt)=0;
         (*lEndPnt  )=0;
         //getchar();
	}

//--------------------------------------------------------------	

//	(*lStartPnt)=(long)((nStartFrame-2)*FRAME_STEP);
//	(*lEndPnt)=(long)((nEndFrame+6)*FRAME_STEP);

	if( ((*lStartPnt)>lDataLength_8000) || ((*lStartPnt)<0) )
		(*lStartPnt)=0;

	if( ((*lEndPnt)>lDataLength_8000) || ((*lEndPnt)<0) )
		(*lEndPnt)=lDataLength_8000;
/*   ���8k seg�ļ�ʱ����          
	if(lSampleRate==16)
	{
		(*lStartPnt)=(*lStartPnt)*2;
		(*lEndPnt)=(*lEndPnt)*2;
		sdata_8000=NULL;
	}
*/
}

/*=====================================================
�������ƣ�	Comp_Feature
�������ܣ�	��һ�仰���������ݽ��д���, ������ǵ�����, ��������(��ѡ�������, ��ֵ���, 
			����, �������, ���ײ������, ���жϳ��˵�, ��ʼ֡�� & ����֡��
�������ʣ�	
���������	OptionMode		ģʽ������MFCC or LPCC, CMS, ...
			sData			������������ͷ�������������������������ָ��
			Frame_Number	һ�仰��, �ܵ�����֡��
���������	FeatureArray	��Ӧ�ڸ�֡����������ʸ�����ѷ������ڴ�
			nStartFrame		��ʼ֡
			nEndFrame		����֡
��ע��		
����˼·��
====================================================*/
void Comp_Feature(int OptionMode, short *sData, int FrameNumber, short &nStartFrame, short &nEndFrame)
{
	short	wSampleCount, wFrameCount;			//�����֡��ѭ����������
	short	*tmpVoiceHead;						//����ָ��ÿһ������֡��ͷ, ��ַ�ǻ�ı��, ���������ǲ����
	float	fOneFrameData[FRAME_LEN];			//һ֡��������, �������ǻ��ڼ�������иı�

	static	int		first =1;					//��־λ, hamming��, cepweightwindow, ���Ǵ�
	static	float	HanmingWindow[FRAME_LEN];	//hanming������

	//���ڶ˵���
	float	*EnergyArray		= NULL;			//��������ÿһ֡������
	short	*nZeroPassArray		= NULL;			//��������ÿһ֡�Ĺ�����	
	float	*fRatioFreq			= NULL;			//��������ÿһ֡�ĵ�Ƶ����
	float	DcLevelOneSentence	= 0;
	static  float	EnergyTh;					//���ڶ˵������������

	//��̬��������������ڴ�
	EnergyArray		= new float [FrameNumber];
	//��̬��������ʵ��ڴ�
	nZeroPassArray	= new short [FrameNumber];
	//��̬�����Ƶ�������ڴ�
	fRatioFreq		= new float [FrameNumber];

	//��ȡ�������ݴ洢��ַ
	tmpVoiceHead = sData;
	
	//�ж�����ǵ�һ��, ���ʼ��hanming��, ���׼�Ȩ��, (MFCC)Mel���Ǵ�
	if(first)
	{
		first = 0;
		Initial(OptionMode, HanmingWindow);
	}

	//---------------->
	//��һ�仰�е�ÿһ֡����ѭ��, ����䵹������
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
	{
	
		//���������ݻ�����ȡ��һ֡���������ݽ��м���
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		{
			fOneFrameData[wSampleCount] = tmpVoiceHead[wSampleCount];
			DcLevelOneSentence += fOneFrameData[wSampleCount];
		}

		//2.Ԥ����		y[n] = x[n] - preemphasis_factor * x[n-1]
		if( OptionMode & PREEMPHASIS)
			PreEmphasis(fOneFrameData);

		//4.��hanming��											n
		//		HanmingWindow[n] = 0.54 - 0.46 * cos( 2 * PI *------);	N:֡��
		//													  N - 1
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
			fOneFrameData[wSampleCount] = fOneFrameData[wSampleCount] * HanmingWindow[wSampleCount];

		//5.����һ֡��MFCC����, ������Ƶ����
		Comp_Mfcc(fOneFrameData, fRatioFreq[wFrameCount]);

		//7.����ѭ���Ĵ���
		//��tmpVoiceHeadָ����һ֡��������ʼ��
		tmpVoiceHead	+= FRAME_STEP;

	}//<---------------------
	//�˴�һ�仰������ѭ������, ������һ�仰�ĵ���

	//�˴�����һ�仰����������
	tmpVoiceHead	= sData;

	DcLevelOneSentence /= (FrameNumber * FRAME_LEN);

	//�������������,  ������
	EnergyCompute(tmpVoiceHead, EnergyArray, nZeroPassArray, FrameNumber, DcLevelOneSentence);

	//�������ڶ˵������������
	EnergyThreshCompte(EnergyArray, FrameNumber, EnergyTh);

/*--------------------------------------->
**			�Ժ���Ϊ�˶˵���		***
*****************************************/
	//�Ե�Ƶ�������й�һ��, �����еĵ�Ƶ������ȥ�����ֵ
	//�ҳ����ֵ, �����ܵ������Ķ�̬��Χ̫��, �ʲ������ֹ�һ���ķ��������Ǻܺ��ʵ�.
	double	tmpRatio = -1e50;
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
		if(tmpRatio < fRatioFreq[wFrameCount])
			tmpRatio = fRatioFreq[wFrameCount];

	//��һ��
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
		fRatioFreq[wFrameCount] -= (float)tmpRatio;

	//�˵���
	//������Ҫ����Ĳ�����: ��������, ÿһ֡������ & ������ & ��Ƶ����,
	EndPointDetection(EnergyArray, nZeroPassArray, fRatioFreq, FrameNumber, nStartFrame, nEndFrame, EnergyTh);
	//ɾ���ڴ�ķ���
	if(EnergyArray!=NULL)
	{
		delete EnergyArray;
	    EnergyArray=NULL;
	}
    if(nZeroPassArray!=NULL)
	{
		delete nZeroPassArray;
	    nZeroPassArray=NULL;
	}
	if(fRatioFreq!= NULL)
	{
		delete fRatioFreq;
		fRatioFreq = NULL;
	}

}

/*=====================================================
�������ƣ�	Comp_Mfcc
�������ܣ�	��ȡ��֡������CEP_NUMάMFCCϵ��ͬʱ��������ʸ����ǰCEP_NUMά, 
			��ĵ�Ƶ����
���������	fOneFrameData				��֡������������ͷ�������������������������ָ��
���������	fAreaEnergy					��ƵƵ�������, Ƶ�ʶο�����cst_lib.h������
��ע��		
����˼·��
====================================================*/
void	Comp_Mfcc(float *fOneFrameData,	float &fAreaEnergy)//!!!
{
	short	wFftCount;					//������Ϊѭ���еļ�������
	float	fFFTAm[MAX_FREQ_POINT];		//����FFTƵ�׵ķ���	
	COMPLEX cFFTData[FFT_NUM];			//�������渴������������

	//������FFT��cFFTData ���г�ʼ��
	for(wFftCount=0; wFftCount<FFT_NUM; wFftCount++)
	{
		cFFTData[wFftCount].real = 0.0;
		cFFTData[wFftCount].image = 0.0;
	}
	for(wFftCount=0; wFftCount<FRAME_LEN; wFftCount++)
		cFFTData[wFftCount].real = fOneFrameData[wFftCount];

	//1.����FFT, ��������źŵ�Ƶ��
	FFT(cFFTData, FFT_ORDER);

	//2.��FFT�ļ�����, ����źŵķ�����
	//			FFTAm(x, y) = x^2 + y^2
	FFTAm(cFFTData, fFFTAm, FFT_ORDER);

	//�����100hz-400hzƵ�������, ��Ϊ�����ڸ�Ƶ��������Ƚϸ�,
	AreaFreqComp(fFFTAm, fAreaEnergy);
}


/*=====================================================
�������ƣ�FFT
�������ܣ�FFT�任����2DIT
�������: x ����������,
		  m FFT�ļ���
�����������,ԭַ����
====================================================*/
void FFT(COMPLEX *input, int x)
{
	int n , i , nv2 , j , k , le , l , le1 , ip , nm1 ;
	COMPLEX t , u , w ;

	n = 1;
	for(i=0; i<x; i++)
		n = n*2 ;

	nv2 = n / 2 ;
	nm1 = n - 1 ;
  	j = 1 ;

	for (i = 1 ; i <= nm1 ; i ++)
	{
		if (i < j)
		{
			t.real = input[i - 1].real ;
			t.image = input[i - 1].image ;
			input[i - 1].real = input[j - 1].real ;
			input[i - 1].image = input[j - 1].image ;
			input[j - 1].real = t.real ;
			input[j - 1].image = t.image ;
		}

		k = nv2 ;

		while (k < j)
		{
			j -= k ;
			k /= 2 ;
		}
		j += k ;
	}

	le = 1 ;
	for (l= 1 ; l <= x ; l ++)
	{
		le *= 2 ;
   		le1 = le / 2 ;
		u.real = 1.0f ;
		u.image = 0.0f ;
		w.real = (float) cos(PI / le1) ;
		w.image =(float) -sin(PI / le1) ;

		for (j = 1 ; j <= le1 ; j ++)
		{
			for (i = j ; i <= n ; i += le)
			{
				ip = i + le1 ;
				t.real = input[ip - 1].real * u.real - input[ip - 1].image * u.image ;
				t.image = input[ip - 1].real * u.image + input[ip - 1].image * u.real ;
				input[ip - 1].real = input[i - 1].real - t.real ;
				input[ip - 1].image = input[i - 1].image - t.image ;
				input[i - 1].real = t.real + input[i - 1].real ;
				input[i - 1].image = t.image + input[i - 1].image ;
			}

			t.real = u.real * w.real - u.image * w.image ;
			t.image = u.image * w.real + u.real * w.image ;
			u.real = t.real ;
      		u.image = t.image ;
		}
	}
}

/*=====================================================
�������ƣ�	FFTAm
�������ܣ�	��ȡFFT�任��ķ��� s(i) = x(i)^ 2 + y(i)^ 2
���������	x ԭʼFFT������ ,
			y ����Ĵ洢FFT���ȵ����У����ڶԳ�����ֻ����һ��,
			m FFT�ļ��� 
���ز�����	�� 
====================================================*/
void FFTAm(COMPLEX *x , float *y, long m)
{
	long n,i;
	n = 1 ;
	for (i = 0 ; i < m ; i ++)
		n *= 2 ;
	for (i = 0 ; i < n/2 ; i ++)
      y[i] =(float) (x[i].real * x[i].real + x[i].image * x[i].image) ;
}

/*=====================================================
�������ƣ�	AreaFreqComp
�������ܣ�	��������Ƶ�������								
���������	fFFTAm[MAX_FREQ_POINT]		FFT�������� 	
���������	fAreaEnergy					����Ƶ�ʵ�����										
====================================================*/
void	AreaFreqComp(float fFFTAm[MAX_FREQ_POINT], float &fAreaEnergy)
{
	short wFFTCount;
	
	//�������Ƶ�����Ӧ��Ƶ��
	static short	lowFreq = (short) (LOW_FREQ_AREA / SAMPLE_FREQUENCY * FFT_NUM);
	static short	highFreq = (short) (HIGH_FREQ_AREA / SAMPLE_FREQUENCY * FFT_NUM);

	fAreaEnergy = 0;
	for(wFFTCount=lowFreq; wFFTCount<highFreq; wFFTCount++)
		fAreaEnergy += fFFTAm[wFFTCount];
	fAreaEnergy = (float)log(fAreaEnergy);
}
