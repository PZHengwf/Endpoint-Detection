#include <math.h>

#include "cst_lib.h"
#include "basop.h"
#include "EndPoint_xhg.h"



/*=====================================================
�������ƣ�	EndPointDetection
�������ܣ�	�˵���
���������	EnergyArray		Frame_Num�������飬�洢ÿһ��Frame������
			fRatioFreq		�洢ÿһ��Frame�ĵ�Ƶ�����������
			nZeroPassArray	�洢ÿһ��Frame�Ĺ��������
			Frame_Num		�ܵ�֡��
			Energy_Th		������������
���������	nStartFrame		���ؼ�����ʼ�˵�
			nEndFrame		������ֹ�˵㡣

��ע��		
����˼·��	���ݹ۲�����, Ԫ���εĹ�����һ�㶼ҪС��30, �������Ĺ�����һ�㶼ҪС��100
			����������������Ƚϵ��Ƚ�, һ����˵, �������
			//	���ڶ��ڹ�����, ��ͬ�ľ���, ������һЩ�Ƚ��쳣�����, 
			//  ��Ӿ��ӵĿ�ʼ��������, �����Ǿ��ӵ���β�Ƚϳ�, ֱ������ĩβ
			//	������ǰ��֡��������Ϊ�˵�Ͳ�̫�ȶ�, ���úܶ�仰����Ӧ����������
			//	����������ֻ��ı�����̫һ��, ��ô��������������Ҳ�ǲ��ɿ���
			//  ���Ը��ݵ�Ƶ�������뷨, ���ܵ�����Ҳ�������ֵ�Ĺ�һ��, ��ʵʵ�鷢������Ҳ����, 

====================================================*/
void	EndPointDetection(float *EnergyArray, short *nZeroPassArray, float *fRatioFreq, short Frame_Num, short &nStartFrame, short &nEndFrame, float Energy_Th)
{
	short	nCrudeStart, nCrudeEnd;				//���Ե������յ�
	short	Wait_Duration = 0;					//ͳ�Ƽ����ñ���
	short	Wait_Duration1= 0;					//ͳ�Ƽ����ñ���
	short	wFrameCount;						//ѭ����������
	float	EnergyThreshUsed;					//���Ե��ڵ�����

	/**********************************************************
	<------------------����
	**********************************************************/
	EnergyThreshUsed =Energy_Th + 1.3f ; //����Energy_Th�Ƕ�������, �����������, �൱��Energy_Th * 27
	
	//ȷ��Ԫ����, �ӵ�0֡��ʼ
	for(wFrameCount=0; wFrameCount<Frame_Num; wFrameCount++)
	{
		//ͬʱ�ù�����, ����Ƶ������, �ܵ�������Ϊ�ж�����
		//�������ҵ�Ԫ����, �����Ժ��������Ļ�, �϶�����Ԫ���ε�, 
		if( (fRatioFreq[wFrameCount]>-4) && (nZeroPassArray[wFrameCount]<70) && (EnergyArray[wFrameCount]>EnergyThreshUsed))
		{
			Wait_Duration++;
			//���������3֡��������, ����Ϊ�ҵ���ʼ��
			if (Wait_Duration > 3)
				break;
		}
		else
			Wait_Duration = 0;
	}
	nCrudeStart = wFrameCount - 3;

	//���������������û���ҵ���ʼ֡nCrudeStart, ���˵�������Ѿ��������һ֡,��ſ�����
	if(wFrameCount == Frame_Num)  
	{
		Wait_Duration = 0;
		for (wFrameCount=0; wFrameCount<Frame_Num; wFrameCount++)
		{
			//��ʱ������������Ϊ�ж�����
			//�൱�����������0.135
			if (EnergyArray[wFrameCount] > (EnergyThreshUsed + 1 ))
			{
				Wait_Duration++;
				//���������4֡��������, ����Ϊ�ҵ���ʼ��
				if (Wait_Duration> 4)
					break;
			}
			else
				Wait_Duration = 0;
		}
		nCrudeStart = wFrameCount - 4;
	}

	//��������β��, ������4֡��ʼ�ҽ���֡nCrudeEnd
	Wait_Duration = 0;
	for (wFrameCount=Frame_Num-4; wFrameCount>nCrudeStart; wFrameCount--)
	{
		//ͬʱ�õ�Ƶ����, ���ܵ�������Ϊ����, ��������Ϊ������ѰԪ����, ��������<40
		if ((fRatioFreq[wFrameCount]>-4) && (EnergyArray[wFrameCount]>EnergyThreshUsed) && (nZeroPassArray[wFrameCount]<70))
		{
			Wait_Duration++;
			if (Wait_Duration> 2)
				break;
		}
		else
			Wait_Duration = 0;
	}
	nCrudeEnd = wFrameCount + 2;

	//���������������û���ҵ�����֡nCrudeEnd, ��ſ�����
	if (wFrameCount == nCrudeStart)  
	{
		Wait_Duration = 0;
		for (wFrameCount=Frame_Num-1; wFrameCount>nCrudeStart; wFrameCount--)
		{
			//������������Ϊ��׼���ж�
			if (EnergyArray[wFrameCount] > (EnergyThreshUsed + 1))
			{
				Wait_Duration++;
				if (Wait_Duration > 2)
					break;
			}
			else
				Wait_Duration = 0;
		}
		nCrudeEnd = wFrameCount + 2;
	}

	//�����ʼ֡�ͽ���֡�ľ���̫С, ����ʼ֡��Ϊ4, ����֡��Ϊ���һ֡, ������һ��
	if (nCrudeEnd - nCrudeStart < 2)
	{
		nCrudeStart = 4;
		nCrudeEnd = Frame_Num - 1;
	}
/***********************************************************
			ϸ��
***********************************************************/
	//Ѱ�ҽ���֡, �Ӵ��еĽ���֡��ʼ����Ҿ�����, ��Ϊ��
	//������ʶ����, ���Ǳ�Ȼ����һ������֡��, ����Ӧ�ô���һ���ľ�����
	//ͨ�����ݷ���, ��������Ϊһ������, ���ҽ���֡ʱ, ���ȶ�, ������
	//EnergyThreshUsed = Energy_Th * 5
	EnergyThreshUsed = Energy_Th + 1.3f;
	Wait_Duration = 0;
	for (wFrameCount = nCrudeEnd; wFrameCount < Frame_Num; wFrameCount++)
	{
		//�жϹ�����, ���������>100, ����Ϊ������, �����Ǿ���
		if((nZeroPassArray[wFrameCount] < 100) && ((EnergyArray[wFrameCount]<EnergyThreshUsed + 0.5) || (fRatioFreq[wFrameCount]<-4.5)))
		{
			if(EnergyArray[wFrameCount] < (EnergyThreshUsed))	//Energy_Th * 6
				Wait_Duration++;
			else
				Wait_Duration = 0;
			if(fRatioFreq[wFrameCount] < -5.1)
				Wait_Duration1++;
			else
				Wait_Duration1 = 0;
			if ((Wait_Duration > 2) || (Wait_Duration1 >2))
				break;
		}
		else
		{
			Wait_Duration = 0;
			Wait_Duration1 = 0;
		}
	}
	nEndFrame = wFrameCount - 2;

	//Ѱ����ʼ֡, �Ӵ��е���ʼ֡��ǰ��
	Wait_Duration = 0;
	Wait_Duration1 = 0;
	for (wFrameCount = nCrudeStart; wFrameCount >= 0; wFrameCount--)
	{
		if((nZeroPassArray[wFrameCount] < 100) && ((EnergyArray[wFrameCount]<EnergyThreshUsed + 0.5) || (fRatioFreq[wFrameCount]<-4.5)))
		{
			if(EnergyArray[wFrameCount] < (EnergyThreshUsed))	//Energy_Th * 6
				Wait_Duration++;
			else
				Wait_Duration = 0;
			if(fRatioFreq[wFrameCount] < -5.0)
				Wait_Duration1++;
			else
				Wait_Duration1 = 0;
			if ((Wait_Duration > 2) || (Wait_Duration1 >2))
				break;
		}
		else
		{
			Wait_Duration = 0;
			Wait_Duration1 = 0;
		}
	}
	nStartFrame = wFrameCount + 2;

	if (nStartFrame < 4)              
		nStartFrame = 4;
	if (nEndFrame > Frame_Num - 4)
		nEndFrame = Frame_Num - 4;

	//���������̫��, ��˵���ʧ��, ����������������һ��
	if ((nEndFrame - nStartFrame) < 5)        
	{
		nEndFrame = Frame_Num - 4;
		nStartFrame = 4;
	}
}

