/*******************************************************************
****	�ļ���		basop.h					****
****	���ܣ�		����LPCC��MFCC���õ���һЩ��ͬ�Ļ�������		****
**** 			                             		****
****	�޸ļ�¼��	2001.7.23 ժ��G.732.1              	****
*******************************************************************/



//	���г�ʼ��
	void	Initial(int OptionMode, float	HanmingWindow[FRAME_LEN]);
//  Ԥ���� 
	void	PreEmphasis(float *fOneFrameData);
//��������, ������, ��������
	void	EnergyCompute(short *tmpVoiceHead, float *EnergyArray, short *nZeroPassArray, short FrameNumber, float DcLevelOneSentence);
//������������, ������������֡���������Ѿ��������
	void	EnergyThreshCompte(float *EnergyArray, short FrameNumber, float &EnergyTh);

//****��������
//	���ż���
	int		sign(float x);
