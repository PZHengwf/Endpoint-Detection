/*******************************************************************
****	�ļ���		basop.cpp									****
****	���ܣ�		����LPCC �� MFCC���õ���һЩ��ͬ�Ļ�������	****
**** 			 �������										****
****	�����01.08.20, ����		          				****
*******************************************************************/

#include  <math.h>

#include "cst_lib.h"
#include "basop.h"
#include "mfcc_xhg.h"

/*=====================================================
�������ƣ�	Initial
�������ܣ�	��Hanming��ϵ��
�������ʣ�	
���������	OptionMode	����ģʽѡ��, LPCC������MFCC, ��������
���������	HanmingWindow[FRAME_LEN]	//hanming������
��ע:		ÿ�γ�������, ֻ��Ҫһ�γ�ʼ���Ϳ�����,
			������ʹ�õ��ĳ����� 
			FRAME_LEN			֡�� 
====================================================*/
void	Initial(int OptionMode, float	HanmingWindow[FRAME_LEN])
{
	int	wSampleCount;		//ѭ���ñ���, ����, ����
	float fDataTemp;					//�м����
	
	//��ʼ��hanming������
	//HANMING 0.54 - 0.46 * COS( 2 * n * PI / ( N - 1) ) (NΪ֡��)
	fDataTemp = (float) 2.0 * PI / (FRAME_LEN - 1);
	for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		HanmingWindow[wSampleCount] = (float) (0.54 - 0.46 * cos( wSampleCount * fDataTemp));

}

/*=====================================================
�������ƣ�	PreEmphasis
�������ܣ�	��ԭʼ��������Ԥ����
���������	fOneFrameData ��֡���������ڴ�ͷ 
�������:	��
��ע��		ÿ֡��ǰһ����ȡ��1�����ֵ����  S'(n)=S(n)-0.98*S(n-1)
			�õ��ĳ���������
			FRAME_LEN			֡��
			PRE_EMPHASIS_FACTOR	Ԥ����ϵ��
====================================================*/
void PreEmphasis(float *fOneFrameData)
{
	
    int   i;
    float acc0[FRAME_LEN];	
    
   	acc0[0] = fOneFrameData[0] - PRE_EMPHASIS_FACTOR * fOneFrameData[0];
	for (i=1; i < FRAME_LEN; i++)
       	acc0[i] = fOneFrameData[i] -PRE_EMPHASIS_FACTOR * fOneFrameData[i-1];
        
	for (i=0; i < FRAME_LEN; i++)
   	 	fOneFrameData[i] = acc0[i];
}

/*=====================================================
�������ƣ�	sign
��������:	���ż���
�������:	x	��Ҫ������ŵ�����
�������:	�����ݵķ���λ, x>0, ����Ϊ1, x=<0, ����λΪ-1
====================================================*/
int sign(float x)
{
  if(x > 0)
    return 1;
  else
    return -1;
}

/*=====================================================
�������ƣ�	EnergyThreshCompte
��������:	�������޼���
�������:	EnergyArray	һ�仰����������
			FrameNumber һ�仰������֡��
�������:	EnergyTh	һ�仰����������
====================================================*/
void	EnergyThreshCompte(float *EnergyArray, short FrameNumber, float &EnergyTh)
{
	float EnergyOneSentence	= 0;
	float EnergyFront		= 0;
	float EnergyEnd			= 0;

	static short first = 1;
//	short first = 1;
	
	//ǰ2֡����������ƽ��
	//�������2֡����������̫��, ��ȡ��Сֵ
	if(fabs(EnergyArray[0] - EnergyArray[1]) < 1.0 )
		EnergyFront = (EnergyArray[0] + EnergyArray[1]) / 2;
	else
	{
		if(EnergyArray[0] < EnergyArray[1])
			EnergyFront = EnergyArray[0];
		else
			EnergyFront = EnergyArray[1];
	}
	
	//��2֡����������ƽ��
	if(fabs(EnergyArray[FrameNumber-2] - EnergyArray[FrameNumber-1]) < 1.0)
		EnergyEnd = (EnergyArray[FrameNumber-2] + EnergyArray[FrameNumber-1]) / 2 ;
	else
	{
		if(EnergyArray[FrameNumber-2] < EnergyArray[FrameNumber-1])
			EnergyEnd = EnergyArray[FrameNumber-2];
		else
			EnergyEnd = EnergyArray[FrameNumber-1];
	}

	//ǰ��������ͺ������������ƽ��
	if(fabs(EnergyFront - EnergyEnd) < 2.0)
		EnergyOneSentence = (EnergyFront + EnergyEnd) / 2;
	else
	{
		if(EnergyFront < EnergyEnd)
			EnergyOneSentence = EnergyFront;
		else
			EnergyOneSentence = EnergyEnd;
	}

	if(first == 1)
	{
		EnergyTh = EnergyOneSentence ;
		first = 0;
	}
	else
		EnergyTh =(float)( ADAPTIVE_FACTOR * EnergyTh + (1-ADAPTIVE_FACTOR) * EnergyOneSentence);

}

/*=====================================================
�������ƣ�	EnergyCompute
��������:	����һ�仰����������, ������
�������:	tmpVoiceHead			һ�仰��ԭʼ����
			FrameNumber				һ�仰�е�����֡��
			DcLevelOneSentence		һ�仰��ֱ����ƽ
���������	EnergyArray				��֡����������
			nZeroPassArray			�þ仰�Ĺ�����
˵��:		
			����:			FRAME_LEN
=====================================================*/
void	EnergyCompute(short *tmpVoiceHead, float *EnergyArray, short *nZeroPassArray, short FrameNumber, float DcLevelOneSentence)
{
	short wFrameCount, wSampleCount;
	float fOneFrameData[FRAME_LEN];
	
	float tmpNoise, fTmp, NoiseLevelOneSentence;
	float NoiseFront	= 0;
	float NoiseEnd		= 0;
	float tmpPointLevel[FRAME_LEN];
	float *pNoise;
	static short first = 1;
	static float NoiseLevelAll;


	//��������
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
	{
		EnergyArray[wFrameCount] = 0.0 ;
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		{
			fOneFrameData[wSampleCount] = tmpVoiceHead[wSampleCount] - DcLevelOneSentence;
			EnergyArray[wFrameCount] += fOneFrameData[wSampleCount] * fOneFrameData[wSampleCount];
		}
	
		//������ȡ����
		if(EnergyArray[wFrameCount] > 1)
			EnergyArray[wFrameCount] = (float)log(EnergyArray[wFrameCount]);
		else
			EnergyArray[wFrameCount] = 0;
	
		/*------------------------------------------------------->*/
		//				�����ʵļ���
		//��������ƽ��������Ӧ
		if((wFrameCount ==0 ) || (wFrameCount==1) || (wFrameCount==FrameNumber-2) || (wFrameCount==FrameNumber-1))
		{
			//�����е��ƽ�ľ���ֵȡ��
			tmpNoise = 0;
			for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
				tmpNoise += (float)fabs(fOneFrameData[wSampleCount]);
			tmpNoise /= FRAME_LEN;
			if((wFrameCount == 0) || (wFrameCount == FrameNumber -2))
				NoiseLevelOneSentence = tmpNoise;
			else
			{
				//һ�仰��������ƽ�������������ǲ���ǰ��֡��ƽ��
				if(wFrameCount == 1)
					pNoise = &NoiseFront;
				else
					pNoise = &NoiseEnd;
				fTmp = tmpNoise / NoiseLevelOneSentence;
				if((fTmp<2) && (fTmp>0.5))
					*pNoise = (NoiseLevelOneSentence + tmpNoise) / 2;
				else if(fTmp > 2)
					*pNoise = NoiseLevelOneSentence;
				else
					*pNoise = tmpNoise;
				if(wFrameCount == 1)
				{
					if(first == 1)
						NoiseLevelAll = *pNoise;
				}
				else
				{
					fTmp = NoiseFront / NoiseEnd;
					if((fTmp<2) && (fTmp>0.5))
						NoiseLevelOneSentence = (NoiseFront + NoiseEnd) / 2;
					else if(fTmp > 2)
						NoiseLevelOneSentence = NoiseEnd;
					else
						NoiseLevelOneSentence = NoiseFront;
					//����ǵ�1�仰, ��ֱ�Ӳ�����仰��������ƽ����������
					if(first == 1)
					{
						NoiseLevelAll = NoiseLevelOneSentence;
						first = 0;
					}
					//�������, �������ǰ��������ƽ�����������ļ�Ȩֵ��Ϊ������ƽ����������
					else
						NoiseLevelAll =(float)( ADAPTIVE_FACTOR * NoiseLevelAll + (1-ADAPTIVE_FACTOR) * NoiseLevelOneSentence);
				}
			}
		}	
		
		//����������˲�, ����źŵĵ�ƽС��������ƽ, ���źŵķ���ȡΪ0
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		{
			// ����źŴ���0, ���ȥ���������޵�ƽ, �����С��0, ����Ϊ������, ����ȡ0,
			if(sign(fOneFrameData[wSampleCount]) == 1)
			{
				tmpPointLevel[wSampleCount] = fOneFrameData[wSampleCount] - ZEROPASS_NOISE_FACT * NoiseLevelAll;
				if(sign(tmpPointLevel[wSampleCount]) == -1)
					tmpPointLevel[wSampleCount] = 0;
			}
			//����ź�С��0, ��������������޵�ƽ, ���������0, ����Ϊ������, ����ȡ0,
			else
			{
				tmpPointLevel[wSampleCount] = fOneFrameData[wSampleCount] + ZEROPASS_NOISE_FACT * NoiseLevelAll;
				if(sign(tmpPointLevel[wSampleCount]) == 1)
					tmpPointLevel[wSampleCount] = 0;
			}
		}

		nZeroPassArray[wFrameCount] = 0;
		for(wSampleCount=0; wSampleCount<FRAME_LEN-1; wSampleCount++)
			nZeroPassArray[wFrameCount] += (int) fabs(float(sign(tmpPointLevel[wSampleCount +1]) - sign(tmpPointLevel[wSampleCount])));
		nZeroPassArray[wFrameCount] /= 2;

		//tmpVoiceHeadָ����һ֡����ʼ��
		tmpVoiceHead	+= FRAME_STEP;
	}
}
