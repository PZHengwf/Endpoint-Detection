#pragma once
#include "Wav_File_Header.h"
#include <cmath>
#include <bitset>
#include <complex>
#include <fstream>

#define _PARAGRAPH_MUTE 0                                                    //����׼���׶�
#define _PARAGRAPH_INTERIM 1                                                 //��������׶�
#define _PARAGRAPH_VOICE 2                                                   //�������н׶�
#define _PARAGRAPH_BURST 3                                                   //�������ƽ׶�

struct FeatureParameter
{
	double voice;
	double engChart;
	double cepsDistance;
	double engDevia;
	double spectrumEnt;

	FeatureParameter(double voice, double engChart, double cepsDistance,
		double engDevia, double spectrumEnt) {
		this->voice = voice;
		this->engChart = engChart;
		this->cepsDistance = cepsDistance;
		this->engDevia = engDevia;
		this->spectrumEnt = spectrumEnt;
	}
};


struct KalmanFilter
{
	double x;                                                                //ϵͳ��״̬��
	double p;                                                                //�������Э����
	double gain;                                                             //����������
	double A;                                                                //x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
	double H;                                                                //z(n)=H*x(n)+w(n),w(n)~N(0,r)
	double q;                                                                //Ԥ���������Э����
	double r;                                                                //������������Э����

	static double predict_q;
	static double newMeasured_q;

	KalmanFilter(double init_x, double init_p) {
		this->x = init_x;                                                    //�������ĳ�ʼֵ��������ֵһ�������ֵ
		this->p = init_p;                                                    //����״̬����ֵ���ķ���ĳ�ʼֵ����ҪΪ0���ⲻ��
		this->A = 1;
		this->H = 1;
		this->q = predict_q;                                                 //Ԥ�⣨���̣��������� Ӱ���������ʣ����Ը���ʵ���������
		this->r = newMeasured_q;                                             //�������۲⣩�������� ����ͨ��ʵ���ֶλ��
	}
	double kalmanFilter_filter(double newMeasured) {
		this->x = this->A * this->x;                                         //%x�������������һ��ʱ���ĺ������ֵ��������Ϣ����
		this->p = this->A * this->A * this->p + this->q;                     //������������� p(n|n-1)=A^2*p(n-1|n-1)+q

		this->gain = this->p * this->H / (this->p * this->H * this->H + this->r);
		this->x = this->x + this->gain * (newMeasured - this->H * this->x);  //���ò������Ϣ���ƶ�x(t)�Ĺ��ƣ�����������ƣ����ֵҲ�������
		this->p = (1 - this->gain * this->H) * this->p;                      //%������������
		return this->x;
	}
};


class Wav_File_Work :protected Wav_File
{
private:
	double *dataDouble;                                                      //��ת����Double�����ݣ����Ʒ�Χ��[-1,1]
	double **dataFrameWindow;                                                //�����֡�Ӵ��������
	unsigned long frameNumber;                                               //��������֡�ĸ���

	unsigned long voiceNumber;                                               //�����������
	vector<VoiceParagraph> voiceParagraph;                                   //������������ļ�������Ҫ�������������

	unsigned long fftNumber;                                                 //����Ҷ�任������ݵĸ���

	double **dataCeps;                                                       //���ڱ��浹������
	int *dataPitch;                                                          //���ڱ����ڼ����굹�׺�Ļ�������
	double **dataMfcc;                                                       //��������mel����ϵ��
	double *dataCepsDistance;                                                //�������浹�׾���
	double *dataEngChart;                                                    //���������ʱ����
	double *dataEngDevia;                                                    //�������������׷���
	double **dataAutoFun;                                                    //������������Ӧ��������
	double **dataPSDF;                                                       //�������湦�����ܶȺ���
	double *dataSpectrumEnt;                                                 //�������湦��������ֵ

	bool Conversion_Double(void);                                            //���������ֽ�����ת��ΪDouble����
	double Hamming_window(double data);                                      //����������
	bool Frame_Window(void);                                                 //���������ֽ����ݷ�֡�Ӵ�

	bool kalmanFilter_filter(void);                                          //���ڽ����ݽ��п������˲�

	double *DistributionSpace(unsigned long col);                            //����һά����ռ�
	double **DistributionSpace(unsigned long row, unsigned long col);        //�����ά����ռ�
	void ReleaseSpace(double **data, unsigned long row);                     //�ͷŶ�ά����ռ�

	void Rader(double *data, unsigned long dataNumber);                      //�׵��㷨������dataNumberһ����2^n
	bool DFT(double *data, unsigned long dataNumber);                        //������ɢ����Ҷ�任�����鲿
	bool IDFT(double *data, unsigned long dataNumber);                       //������ɢ����Ҷ�任�����鲿
	double* FFT(double *data, unsigned long dataNumber);                     //������ɢ����Ҷ�任�����鲿 (���ؾ��棬��Ϊ���ܳ�����չ�ڴ����������Ա��뷵���µ�ַ)
	bool IFFT(double *data, unsigned long dataNumber);                       //������ɢ�渵��Ҷ�任�����鲿
	bool FrameFFT(double** data, unsigned long frameNumber);                 //���ڼ����֡�������FFT
	bool FrameIFFT(double** data, unsigned long frameNumber);                //���ڼ����֡�������IFFT
	double Mel_Filter(unsigned long data, int filterFlag);                   //mel �����˲���
	double FilterFunction(int flag);                                         //���ڼ��������˲���Ƶ�ʵĸ�������
	bool AutocorrelativeFunction(void);                                      //����ÿһ֡������غ����������������ص�����һ����
	bool PSDF(void);                                                         //�����һ���Ĺ������ܶȺ���,������֮ǰ��������AutoCorrelativeFunction

	bool NormallizationFun(double *data, unsigned long dataNumber);
	void Normalization(void);                                                //���ص����ݹ�һ������

	//��״̬������ (����Ϊ��ǰ֡λ�ã����صĽ���״̬��ת�����)

	void FrameVoice(double *voice);                                          //����һ���ļ��㷽ʽ����ÿһ֡�ı�׼ֵ

	int Mute_State_Process(FeatureParameter framePoint);                     //������״̬����
	int Interim_State_Process(FeatureParameter framePoint, unsigned long length);      //��������״̬����
	int Voice_State_Process(FeatureParameter framePoint);                    //����״̬����
	int Burst_State_Process(FeatureParameter framePoint, unsigned long length);        //������/������״̬����

	void ShowEndpoint();                                                     //ʹ�����������ȡ���еĶ���������ʾ
public:
	static const unsigned long N;                                            //��ʾÿ������֡��
	static const int FrameShift;                                             //��������֡��
	static const double PI;                                                  //��ѧԲ����PI
	static const int FilterNumber;                                           //mel �˲�������
	static const int CoefficientOrder;                                       //mfcc����

	static double threshold_h_EngChart;                                      //�������������źŶ�ʱ����������
	static double threshold_l_EngChart;                                      //�������������źŶ�ʱ����������

	static double threshold_h_CepsDistance;                                  //�������������źŵ��׾��������
	static double threshold_l_CepsDistance;                                  //�������������źŵ��׾��������

	static double threshold_h_EngDevia;                                      //�������������ź��������������
	static double threshold_l_EngDevia;                                      //�������������źŵ��׾��������

	static double threshold_h_SpectrumEnt;                                   //�������������ź�����-�ظ�����
	static double threshold_l_SpectrumEnt;                                   //�������������ź�����-�ص�����

	static unsigned long interim_StateLength;                                //��������״̬��̳�������
	static unsigned long burst_StateLength;                                  //������\������״̬��̳�������

	Wav_File_Work(void) {}
	Wav_File_Work(FILE *f) :Wav_File(f) {
		try
		{
			if (Wav_File::Read_File() == false) {                            //���ø�������ȡ�ļ�
				throw;
			}

			dataDouble = NULL;
			dataFrameWindow = NULL;
			dataCeps = NULL;
			dataPitch = NULL;
			dataMfcc = NULL;
			dataCepsDistance = NULL;
			dataEngChart = NULL;
			dataEngDevia = NULL;
			dataAutoFun = NULL;
			dataPSDF = NULL;
			dataSpectrumEnt = NULL;

			dataDouble = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());   //ΪDouble�����������ڴ�ռ�
			if (dataDouble == NULL) {                                        //����ռ�δ�ɹ�
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //�����ֽ�����ת��ΪDouble����
				throw invalid_argument("ERROR : Data change failure !");
			}

			if (kalmanFilter_filter() == false) {                            //�����ֽ�����ת��ΪDouble����
				throw invalid_argument("ERROR : Data kalman filter failure !");
			}

			if (Frame_Window() == false) {                                   //��֡�Ӵ�
				throw invalid_argument("ERROR : Frame windows failure !");
			}


			//this->Cepstrum();                                                //���㵹Ƶ��
			//this->Pitch();                                                   //�����������

			this->MFCC();                                                    //����÷������ϵ��
			this->CepstrumDistance();                                        //���㵹�׾���


			this->EnergySpectrum();                                          //����������
			this->EnergyDeviation();                                         //���������׷���

			this->SpectrumEntropy();                                         //��������

			this->Normalization();

			this->ShowData();

			//this->Endpoint_Detection();
			//this->ShowEndpoint();

		}
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~Wav_File_Work() {
		free(dataDouble);
	}
	double Get_dataNumber(void);                                             //��ȡDouble���ݵĸ���
	double Get_DataDouble(unsigned long Number);                             //��ȡת�����Double����
	int Get_WindowLength(void);                                              //��ȡ֡�������Ĵ�С��
	unsigned long Get_FrameNumber(void);                                     //��ȡ֡��

	double Get_DataCepsDistance(unsigned long Number);                       //��ȡָ��λ���ϵĵ��׾���
	double Get_DataEngChart(unsigned long Number);                           //��ȡָ��λ���ϵ�������
	double Get_DataEngDevia(unsigned long Number);                           //��ȡָ��λ���ϵ������׷���
	double Get_DataSpectrumEnt(unsigned long Number);                        //��ȡָ��λ���ϵĹ���������

	unsigned long Get_voiceNumber(void);                                     //��ȡ�����������
	VoiceParagraph Get_dataVoicePoint(unsigned long Number);                 //��ȡĳ��������
	void ShowData(void);                                                     //���Ǹ����չʾ���ݺ���
	void SaveNewWav(char* path);                                             //����ȥ���հ״��������ļ�
	void SaveParagraphInfo(char* path);                                      //���������Ϣ
	bool Cepstrum(void);                                                     //��Ƶ��
	bool Pitch(void);                                                        //�������ڣ�����ע��-����ǰ�����ȼ��㵹��Cepstrum��
	bool MFCC(void);                                                         //÷������ϵ��
	bool CepstrumDistance(void);                                             //���㵹�׾��루����ע��-����ǰ�����ȼ���mel����ϵ��MFCC��
	bool EnergySpectrum(void);                                               //�����ʱ����
	bool EnergyDeviation(void);                                              //���������׷���(����ע��-����ǫ�����ȼ���������)
	bool SpectrumEntropy(void);                                              //��������
	bool Endpoint_Detection(void);                                           //�˵��⺯��
};
