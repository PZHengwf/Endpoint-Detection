#pragma once
#include "Wav_File_Header.h"
#include <cmath>
class Wav_File_Work :protected Wav_File
{
private:

	double *dataDouble;                                                      //��ת����Double�����ݣ����Ʒ�Χ��[-1,1]

	double *dataDFT;                                                         //���ڱ���������ɢ����Ҷ�任������
	double *dataIDFT;                                                        //���ڱ���������ɢ����Ҷ�任������

	double *New_dataDouble;													 //���ڱ�����ٸ���Ҷ�任֮ǰ���������
	double *dataFFT;														 //���ڱ�����ٸ���Ҷ�任������
	double *Em;															     //��������Ƶ������
	double *SCm;														     //��������Ƶ������
	double *BWm;															 //���ڱ�����
	double **SERmi;															 //���ڱ����Ӵ�������
	double *Nor_dataDouble;													 //���ڱ����һ������
	double **SFmi;															 //���ڱ����Ӵ�������
	double *RPm;															 //���ڱ���Ƶ�׹�����
	double **Ai;															 //����Ԥ��ϵ��
	double *err;															 //����
	double *Xn;																 //���ڱ����ʱ����Ҷ�任������
	double *H;																 //����Ԥ����
	double **h;																 //����Ԥ�⵹��ϵ��
	double *h_Spectrum;										                 //����Ԥ�⵹��


	bool Conversion_Double(void);                                            //���������ֽ�����ת��ΪDouble����
	double Hamming_window(double data);                                      //����������
	bool DFT(void);                                                          //������ɢ����Ҷ�任�����鲿

	int Is_Pow2(void);														 //�ж�FFT�����Ƿ�Ϊ2�������η�
	long Add_Zero(int i);													 //����
	void Reverse_dataDouble(double *data, int len);						     //������������
	int FFT(void);															 //���ٸ���Ҷ�任

	int spectralEnergy(int len);											 //����Ƶ������
	int spectralCentroid(int len);										     //Ƶ������
	int bandWidth(int len);													 //���
	int Subband_Energy_Ratio(int len);										 //�Ӵ�������
	void Normalized_Spectral_Signal(int k);								     //��һ��Ƶ���ź�
	int Subband_Spectral_Flux(int len);									     //�Ӵ�������
	int Spectral_Rolloff_Point(int len);								     //Ƶ�׹�����
	int LPC_Levinson_Durbin(void);											 //����Ԥ��ϵ��
	int STFT(void);															 //��ʱ����Ҷ�任
	int LPC(void);															 //����Ԥ����
	int LPCC_Coefficien(void);												 //����Ԥ�⵹��ϵ��
	int LPCC(void);                                                          //����Ԥ�⵹��

public:
	static const int N;                                                      //��ʾÿ������֡��
	static const int FrameShift;                                             //��������֡��
	static const double PI;                                                  //��ѧԲ����PI
	static const int Sub_band;												 //�Ӵ�����3
	static const int P;														 //LPC����
	Wav_File_Work(void) {}
	Wav_File_Work(FILE *f) :Wav_File(f) {
		try
		{
			if (Wav_File::Read_File() == false) {                            //���ø�������ȡ�ļ�
				throw;
			}
			dataDouble = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());   //ΪDouble�����������ڴ�ռ�
			if (dataDouble == NULL) {                                        //����ռ�δ�ɹ�
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //�����ֽ�����ת��ΪDouble����
				throw invalid_argument("ERROR : Data change failure !");
			}
		}
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~Wav_File_Work() {
		free(dataDouble);
	}
	bool Conversion_New(double *a, int n);//Ƶ�ײ�����һ��
	void ShowData(void);                                                     //���Ǹ����չʾ���ݺ���
};
