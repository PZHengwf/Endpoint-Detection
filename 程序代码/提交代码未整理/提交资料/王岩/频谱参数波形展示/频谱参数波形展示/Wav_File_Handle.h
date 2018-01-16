#pragma once
#include "Wav_File_Header.h"
#include <cmath>
class Wav_File_Work :protected Wav_File
{
private:

	double *dataDouble;                                                      //��ת����Double�����ݣ����Ʒ�Χ��[-1,1]

	double *Nor_dataDouble;													 //���ڱ����һ������
	
	double **SERmi;															 //���ڱ����Ӵ�������
	double **SFmi;															 //���ڱ����Ӵ�������
	double *RPm;															 //���ڱ���Ƶ�׹�����
	
	
	


	double *Xn;																 //���ڱ����ʱ����Ҷ�任������
	double *dataDFT;                                                         //���ڱ���������ɢ����Ҷ�任����
	double *dataIDFT;														 //���ڱ���������ɢ����Ҷ�任����
	double *New_dataDouble;													 //���ڱ�����ٸ���Ҷ�任֮ǰ���������
	double *dataFFT;														 //���ڱ�����ٸ���Ҷ�任������

	double *Em;															     //��������Ƶ������
	double *SCm;														     //��������Ƶ������
	double *BWm;															 //���ڱ�����

	double **Ai;															 //����Ԥ��ϵ��
	double *err;															 //����
	double *H;																 //����Ԥ����
	double **h;																 //����Ԥ�⵹��ϵ��
	double *hSpectrum;										                 //����Ԥ�⵹��


	
	int XnNumber;//��Ŷ�ʱ����Ҷ�任���ݸ���
	int dataDFTNumber;//���������ɢ����Ҷ�任���ݸ���
	int dataIDFTNumber;//���������ɢ����Ҷ�任���ݸ���
	int dataFFTNumber;//��ſ��ٸ���Ҷ�任���ݸ���
	int EmNumber;//���Ƶ���������ݸ���
	int SCmNumber;//���Ƶ���������ݸ���
	int BWmNumber;//��ſ�����ݸ���
	int AiNumber;//�������Ԥ��ϵ������
	int HNumber;//�������Ԥ���׸���
	int hNumber;//�������Ԥ�⵹��ϵ������
	int hSpectrumNumber;//�������Ԥ�⵹�׸���



	bool Conversion_Double(void);                                            //���������ֽ�����ת��ΪDouble����
	double Hamming_window(double data);                                      //����������
	int STFT(void);															 //��ʱ����Ҷ�任
	bool DFT(void);                                                          //������ɢ����Ҷ�任�����鲿
	bool IDFT(double *data, unsigned long dataNumber);                       //������ɢ����Ҷ�任�����鲿

	int Is_Pow2(void);														 //�ж�FFT�����Ƿ�Ϊ2�������η�
	long Add_Zero(int i);													 //����
	void Reverse_dataDouble(double *data, int len);						     //������������
	bool FFT(void);															 //���ٸ���Ҷ�任

	bool spectralEnergy(int len);											 //����Ƶ������
	bool spectralCentroid(int len);										     //Ƶ������
	bool bandWidth(int len);												 //���

	bool LPC_Levinson_Durbin(void);											 //����Ԥ��ϵ��
	bool LPC(void);															 //����Ԥ����
	bool LPCC_Coefficien(void);												 //����Ԥ�⵹��ϵ��
	bool LPCC(void);                                                          //����Ԥ�⵹��

	

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

			dataIDFT = (double *)malloc(sizeof(double)*Wav_File::Get_dataNumber());//Ϊ������Ҷ�任��������ռ�
			if (dataIDFT == NULL) {                                        //����ռ�δ�ɹ�
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //�����ֽ�����ת��ΪDouble����
				throw invalid_argument("ERROR : Data change failure !");
			}
			this->STFT();
			this->FFT();
			//this->DFT();
			//this->IDFT(dataIDFT, Wav_File::Get_dataNumber());
			this->spectralEnergy(Get_dataFFTNumber());
			this->spectralCentroid(Get_dataFFTNumber());
			this->bandWidth(Get_dataFFTNumber());
			this->LPC_Levinson_Durbin();
			this->LPC();
			this->LPCC_Coefficien();
			this->LPCC();
		}
		
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~Wav_File_Work() {
		free(dataDouble);
	}

	bool Conversion_New(double *a,int n);									 //Ƶ�ײ�����һ������

	double Get_Xn(int n);										//���ڻ�ȡ��ʱ����Ҷ�任ָ��λ������
	double Get_XnNumber(void);									//���ڻ�ȡ��ʱ����Ҷ�任���ݸ���
	
	double Get_dataDFT(int n);									//���ڻ�ȡ������ɢ����Ҷ�任ָ��λ������
	double Get_dataDFTNumber(void);								//���ڻ�ȡ������ɢ����Ҷ�任���ݸ���

	double Get_dataIDFT(int n);									//���ڻ�ȡ������ɢ����Ҷ�任ָ��λ������
	double Get_dataIDFTNumber(void);							//���ڻ�ȡ������ɢ����Ҷ�任���ݸ���


	double Get_dataFFT(int n);									//���ڻ�ȡ���ٸ���Ҷ�任ָ��λ������
	double Get_dataFFTNumber(void);								//���ڻ�ȡ���ٸ���Ҷ�任���ݸ���

	double Get_Em(int n);								        //���ڻ�ȡָ��λ��Ƶ������
	double Get_EmNumber(void);									//���ڻ�ȡƵ����������

	double Get_SCm(int n);										//���ڻ�ȡָ��λ��Ƶ������
	double Get_SCmNumber(void);									//���ڻ�ȡƵ�����ĸ���

	double Get_BWm(int n);										//���ڻ�ȡָ��λ�ÿ��
	double Get_BWmNumber(void);									//���ڻ�ȡ�������

	double* Get_Ai(int n);										//���ڻ�ȡָ��λ������Ԥ��ϵ��
	double Get_AiNumber(void);									//���ڻ�ȡ����Ԥ��ϵ������

	double Get_H(int n);										//���ڻ�ȡָ��λ������Ԥ����
	double Get_HNumber(void);									//���ڻ�ȡ����Ԥ���׸���

	double* Get_h(int n);										//���ڻ�ȡָ��λ������Ԥ�⵹��ϵ��
	double Get_hNumber(void);									//���ڻ�ȡ����Ԥ���׸���

	double Get_hSpectrum(int n);								//���ڻ�ȡָ��λ������Ԥ�⵹��
	double Get_hSpectrumNumber(void);							//���ڻ�ȡ����Ԥ�⵹�׸���
	
	
	
	
	
	int Subband_Energy_Ratio(int len);										 //�Ӵ�������
	void Normalized_Spectral_Signal(int k);								     //��һ��Ƶ���ź�
	int Subband_Spectral_Flux(int len);									     //�Ӵ�������
	int Spectral_Rolloff_Point(int len);								     //Ƶ�׹�����
	
	
	
	void ShowData(void);                                                     //���Ǹ����չʾ���ݺ���
};
