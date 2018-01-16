#include "stdafx.h"
#include "Wav_File_Handle.h"

double KalmanFilter::predict_q = 0.7;                                        //Ԥ������е�����Э����    ������ע�����˴�����ֵ��Ҫ������ʷ��������ʵ����ܻ��������ݣ�
double KalmanFilter::newMeasured_q = 0.7;                                    //���������е�����Э����    ������ע�����˴�����ֵ��Ҫ������ʷ��������ʵ����ܻ��������ݣ�

const unsigned long Wav_File_Work::N = 256;                                  //��ʼ��ÿ�����Ĵ���
const int Wav_File_Work::FrameShift = 10;                                    //��ʼ����������֡��
const double Wav_File_Work::PI = 3.14159;                                    //��ʼ��Բ����
const int Wav_File_Work::FilterNumber = 24;                                  //mel �˲�������
const int Wav_File_Work::CoefficientOrder = 12;                              //mfcc����

double Wav_File_Work::threshold_h_CepsDistance = 0.8;
double Wav_File_Work::threshold_h_EngChart = 0.8;
double Wav_File_Work::threshold_h_EngDevia = 0.8;
double Wav_File_Work::threshold_h_SpectrumEnt = 0.8;

double Wav_File_Work::threshold_l_CepsDistance = 0.2;
double Wav_File_Work::threshold_l_EngChart = 0.2;
double Wav_File_Work::threshold_l_EngDevia = 0.2;
double Wav_File_Work::threshold_l_SpectrumEnt = 0.2;

unsigned long Wav_File_Work::interim_StateLength = 10;                       //��������״̬��̳�������
unsigned long Wav_File_Work::burst_StateLength = 5;                          //������\������״̬��̳�������

bool Wav_File_Work::Conversion_Double(void)                                  //���������ֽ�����ת��ΪDouble����
{
	try
	{
		cout << "TIP : Change to double data ..." << endl;
		const double Flag = pow((double)2, Wav_File::Get_ChunkAlign() * 8);  //��ʾ����ԭ�������ֵ��һ��
		for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {     //����ÿ������
			dataDouble[i] = (double)(Wav_File::Get_Data(i) / Flag);          //����ÿ��������[-1,1]֮��
		}
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}

double Wav_File_Work::Hamming_window(double data)                            //����������
{
	if (data >= 0 && data <= (N - 1)) {                                      //0<= n <= N-1�����
		return 0.54 - 0.46*cos(2 * PI*data / (N - 1));                       //������ֵ
	}
	else {
		return 0;
	}
}

bool Wav_File_Work::Frame_Window(void)                                       //���������ֽ����ݷ�֡�Ӵ�
{
	cout << "TIP : Frame window ..." << endl;
	if (dataFrameWindow) {                                                   //���ԭ��������
		ReleaseSpace(dataFrameWindow, frameNumber);
	}
	frameNumber = (unsigned long)(Wav_File_Work::Get_dataNumber() - Wav_File_Work::N) / Wav_File_Work::FrameShift + 1;         //��֡��
	if (frameNumber < 1) {
		return false;
	}
	dataFrameWindow = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //����֡�ĸ���
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {               //�������е�ÿһ������
			if ((j + i*Wav_File_Work::FrameShift) > this->Get_dataNumber()) {
				dataFrameWindow[i][j] = 0;
			}
			dataFrameWindow[i][j] = dataDouble[j + i*Wav_File_Work::FrameShift] * Hamming_window(j);       //����ԭ�������е�����
		}
	}
	return true;
}

bool Wav_File_Work::kalmanFilter_filter(void)                                //���ڽ����ݽ��п������˲�
{
	double sumData = 0;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
		sumData += dataDouble[i];
	}

	KalmanFilter *kalman = new KalmanFilter(sumData / Wav_File::Get_dataNumber(), 0);

	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {         //ÿ�����ݽ��뿨�����˲�������
		dataDouble[i] = kalman->kalmanFilter_filter(dataDouble[i]);
	}

	return true;
}

double* Wav_File_Work::DistributionSpace(unsigned long col)                  //����һά����ռ�
{
	double *tempspace = (double*)malloc(sizeof(double)*col);
	if (tempspace == NULL) {
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}
	return tempspace;
}

double** Wav_File_Work::DistributionSpace(unsigned long row, unsigned long col)        //�����ά����ռ�
{
	double **tempspace = (double**)malloc(sizeof(double*)*row);
	if (tempspace == NULL) {
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}
	for (unsigned long i = 0; i < row; ++i) {
		tempspace[i] = (double*)malloc(sizeof(double)*col);
		if (tempspace[i] == NULL) {
			throw invalid_argument("ERROR : Memory failure !");
			return false;
		}
	}
	return tempspace;
}

void Wav_File_Work::ReleaseSpace(double **data, unsigned long row)           //�ͷŶ�ά����ռ�
{
	for (unsigned long i = 0; i < row; ++i) {
		free(data[i]);
	}
	free(data);
}

void Wav_File_Work::Rader(double *data, unsigned long dataNumber)            //�׵��㷨������dataNumberһ����2^n
{
	unsigned long endNum;
	double temp;
	endNum = 0;
	for (unsigned long i = 0; i < dataNumber - 1; i++) {                     //��Ϊ��һ������Զ��0���䣬���Բ��Ҵ���ΪN-1
		if (i < endNum) {                                                    //���˳����С���������򽻻������Ƿ�ֹ�ظ�������һ���ֶΣ���Ϊ���ں�Ҳ��
			temp = data[i];                                                  //��������
			data[i] = data[endNum];
			data[endNum] = temp;
		}
		unsigned long k = dataNumber >> 1;                                   //100 ��������һ�룬�����ж����λ�Ƿ�Ϊ1
		while (k <= endNum) {                                                //��������������λΪ1
			endNum = endNum - k;                                             //��ǰλ��0
			k >>= 1;                                                         //����/2
		}
		endNum = endNum + k;                                                 //��ǰλΪ0������תΪ1
	}
}

bool Wav_File_Work::DFT(double *data, unsigned long dataNumber)              //������ɢ����Ҷ�任�����鲿
{
	double exp;                                                              //����ָ������
	double *dataDFT;
	dataDFT = DistributionSpace(dataNumber);
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //һ��һ�������µ�����
		dataDFT[i] = 0;                                                      //Ԥ��������
		exp = -2 * PI*i / dataNumber;                                        //ָ������
		for (unsigned long j = 0; j < dataNumber; ++j) {                     //���ۼӺ�
			dataDFT[i] += data[j] * cos(j*exp);                              //��ʵ�����ֵ�����
		}
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {
		data[i] = dataDFT[i];                                                //��������
	}
	free(dataDFT);
	return true;
}

bool Wav_File_Work::IDFT(double *data, unsigned long dataNumber)             //������ɢ����Ҷ�任�����鲿
{
	double exp;                                                              //����ָ������
	double *dataIDFT;
	dataIDFT = DistributionSpace(dataNumber);
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //һ��һ�������µ�����
		dataIDFT[i] = 0;                                                     //Ԥ��������
		exp = -2 * PI*i / dataNumber;                                        //ָ������
		for (unsigned long j = 0; j < dataNumber; ++j) {                     //���ۼӺ�
			dataIDFT[i] += data[j] * cos(j*exp);                             //��ʵ�����ֵ�����
		}
		dataIDFT[i] /= dataNumber;                                           //����ع�
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {
		data[i] = dataIDFT[i];                                               //��������
	}
	free(dataIDFT);
	return true;
}

double* Wav_File_Work::FFT(double *data, unsigned long dataNumber)           //������ɢ����Ҷ�任�����鲿
{
	short power = 0;
	for (int i = dataNumber - 1; i > 0; ++power, i /= 2);

	fftNumber = (unsigned long)pow((float)2, (int)power);
	double *dataFFT = DistributionSpace(fftNumber);
	for (unsigned long i = 0; i < dataNumber; i++) {
		dataFFT[i] = data[i];                                                //��ʼ�����ٸ���Ҷ�任����
	}
	double *W = DistributionSpace(fftNumber / 2);                            //������ת����
	for (unsigned long i = 0; i < fftNumber / 2; i++) {
		W[i] = cos(2 * i * PI / fftNumber);
	}
	unsigned long ulGroupLength = 1;										 //�εĳ���
	unsigned long ulHalfLength = 0;											 //�γ��ȵ�һ��
	unsigned long ulGroupCount = 0;											 //�ε�����
	double cw, c1, c2;
	for (short b = 0; b < power; b++)                                        //����FFT
	{
		ulHalfLength = ulGroupLength;
		ulGroupLength *= 2;
		for (unsigned long j = 0; j < fftNumber; j += ulGroupLength)
		{
			for (unsigned long k = 0; k < ulHalfLength; k++)
			{
				cw = W[k * fftNumber / ulGroupLength], dataFFT[j + k + ulHalfLength];
				c1 = dataFFT[j + k] + cw;
				c2 = dataFFT[j + k] - cw;
				dataFFT[j + k] = c1;
				dataFFT[j + k + ulHalfLength] = c2;
			}
		}
	}
	free(W);
	if (fftNumber == dataNumber) {                                           //���FFT����ʱ�������ã�û����չ������Ҫ��������
		for (unsigned long i = 0; i < dataNumber; ++i) {
			data[i] = dataFFT[i];
		}
		free(dataFFT);
		return NULL;
	}
	else {                                                                   //������Ҫ�����µ����ݵ�ַ
		free(data);
		return dataFFT;
	}
}

bool Wav_File_Work::IFFT(double *data, unsigned long dataNumber)             //������ɢ�渵��Ҷ�任�����鲿
{
	short power = 0;
	double *dataIFFT;
	for (unsigned long i = dataNumber - 1; i > 0; ++power, i /= 2);          //�������
	for (unsigned long i = 1, j = dataNumber / 2; i < N - 1; ++i) {          //�׵��㷨����λ��
		if (i < j) {
			double temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
		unsigned long k = dataNumber / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}
	dataIFFT = DistributionSpace(dataNumber);
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //�����
		dataIFFT[i] = data[i] / dataNumber;
	}
	for (short p = 0; p < power; ++p) {                                      //IFFT����
		for (unsigned long i = 0; i < pow((long double)2, (long double)p); ++i) {
			double cw = i*pow((long double)2, (long double)power - (p + 1)); //������ת����
			for (unsigned long j = i; j < dataNumber - 1; j += (unsigned long)pow((long double)2, (long double)(p + 1))) {
				unsigned long index = j + (unsigned long)pow((long double)2, (long double)p);
				cw = dataIFFT[index] * cos(2 * Wav_File_Work::PI*cw / dataNumber);
				dataIFFT[index] = dataIFFT[j] - cw;
				dataIFFT[j] = dataIFFT[j] + cw;
			}
		}
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //�������ݵ�����
		data[i] = dataIFFT[i];
	}
	free(dataIFFT);
	return true;
}

bool Wav_File_Work::FrameFFT(double** data, unsigned long frameNumber)       //���ڼ����֡�������FFT
{
	cout << "TIP : Calculate FrameFFT ..." << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //��֡����FFT
		double *space = this->FFT(data[i], Wav_File_Work::N);                //�����·��صĵ�ַ����Ϊ���п����������ݵĵ�ַ
		if (space != NULL) {
			data[i] = space;
		}
	}
	return true;
}
bool Wav_File_Work::FrameIFFT(double** data, unsigned long frameNumber)      //���ڼ����֡�������IFFT
{
	cout << "TIP : Calculate FrameIFFT ..." << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //��֡����IFFT
		this->IFFT(data[i], Wav_File_Work::N);
	}
	return true;
}

double Wav_File_Work::Mel_Filter(unsigned long data, int filterFlag)         //mel �����˲���         ��ʽ��������
{
	if (data < FilterFunction(filterFlag - 1) || data >= FilterFunction(filterFlag + 1)) {
		return 0;
	}
	else if (data >= FilterFunction(filterFlag - 1) && data <= FilterFunction(filterFlag)) {
		return 2 * (data - FilterFunction(filterFlag - 1)) /
			((FilterFunction(filterFlag + 1) - FilterFunction(filterFlag - 1))*
			(FilterFunction(filterFlag) - FilterFunction(filterFlag - 1)));
	}
	else if (data >= FilterFunction(filterFlag) && data <= FilterFunction(filterFlag + 1)) {
		return 2 * (FilterFunction(filterFlag - 1) - data) /
			((FilterFunction(filterFlag + 1) - FilterFunction(filterFlag - 1))*
			(FilterFunction(filterFlag) - FilterFunction(filterFlag - 1)));
	}
	else {
		return 0;
	}
}

double Wav_File_Work::FilterFunction(int flag)                               //���ڼ��������˲���Ƶ�ʵĸ�������      ������������
{
	return 700 * (exp(log((long double)1 + 20000 / 700)*flag / (Wav_File_Work::FilterNumber + 1)) - 1);
}

bool Wav_File_Work::AutocorrelativeFunction(void)                            //����ÿһ֡������غ����������������ص�����һ����
{
	if (dataAutoFun) {
		ReleaseSpace(dataAutoFun, frameNumber);
	}
	dataAutoFun = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataAutoFun[i][j] = 0;
			for (unsigned long k = 0; k < Wav_File_Work::N - j; ++k) {
				dataAutoFun[i][j] += dataFrameWindow[i][k] * dataFrameWindow[i][k + j];
			}
		}
	}
	return true;
}

bool Wav_File_Work::PSDF(void)                                               //�����һ���Ĺ������ܶȺ���
{
	if (dataPSDF) {
		ReleaseSpace(dataPSDF, frameNumber);
	}
	dataPSDF = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		double sum = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N / 2; ++j) {
			sum += dataAutoFun[i][j];
		}
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataPSDF[i][j] = dataAutoFun[i][j] / sum;
		}
	}
	return true;
}

bool Wav_File_Work::NormallizationFun(double *data, unsigned long dataNumber)
{
	double max = 0;
	for (unsigned long i = 0; i < dataNumber; ++i) {
		double tempdata = data[i] > 0 ? data[i] : -data[i];
		if (tempdata<pow((double)10, -3) || tempdata>pow((double)10, 3)) {
			data[i] = 0;
		}
		if (tempdata > max) {
			max = tempdata;
		}
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {
		data[i] /= max;
	}
	return true;
}

void Wav_File_Work::Normalization(void)                                      //���ص����ݹ�һ������
{
	if (dataDouble) {
		this->NormallizationFun(dataDouble, Wav_File::Get_dataNumber());
	}
	if (dataCepsDistance) {
		this->NormallizationFun(dataCepsDistance, frameNumber);
	}
	if (dataEngChart) {
		this->NormallizationFun(dataEngChart, frameNumber);
	}
	if (dataEngDevia) {
		this->NormallizationFun(dataEngDevia, frameNumber);
	}
	if (dataSpectrumEnt) {
		this->NormallizationFun(dataSpectrumEnt, frameNumber);
		for (unsigned long i = 0; i < frameNumber; ++i) {
			dataSpectrumEnt[i] = dataSpectrumEnt[i] * 100 - (unsigned long)(dataSpectrumEnt[i] * 100);
			if (_finite(dataSpectrumEnt[i]) == 0) {
				dataSpectrumEnt[i] = 0;
			}
		}
	}
}

void Wav_File_Work::FrameVoice(double *voice)                                //����һ���ļ��㷽ʽ����ÿһ֡�ı�׼ֵ, ��ʹ��ƽ��ֵ
{
	for (unsigned long i = 0; i < frameNumber; ++i) {
		voice[i] = 0;
		for (unsigned long j = 0; j < this->N; ++j) {
			voice[i] += this->dataFrameWindow[i][j];
		}
		voice[i] /= this->N;
	}
}

int Wav_File_Work::Mute_State_Process(FeatureParameter framePoint)           //������״̬����
{
	bool logic_A = fabs(framePoint.engChart) > this->threshold_l_EngChart;
	bool logic_B = framePoint.cepsDistance > this->threshold_l_CepsDistance;
	bool logic_C = framePoint.engDevia > this->threshold_l_EngDevia;
	bool logic_D = framePoint.spectrumEnt > this->threshold_l_SpectrumEnt;

	if ((logic_A && logic_B) || (logic_A && logic_C) || (logic_B && logic_C) ||
		(logic_A && logic_D) || (logic_C && logic_D) || (logic_B && logic_D)) {        //����һ���߼����ʽ����ʾ�������������ﵽ�����������������ɹ�
		return _PARAGRAPH_INTERIM;
	}
	else {
		return _PARAGRAPH_MUTE;
	}
}

int Wav_File_Work::Interim_State_Process(FeatureParameter framePoint, unsigned long length)      //��������״̬����
{
	//1�� to _PARAGRAPH_MUTE
	if (this->Mute_State_Process(framePoint) == _PARAGRAPH_MUTE) {
		return _PARAGRAPH_MUTE;
	}
	else {
		//2�� to _PARAGRAPH_INTERIM
		if (length < this->interim_StateLength) {
			return _PARAGRAPH_INTERIM;
		}
		else {
			//3�� to _PARAGRAPH_VOICE
			return _PARAGRAPH_VOICE;
		}
	}
}

int Wav_File_Work::Voice_State_Process(FeatureParameter framePoint)          //����״̬����
{
	bool logic_A = (fabs(framePoint.engChart) > this->threshold_l_EngChart) &&
		(framePoint.engChart < this->threshold_h_EngChart);
	bool logic_B = (framePoint.cepsDistance > this->threshold_l_CepsDistance) &&
		(framePoint.cepsDistance < this->threshold_h_CepsDistance);
	bool logic_C = (framePoint.engDevia > this->threshold_l_EngDevia) &&
		(framePoint.engDevia < this->threshold_h_EngDevia);
	bool logic_D = (framePoint.spectrumEnt > this->threshold_l_SpectrumEnt) &&
		(framePoint.spectrumEnt < this->threshold_h_SpectrumEnt);

	if ((logic_A && logic_B) || (logic_A && logic_C) || (logic_B && logic_C) ||
		(logic_A && logic_D) || (logic_C && logic_D) || (logic_B && logic_D)) {        //����һ���߼����ʽ����ʾ�������������ﵽ�����������������ɹ�
			//�����������������������״̬
		return _PARAGRAPH_VOICE;
	}
	else {
		//��������֮��
		return _PARAGRAPH_BURST;
	}
}

int Wav_File_Work::Burst_State_Process(FeatureParameter framePoint, unsigned long length)        //������/������״̬����
{
	//1) to _PARAGRAPH_MUTE
	if (length > this->burst_StateLength) {
		return _PARAGRAPH_MUTE;
	}
	else {
		//2) to _PARAGRAPH_VOICE
		if (this->Voice_State_Process(framePoint) == _PARAGRAPH_VOICE) {
			return _PARAGRAPH_VOICE;
		}
		else {
			//3) to _PARAGRAPH_BURST
			return _PARAGRAPH_BURST;
		}
	}
}

void Wav_File_Work::ShowEndpoint()                                           //ʹ�����������ȡ���еĶ���������ʾ
{
	for (unsigned long i = 0; i < this->voiceNumber; ++i) {
		VoiceParagraph tempParagraph = this->Get_dataVoicePoint(i);
		cout << tempParagraph.begin << " " << tempParagraph.end << " " << tempParagraph.voiceLength << endl;
	}
}

double Wav_File_Work::Get_dataNumber(void)                                   //��ȡDouble���ݵĸ���
{
	return Wav_File::Get_dataNumber();
}

double Wav_File_Work::Get_DataDouble(unsigned long Number)                   //��ȡת�����Double����
{
	if (Number >= (Wav_File::Get_dataNumber()) || Number < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataDouble[Number];
	}
}

int Wav_File_Work::Get_WindowLength(void)                                    //��ȡ֡�������Ĵ�С��
{
	return N;
}

unsigned long Wav_File_Work::Get_FrameNumber(void)                           //��ȡ֡��
{
	return frameNumber;
}

double Wav_File_Work::Get_DataCepsDistance(unsigned long Number)             //��ȡָ��λ���ϵĵ��׾���
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataCepsDistance[Number];
	}
}
double Wav_File_Work::Get_DataEngChart(unsigned long Number)                 //��ȡָ��λ���ϵ�������
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataEngChart[Number];
	}
}
double Wav_File_Work::Get_DataEngDevia(unsigned long Number)                 //��ȡָ��λ���ϵ������׷���
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataEngDevia[Number];
	}
}
double Wav_File_Work::Get_DataSpectrumEnt(unsigned long Number)              //��ȡָ��λ���ϵĹ���������
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataSpectrumEnt[Number];
	}
}


unsigned long Wav_File_Work::Get_voiceNumber(void)                           //��ȡ�����������
{
	return voiceNumber;
}

VoiceParagraph Wav_File_Work::Get_dataVoicePoint(unsigned long Number)       //��ȡĳ��������
{
	if (Number >= voiceNumber || Number < 0) {                               //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
	}
	else {
		return voiceParagraph[Number];
	}
}

void Wav_File_Work::ShowData(void)                                           //���Ǹ����չʾ���ݺ���
{
	/*
	int max = 0, min = 0;
	cout << "TIP : Double data " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
		cout << dataDouble[i] << "\t";
		if (dataDouble[i] > dataDouble[max]) {
			max = i;
		}
		if (dataDouble[i] < dataDouble[min]) {
			min = i;
		}
	}
	cout << endl;
	*/
	/*
	cout << "Max " << dataDouble[max] << endl;
	cout << "Min " << dataDouble[min] << endl;
	cout << "TIP : Parameter " << endl;
	cout << "Max energy " << Get_maxEnergy() << endl;
	cout << "Min energy " << Get_minEnergy() << endl;
	cout << "Max ZCR " << Get_maxZCR() << endl;
	cout << "Min ZCR " << Get_minZCR() << endl;

	cout << "TIP : Energy " << endl;
	for (auto i : dataEnergy) {
	cout << i << "\t";
	}
	cout << endl;

	cout << "TIP : ZCR " << endl;
	for (auto i : dataZCR) {
	cout << i << "\t";
	}
	cout << endl;

	for (int i = 0; i < (Wav_File::Get_dataNumber() - N); ++i) {
	cout << Get_DataEnergy(i) << endl;
	}

	for (int i = 0; i < (Wav_File::Get_dataNumber() - N); ++i) {
	cout << Get_DataZCR(i) << endl;
	}
	*/

	/*
	cout << "TIP : DFT " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
	cout << dataDFT[i] << "\t";
	}
	cout << endl;
	*/
	/*
	cout << "TIP : FFT " << endl;
	for (unsigned long i = 0; i < fftNumber; ++i) {
	cout << dataFFT[i] << "\t";
	}
	cout << endl;
	*/
	/*
	cout << "TIP : Cepstrum " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
	cout << dataCeps[i] << "\t";
	}
	cout << endl;
	*/

	/*
	for (unsigned long i = 0; i < frameNumber; ++i) {
	for (int j = 0; j < Wav_File_Work::CoefficientOrder; ++j) {
	cout << dataMfcc[i][j] << "\t";
	}
	cout << endl;
	}
	*/

	/*
	cout << "���׾������ݣ�" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
	cout<< dataCepsDistance[i] << "\t";
	}
	cout << endl;
	cout << "���������ݣ�" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
	cout << dataEngChart[i] << "\t";
	}
	cout << endl;
	*/
	/*
	cout << "�����������ݣ�" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataEngDevia[i] *= 10;
		dataEngDevia[i] = dataEngDevia[i] - (int)dataEngDevia[i];
		cout << dataEngDevia[i] << "\t";
	}
	cout << endl;
	*/

	cout << "�������ݣ�" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
		cout << dataSpectrumEnt[i] << "\t";
	}
	cout << endl;

}

void Wav_File_Work::SaveNewWav(char* path)                                    //����ȥ���հ״��������ļ�
{
	Wav_File::SaveNewWav(voiceNumber, voiceParagraph, Wav_File_Work::FrameShift, path);                //���ø�������ɺ���
}

void Wav_File_Work::SaveParagraphInfo(char* path)                            //��������Ϣ���ļ�
{
	ofstream paragraphData(path, ios::out);
	if (paragraphData.is_open()) {
		unsigned long sumLength = 0;
		paragraphData << "No | Begin | End | Length" << endl;
		for (unsigned long i = 0; i < this->Get_voiceNumber(); ++i) {
			VoiceParagraph tempPoint = this->Get_dataVoicePoint(i);
			sumLength += tempPoint.voiceLength;
			paragraphData << (i + 1) << " | " << tempPoint.begin << " | " << tempPoint.end << " | " << tempPoint.voiceLength << endl;
		}
		paragraphData << endl;
		paragraphData << "Paragraph number : " << this->Get_voiceNumber() << endl;
		paragraphData << "Paragraph data number : " << this->Get_FrameNumber() << endl;
		paragraphData << "Paragraph useful length : " << sumLength << endl << endl;

		paragraphData << "Settings" << endl;
		paragraphData << "----------------------------" << endl;
		paragraphData << "predict_q : " << KalmanFilter::predict_q << endl;
		paragraphData << "newMeasured_q : " << KalmanFilter::newMeasured_q << endl;
		paragraphData << "N : " << Wav_File_Work::N << endl;
		paragraphData << "FrameShift : " << Wav_File_Work::FrameShift << endl;
		paragraphData << "PI : " << Wav_File_Work::PI << endl;
		paragraphData << "FilterNumber : " << Wav_File_Work::FilterNumber << endl;
		paragraphData << "CoefficientOrder : " << Wav_File_Work::CoefficientOrder << endl;
		paragraphData << "threshold_h_CepsDistance : " << Wav_File_Work::threshold_h_CepsDistance << endl;
		paragraphData << "threshold_h_EngChart : " << Wav_File_Work::threshold_h_EngChart << endl;
		paragraphData << "threshold_h_EngDevia : " << Wav_File_Work::threshold_h_EngDevia << endl;
		paragraphData << "threshold_h_SpectrumEnt : " << Wav_File_Work::threshold_h_SpectrumEnt << endl;
		paragraphData << "threshold_l_CepsDistance : " << Wav_File_Work::threshold_l_CepsDistance << endl;
		paragraphData << "threshold_l_EngChart : " << Wav_File_Work::threshold_l_EngChart << endl;
		paragraphData << "threshold_l_EngDevia : " << Wav_File_Work::threshold_l_EngDevia << endl;
		paragraphData << "threshold_l_SpectrumEnt : " << Wav_File_Work::threshold_l_SpectrumEnt << endl;
		paragraphData << "interim_StateLength : " << Wav_File_Work::interim_StateLength << endl;
		paragraphData << "burst_StateLength : " << Wav_File_Work::burst_StateLength << endl;
	}
	else {
		cout << "ERROR : Paragraph write open fail !" << endl;
	}
}

bool Wav_File_Work::Cepstrum(void)                                           //��Ƶ��
{
	cout << "TIP : Calculate CepsTrum ..." << endl;
	if (frameNumber < 1) {                                                   //��������֡�Ĵ���
		throw invalid_argument("ERROR : No frame !");
		return false;
	}

	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //�������ݵ��������飬����ԭ������
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);                                   //��֡���ݽ���FFT
	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = log(fabs(tempData[i][j]));                      //��ģ����
		}
	}

	dataCeps = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //�������ݵ��������
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataCeps[i][j] = tempData[i][j];
		}
	}

	ReleaseSpace(tempData, frameNumber);

	this->FrameIFFT(dataCeps, frameNumber);                                  //��֡���ݽ���IFFT
	return true;
}

bool Wav_File_Work::Pitch(void)                                              //��������
{
	cout << "TIP : Calculate Pitch ..." << endl;
	int Pmax = Wav_File::Get_SampleRate() / 60;                              //���������
	int Pmin = Wav_File::Get_SampleRate() / 500;                             //���������
	if (dataPitch) {                                                         //���ԭ�����������Ϊ�����ƻ�
		free(dataPitch);                                                     //����ԭ��������
	}
	dataPitch = (int*)malloc(sizeof(int)*frameNumber);                       //Ϊ���ݷ���ռ�
	if (dataPitch == NULL) {
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //ÿһ֡��Ҫ�ҵ�һ�����ֵ
		unsigned long max = 0;
		for (int j = Pmin; j < Pmax; ++j) {                                  //���Ʋ��ҷ�Χ
			if (dataCeps[i][j] >= dataCeps[i][max]) {                        //�ҵ����ݵ����ֵ
				max = j;
			}
		}
		dataPitch[i] = max;
	}
	return true;
}

bool Wav_File_Work::MFCC(void)                                               //÷������ϵ��
{
	cout << "TIP : Calculate MFCC ..." << endl;
	if (frameNumber < 1) {                                                   //��֡ʱ�Ľ�������
		throw invalid_argument("ERROR : No frame !");
		return false;
	}

	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //�������ݵ��������飬����ԭ������
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);                                   //��֡���ݽ���FFT
	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = pow(tempData[i][j], 2);                         //ƽ������
		}
	}

	double **temp_dataMfcc = DistributionSpace(frameNumber, Wav_File_Work::FilterNumber);        //Ϊ���ݷ���ռ�

	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (int m = 0; m < Wav_File_Work::FilterNumber; ++m) {              //ÿ������ͨ��FilterNumber�������˲���
			temp_dataMfcc[i][m] = 0;
			for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {           //�ۼӺͼ�������
				temp_dataMfcc[i][m] += tempData[i][j] * this->Mel_Filter(j, m);
			}
		}
	}
	ReleaseSpace(tempData, frameNumber);

	dataMfcc = DistributionSpace(frameNumber, Wav_File_Work::CoefficientOrder);        //Ϊ���ݷ���ռ�

	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (int n = 0; n < Wav_File_Work::CoefficientOrder; ++n) {          //��ΪCoefficientOrder������
			dataMfcc[i][n] = 0;
			for (int j = 0; j < Wav_File_Work::FilterNumber; ++j) {          //��ʽ��ȡmfcc
				double tempdataA = fabs(temp_dataMfcc[i][j]);
				double tempdataB = cos((n + 1)*Wav_File_Work::PI*(j - 0.5) / Wav_File_Work::FilterNumber);
				if (_finite(log(tempdataA)* tempdataB) == 0) {
					continue;
				}
				dataMfcc[i][n] += log(tempdataA)* tempdataB;
			}
			dataMfcc[i][n] *= sqrt((long double)2 / Wav_File_Work::FilterNumber);
		}
	}

	ReleaseSpace(temp_dataMfcc, frameNumber);
	return true;
}

bool Wav_File_Work::CepstrumDistance(void)                                   //���㵹�׾��루����ע��-����ǰ�����ȼ���mel����ϵ��MFCC��
{
	cout << "TIP : Calculate Cepstrum Distance ..." << endl;
	dataCepsDistance = DistributionSpace(frameNumber);                       //Ϊ���׾������ݷ���ռ�
	double *tempCeps = DistributionSpace(Wav_File_Work::CoefficientOrder);   //Ϊ�������ݷ���ռ�
	for (int i = 0; i < Wav_File_Work::CoefficientOrder; ++i) {              //ȡǰ5֡���㱳������ϵ��
		tempCeps[i] = 0;
		for (int j = 0; j < 5; ++j) {
			tempCeps[i] += dataMfcc[j][i];
		}
		tempCeps[i] /= 5;
	}

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //��֡���㵹�׾���
		dataCepsDistance[i] = 0;
		for (int j = 0; j < Wav_File_Work::CoefficientOrder; ++j) {
			dataCepsDistance[i] += pow(dataMfcc[i][j] - tempCeps[j], 2);
		}
		dataCepsDistance[i] = 4.3429*sqrt(pow(dataMfcc[i][0] - tempCeps[0], 2) + 2 * dataCepsDistance[i]);
	}
	return true;
}

bool Wav_File_Work::EnergySpectrum(void)                                     //�����ʱ����
{
	cout << "TIP : Calculate Energy Spectrum ..." << endl;
	dataEngChart = DistributionSpace(frameNumber);
	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //�������ݵ��������飬����ԭ������
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);                                   //��֡���ݽ���FFT
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataEngChart[i] = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataEngChart[i] += tempData[i][j];
		}
	}

	ReleaseSpace(tempData, frameNumber);
	return true;
}

bool Wav_File_Work::EnergyDeviation(void)                                    //���������׷���
{
	cout << "TIP : Calculate Energy Deviation ..." << endl;
	dataEngDevia = DistributionSpace(frameNumber);
	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //�������ݵ��������飬����ԭ������
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataEngDevia[i] = 0;
		double tempEngChart = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {                //���¼���EngChart����Ϊ��������κδ���EngChart��ֵ�����С
			tempEngChart += fabs(tempData[i][j]);
		}
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataEngDevia[i] += pow(fabs(tempData[i][j]) - tempEngChart / Wav_File_Work::N, 2);
		}
	}

	ReleaseSpace(tempData, frameNumber);
	return true;
}

bool Wav_File_Work::SpectrumEntropy(void)                                    //��������
{
	cout << "TIP : Calculate Spectrum Entropy ..." << endl;
	this->AutocorrelativeFunction();                                         //��ÿһ֡�Ķ�ʱ����غ���
	this->FrameFFT(dataAutoFun, frameNumber);                                //�Զ�ʱ����غ������п��ٸ���Ҷ�任���õ�����֡�Ķ�ʱ�������ܶ�
	this->PSDF();                                                            //������һ֡�е�ÿ��Ƶ�ʷ����Ĺ�һ���������ܶȺ���
	if (dataSpectrumEnt) {
		free(dataSpectrumEnt);
	}
	dataSpectrumEnt = DistributionSpace(frameNumber);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataSpectrumEnt[i] = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N / 2; ++j) {
			dataSpectrumEnt[i] += dataPSDF[i][j] * log(dataPSDF[i][j]);      //���ݹ�һ���Ĺ������ܶȣ�����������Ϣ��
		}
		dataSpectrumEnt[i] *= -1;
	}
	return true;
}

bool Wav_File_Work::Endpoint_Detection(void)                                 //is a voise end point check function
{
	int statusFlag = 0;                                                      //��������״̬��־
	unsigned long begin = 0;                                                 //������������
	unsigned long end = 0;                                                   //����������յ�
	unsigned long voiceLength = 0;                                           //��������ĳ���

	unsigned long suppose_V_Length = 0;                                      //����״̬�����������ĳ���
	unsigned long burse_V_Length = 0;                                        //����״̬�����������ĳ���
	bool voiceJudge = true;                                                  //���Ϊtrue�����������������������Ϊfalse����˵���������仹û�н����������յ��ж�

	voiceNumber = 0;

	statusFlag = _PARAGRAPH_MUTE;                                            //��ʼʱ�������ھ�����

	//����voice�ı�׼��
	double *voice = this->DistributionSpace(frameNumber);                    //Ϊvoice����ռ�
	this->FrameVoice(voice);

	for (unsigned long i = 0; i < frameNumber; ++i) {
		FeatureParameter feature(voice[i], this->dataEngChart[i], this->dataCepsDistance[i],
			this->dataEngDevia[i], this->dataSpectrumEnt[i]);

		switch (statusFlag)
		{
		case _PARAGRAPH_MUTE:
			statusFlag = this->Mute_State_Process(feature);
			//1) => _PARAGRAPH_MUTE

			//2) => _PARAGRAPH_INTERIM
			if (statusFlag == _PARAGRAPH_INTERIM) {                          //����ת����׼���׶�
				suppose_V_Length = 0;                                        //��ʼ��׼���׶εĳ���ʱ�䳤��
			}
			break;
		case _PARAGRAPH_INTERIM:
			statusFlag = this->Interim_State_Process(feature, suppose_V_Length);
			//1) => _PARAGRAPH_MUTE

			//2) => _PARAGRAPH_INTERIM
			++suppose_V_Length;                                              //��������״̬�������׼��ʱ��ĳ���

			//3) => _PARAGRAPH_VOICE
			if (statusFlag == _PARAGRAPH_VOICE) {                            //����ת���������׶�
				begin = i - suppose_V_Length;                                //���׼���׶ο�ʼʱ��֡����Ϊ���
			}
			break;
		case _PARAGRAPH_VOICE:
			voiceJudge = false;                                              //������ʼ��
			statusFlag = this->Voice_State_Process(feature);
			//1) => _PARAGRAPH_VOICE
			++voiceLength;
			//2) => _PARAGRAPH_BURST
			if (statusFlag == _PARAGRAPH_BURST) {                            //������ת������������Ϊ���㱬�����ĳ�����׼��
				burse_V_Length = 0;
			}
			break;
		case _PARAGRAPH_BURST:
			statusFlag = this->Burst_State_Process(feature, burse_V_Length);
			//1�� => _PARAGRAPH_BURST
			++burse_V_Length;
			//2�� => _PARAGRAPH_VOICE

			//3�� => _PARAGRAPH_MUTE
			//this is a end point
			if (statusFlag == _PARAGRAPH_MUTE) {
				end = i - burse_V_Length;
				voiceLength = end - begin;
				VoiceParagraph voiceTemp(begin, end, voiceLength);
				this->voiceParagraph.push_back(voiceTemp);                   //������һ�ε�
				this->voiceNumber++;
				voiceJudge = true;
			}
			break;
		default:break;                                                       //�����ܵ����,����
		}
	}
	if (!voiceJudge) {
		VoiceParagraph voiceTemp(begin, frameNumber, frameNumber - begin);
		this->voiceParagraph.push_back(voiceTemp);
		this->voiceNumber++;
	}
	return true;
}

