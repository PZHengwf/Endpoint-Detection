#include "stdafx.h"
#include "Wav_File_Handle.h"

const int Wav_File_Work::N = 256;                                            //��ʼ��ÿ�����Ĵ���
const int Wav_File_Work::FrameShift = 10;                                    //��ʼ����������֡��
const double Wav_File_Work::PI = 3.14159;                                    //��ʼ��Բ����
const int Wav_File_Work::Sub_band = 16;										 //�Ӵ�����
const int Wav_File_Work:: P=14;										 //LPC����



/**********************************************************************
*                ���������ֽ�����ת��ΪDouble����                       *
**********************************************************************/
bool Wav_File_Work::Conversion_Double(void)                                  //���������ֽ�����ת��ΪDouble����
{
	try
	{
		const double Flag = pow((double)2, Wav_File::Get_ChunkAlign() * 8-1);//��ʾ����ԭ�������ֵ��һ��
		for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {     //����ÿ������
			dataDouble[i] = (double)Wav_File::Get_Data(i) / Flag-1;          //����ÿ��������[-1,1]֮��
		}
	}
	catch (invalid_argument &e){
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}



/**********************************************************************
*                Ƶ�ײ�����һ�������ڽ����ݿ�����[-1��1]֮��             *
*					double  *a   :��һ��������						  *
**********************************************************************/
bool Wav_File_Work::Conversion_New(double *a,int n)
{
	try
	{

		double max = 0, min = 0;
		for (int i = 0; i<n; i++)
		{
			if (abs(a[i])>max)
				max = abs(a[i]);
			if (a[i] < min)
				min = a[i];
		}
		for (unsigned long i = 0; i < n; ++i) {     //����ÿ������
			a[i] = a[i]/max;          //����ÿ��������[-1,1]֮��
		}
	}
	catch (invalid_argument &e){
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;


}

/**********************************************************************
*                �������������������ݼӴ�����                           *
*             double data   : �Ӵ�����ʱ������λ��                      *
**********************************************************************/
double Wav_File_Work::Hamming_window(double data)                            //����������
{
	if (data >= 0 && data <= (N - 1)) {                                      //0<= n <= N-1�����
		return 0.54 - 0.46*cos(2 * PI*data / (N - 1));                       //������ֵ
	}
	else {
		return 0;
	}
}




/**********************************************************************
*                         �����Ӵ�������                               *
*				int  len :�����Ӵ������ȵ����ݵĸ���					   *
*				�Ӵ������ȱ��������� double **SERmi ��                  *
*							�����Ӵ������ȸ���						  *
**********************************************************************/
int Wav_File_Work::Subband_Energy_Ratio(int len)
{
	int i, n;
	double f = 0;
	SERmi = (double**)malloc(sizeof(double)*(len - N));						 //Ϊ�Ӵ������������ڴ�ռ�
	if (SERmi == NULL) {													 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < len - N; i += Wav_File_Work::FrameShift, n++)
	{
		SERmi[n] = (double*)malloc(Sub_band*sizeof(double));
		double sum = 0;
		double p = N / Sub_band;											 //Sun_bandΪ���ֵ��Ӵ�����
		if ((int)p != p)
		{
			p = (int)p + 1;
		}
		f = p;
		int j = i, k = 0;
		for (; k < 15; k++)
		{
			for (; j <p + i; j++)
			{
				//sum += (dataFFT[i]*dataFFT[i])*Hamming_window(i + N - 1 - j);//������ٸ���Ҷ�任������ݵ��Ӵ�������
				sum += (dataDFT[i]*dataDFT[i])*Hamming_window(i + N - 1 - j);//������ɢ����Ҷ�任������ݵ��Ӵ�������
			}
			SERmi[n][k] = sum / Em[n];
			p += f;
			if (p>len)
				p = len;
		}
	}
	return n;																 //�����Ӵ������ȸ���
}																			 //ղ̫��


/**********************************************************************
*                        ��Ƶ���źŹ�һ��                              *
*				 int  k �� ��һ���������±�                            *
* ��һ����Ƶ���źű������µ����� double *Nor_real�� double *Nor_imag �� *
*			*Nor_real ���ʵ�����ݣ�*Nor_imag ����鲿����		      *
**********************************************************************/
void Wav_File_Work::Normalized_Spectral_Signal(int k)
{
	double sum = 0;
	for (int n = k; n < k + N; n++)
	{
		//Nor_dataDouble[n] = 10 * log(dataFFT[n])/log(10);					 //���ٸ���Ҷ�任֮��Ƶ���źŵĹ�һ��
		Nor_dataDouble[n] = 10 * log(dataDFT[n] * dataDFT[n]) / log(10);	 //��ɢ����Ҷ�任֮��Ƶ���źŵĹ�һ��
		sum += (Nor_dataDouble[n] * Nor_dataDouble[n]) * Hamming_window(k + N - 1 - n);

	}
	Nor_dataDouble[k] = Nor_dataDouble[k] / sqrt(sum);
}																			 //ղ̫��


/**********************************************************************
*							�����Ӵ�������							  *
*					int  len�������Ӵ������������ݵĸ���                *
*					�Ӵ������������� double **SFmi ��                  *
*							�����Ӵ�����������                         *
**********************************************************************/
int Wav_File_Work::Subband_Spectral_Flux(int len)
{
	int i, n;
	double f = 0;
	Nor_dataDouble = (double*)malloc(sizeof(double)*(len - N));				 //Ϊ��һ��ʵ�����������ڴ�ռ�
	if (Nor_dataDouble == NULL) {											 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	SFmi = (double**)malloc(sizeof(double)*(len - N));						 //Ϊ�Ӵ������������ڴ�ռ�
	if (SFmi == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i <len - N; i += Wav_File_Work::FrameShift, n++)
	{
		SFmi[n] = (double*)malloc(Sub_band*sizeof(double));
		
		double p = N / Sub_band;											 //Sun_bandΪ���ֵ��Ӵ�����
		if ((int)p != p)
		{
			p = (int)p + 1;
		}
		f = p;
		int j = i, k = 0;
		for (; k < 15; k++)
		{
			double sum = 0;
			int l = j;
			for (; j <p + i; j++)											 //j��ʾ��ÿһ���Ӵ���Ƶ�����޵����޵�ѭ������
			{
				Normalized_Spectral_Signal(j);
				Normalized_Spectral_Signal(j + 1);
				sum += (Nor_dataDouble [j + 1] - Nor_dataDouble[j])*Hamming_window(i + N - 1 - j);
			}
			SFmi[n][k] = sum / (p + i - l);
			p += f;
			if (p>len)
				p = len;
		}
	}
	return n;																 //�����Ӵ�����������
}																			 //ղ̫��


/**********************************************************************
*							����Ƶ�׹�����                             * 
*					int  len :����Ƶ�׹���������ݵĸ���				  *	
*                          ��������㲻����                            * 
**********************************************************************/
/*int Wav_File_Work::Spectral_Rolloff_Point(int len)																					
{
	double sum1, sum2;
	int i, j;
	RPm = (double*)malloc(sizeof(double)*(len - N));						 //ΪƵ�׹����������ڴ�ռ�
	if (RPm == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0,j=0; i < len-N ; i+=Wav_File_Work::FrameShift,j++)
	{
		sum1 = 0, sum2 = 0;
		
		for (int n = i; n < i + N; n++)
		{
			sum1 += dataDFT[n];
		}
		for (int n = i; n < i + N; n++)
		{
			sum2 +=real[n];
			if (sum2 == 0.85*sum1)
			{
				RPm[j] = n;
				break;
			}
		}
	}
	return j;																 //����Ƶ�׹��������
}*/																			 //ղ̫��




/**********************************************************************
*                       ���ж�ʱ����Ҷ�任                             *
*			��ʱ����Ҷ�任������������� double *Xn ��			          *
*					���ض�ʱ����Ҷ�任������ݸ���			              *
**********************************************************************/
int Wav_File_Work::STFT(void)
{
	int i, n;
	Xn= (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber()-N);		 //Ϊ��ʱ����Ҷ�任���������ڴ�ռ�
	if (Xn == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		Xn[n] = 0;
		for (int k = i; k < i + N; k++)
		{
			Xn[n]+= dataDouble[k] * Hamming_window(i + N - 1 - k)*cos(2 * PI*k*i / N);
		}
	}
	Conversion_New(Xn, n);
	XnNumber = n;
	return n;																 //���ض�ʱ����Ҷ�任���ݸ���
}																			 //ղ̫��

double Wav_File_Work::Get_XnNumber(void)
{
	return XnNumber;

}
double Wav_File_Work::Get_Xn(int n)
{
	if (n>= (Wav_File_Work::Get_XnNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return Xn[n];
	}
		
}


/**********************************************************************
*                         ��ɢ����Ҷ�任                               *
*              �任������������� double *dataDFT ��                    *
**********************************************************************/
bool Wav_File_Work::DFT(void)                                                //������ɢ����Ҷ�任�����鲿
{
	double exp;                                                              //����ָ������
	//free(dataDFT);                                                           //����ԭ��������
	dataDFT = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());    //Ϊ�����ݷ���ռ�
	if (dataDFT == NULL){                                                    //����ʧ�ܵ����
		return false;
	}
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i){          //һ��һ�������µ�����
		dataDFT[i] = 0;                                                      //Ԥ��������
		exp = -2 * PI*i / Wav_File::Get_dataNumber();                        //ָ������
		for (unsigned long j = 0; j < Wav_File::Get_dataNumber(); ++j){      //���ۼӺ�
			dataDFT[i] += dataDouble[j] * cos(j*exp);                        //��ʵ�����ֵ�����
		}
	}
	Conversion_New(dataDFT, Wav_File::Get_dataNumber());
	dataDFTNumber = Wav_File::Get_dataNumber();
	return true;
}

double Wav_File_Work::Get_dataDFTNumber()
{
	return dataDFTNumber;
}

double Wav_File_Work::Get_dataDFT(int n)
{
	if (n >= (Wav_File_Work::Get_dataDFTNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataDFT[n];
	}
}

bool Wav_File_Work::IDFT(double *data, unsigned long dataNumber)             //������ɢ����Ҷ�任�����鲿
{
	double exp;                                                              //����ָ������
	double *dataIDFT;
	dataIDFT = (double*)malloc(sizeof(double)*dataNumber);                   //Ϊ�����ݷ���ռ�
	if (dataIDFT == NULL) {                                                  //����ʧ�ܵ����
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}
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
	Conversion_New(data, dataNumber);
	 dataIDFTNumber= dataNumber;
	free(dataIDFT);
	return true;
}

double Wav_File_Work::Get_dataIDFTNumber(void)
{
	return dataIDFTNumber;

}
double Wav_File_Work::Get_dataIDFT(int n)
{
	if (n >= (Wav_File_Work::Get_dataIDFTNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataIDFT[n];
	}
}


/**********************************************************************
*        �����жϽ��п��ٸ���Ҷ�任�����ݸ����Ƿ�Ϊ2�����η�              *
*		 ����ǣ��򷵻�0�����򷵻ظ�����ӽ�2�������ݵ�����               *
**********************************************************************/
int Wav_File_Work::Is_Pow2(void)
{
	double i;
	double f = Wav_File::Get_dataNumber();
	int j;																	 //������ݸ�����2��������
	for (j = 1; (f = f / 2) != 1 && f>1; j++);
	if (pow(2, j) - Wav_File::Get_dataNumber() == 0)
		return 0;
	else
		return j;
}																			 //ղ̫��


/**********************************************************************
*              �������ݸ�������2��������ʱ������ȱλ�ò���               *
*                  int  i  :	2�������ݵ�����                        *
*				     �������ز�������ݵ��ܸ���						  *
**********************************************************************/
long Wav_File_Work::Add_Zero(int i)
{
	for (int j = 0; j < Wav_File::Get_dataNumber(); j++)					 //��ԭ�������ݸ�ֵ����������
		New_dataDouble[j] = dataDouble[j];
	for (int j = Wav_File::Get_dataNumber(); j < pow(2, i); j++)			     //��ԭ���������ϲ���
		New_dataDouble[j] = 0;
	return pow(2, i);
}																			 //ղ̫��


/**********************************************************************
*      ���ڴ��ҽ��п��ٸ���Ҷ�任������˳���Դﵽ���������˳���      *
* struct compx *s : �Խṹ����ʽ�洢���������ݣ����ٸ���Ҷ�任�Ľ��ͬ�������ڴ˽ṹ���� *
*                   int len : ���ݵ��ܸ���                            *
**********************************************************************/
void Wav_File_Work::Reverse_dataDouble(double *data, int len)				 //�׵��㷨������������
{

	long i, k, j = 0;
	double t;
	long nv2 = len / 2;
	long nm1 = len;
	for (i = 0; i < nm1 - 1; i++)
	{
		if (i < j)															 //�������ݽ���
		{
			t = data[j];
			data[j] = data[i];
			data[i] = t;
		}
		k = nv2;
		while (k <= j)
		{
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}
}																			 //ղ̫��

/**********************************************************************
*                       ���п��ٸ���Ҷ�任                             *
*					���ؿ��ٸ���Ҷ�任������ݸ���                      *
**********************************************************************/
bool Wav_File_Work::FFT(void)
{

	int l;																	 //�������
	long len;																 //������ݳ���
	if (Is_Pow2())
	{
		New_dataDouble = (double*)malloc(sizeof(double)*pow(2, Is_Pow2()));  //Ϊ�µ���������ռ䣬�ռ��СΪ����֮��Ŀռ䳤��
		len = Add_Zero(Is_Pow2());											 //����Ϊ���������鳤��
	}
	else
	{
		New_dataDouble = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());//Ϊ�µ���������ռ䣬�ռ��СΪԭ����ռ䳤��
		New_dataDouble = dataDouble;										 //��ԭ�������ݸ�ֵ��������
		len = Wav_File::Get_dataNumber();								     //����Ϊԭ���鳤��
	}

	int f = len;
	dataFFT = (double *)malloc(sizeof(double)*len);							 //Ϊ���ٸ���Ҷ�任����������ռ�
	if (dataFFT == NULL) {													 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (int i = 0; i < len; i++)
	{
		dataFFT[i] = New_dataDouble[i];										 //��ʼ�����ٸ���Ҷ�任����
	}

	for (l = 1; (f = f / 2) != 1; l++);										 //�õ��������ܳ�������
	Reverse_dataDouble(dataFFT, len);										 //��������

	double *W = (double *)malloc(sizeof(double)*len / 2);						 //������ת����
	for (unsigned long i = 0; i < len / 2; i++)
	{
		W[i] = cos(2 * i * PI / len);
	}

	unsigned long ulGroupLength = 1;										 //�εĳ���
	unsigned long ulHalfLength = 0;											 //�γ��ȵ�һ��
	unsigned long ulGroupCount = 0;											 //�ε�����
	double cw, c1, c2;
	for (unsigned long b = 0; b < l; b++)									 //����FFT
	{
		ulHalfLength = ulGroupLength;
		ulGroupLength *= 2;
		for (unsigned long j = 0; j < len; j += ulGroupLength)
		{
			for (unsigned long k = 0; k < ulHalfLength; k++)
			{
				cw  = W[k * len / ulGroupLength] * dataFFT[j + k + ulHalfLength];
				c1 = dataFFT[j + k] + cw;
				c2 = dataFFT[j + k] - cw;
				dataFFT[j + k] = c1;
				dataFFT[j + k + ulHalfLength] = c2;
			}
		}
	}
	Conversion_New(dataFFT, len);
	dataFFTNumber = len;
	return true;
																	 //���ؿ��ٸ���Ҷ�任���ݵĳ���
}																			 //ղ̫��

double Wav_File_Work::Get_dataFFTNumber(void)
{
	return dataFFTNumber;
}


double Wav_File_Work::Get_dataFFT(int n)
{
	if (n >= (Wav_File_Work::Get_dataFFTNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataFFT[n];
	}

}


/**********************************************************************
*                       ����Ƶ������                                   *
*             int  len  :����Ƶ���������ݵĸ���                          *
*				Ƶ���������������� double *Em ��                        *
*				        ����Ƶ�������ĸ���                             *
**********************************************************************/
bool Wav_File_Work::spectralEnergy(int len)
{
	int j, k;
	Em = (double*)malloc(sizeof(double)*(len - N));							 //ΪƵ�����������ڴ�ռ�
	if (Em == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (k = 0, j = 0; k < len - N; k += Wav_File_Work::FrameShift, j++)
	{
		Em[j] = 0;
		for (int i = k; i < k + N; i++)
		{
			Em[j]+=(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i)*i;	 //������ٸ���Ҷ�任������ݵ�Ƶ������
			//Em[j] += (dataDFT[i] * dataDFT[i])*Hamming_window(k + N - 1 - i);	 //������ɢ����Ҷ�任������ݵ�Ƶ������
		}
	}
	//Conversion_New(Em, j);
	EmNumber = j;
	return true;																 //����Ƶ����������
}																			 //ղ̫��

double Wav_File_Work::Get_EmNumber(void)
{

	return EmNumber;
}

double Wav_File_Work::Get_Em(int n)
{
	if (n >= (Wav_File_Work::Get_EmNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return Em[n];
	}
}


/**********************************************************************
*						 ����Ƶ������						          *
*				Ƶ�����ı��������� double *SCm ��                      *
*				int  len :����Ƶ���������ݵĸ���                       *
*					    ����Ƶ�����ĸ���								  *
**********************************************************************/
bool Wav_File_Work::spectralCentroid(int len)
{
	int j, k;
	SCm = (double*)malloc(sizeof(double)*(len - N));						 //ΪƵ�����������ڴ�ռ�
	if (SCm == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (k = 0, j = 0; k < len - N; k += Wav_File_Work::FrameShift, j++)
	{
		SCm[j] = 0;
		for (int i = k; i < k + N; i++)
		{
			SCm[j]+=(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i)*i;//������ٸ���Ҷ�任������ݵ�Ƶ������
			//SCm[j] += (dataDFT[i] * dataDFT[i])*Hamming_window(k + N - 1 - i)*i; //������ɢ����Ҷ�任������ݵ�Ƶ������
		}
		SCm[j] = SCm[j] / Em[j];
	}
	//Conversion_New(SCm, j);
	SCmNumber = j;
	return true;																 //����Ƶ�����ĸ���
}																			 //ղ̫��

double Wav_File_Work::Get_SCmNumber(void)
{
	return SCmNumber;
}

double Wav_File_Work::Get_SCm(int n)
{
	if (n >= (Wav_File_Work::Get_SCmNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return SCm[n];
	}

}


/**********************************************************************
*                           �������                                  *
*					int  len :����������ݵĸ���						 *
*					������������ double *BWm ��                     *
*							���ش������								 *
**********************************************************************/
bool Wav_File_Work::bandWidth(int len)
{
	int j, k;
	BWm = (double*)malloc(sizeof(double)*(len - N));						 //Ϊ��������ڴ�ռ�
	if (BWm == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (k = 0, j = 0; k < len - N; k += Wav_File_Work::FrameShift, j++)
	{
		double sum = 0;
		for (int i = k; i < k + N; i++)
		{
			sum+=pow((i-SCm[j]),2)*(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i);//������ٸ���Ҷ�任������ݵĿ��
			//sum += pow((i - SCm[j]), 2)*(dataDFT[i] * dataDFT[i])*Hamming_window(k + N - 1 - i);//������ɢ����Ҷ�任������ݵĿ��
		}
		BWm[j] = sqrt(sum / Em[j]);
	}
	Conversion_New(BWm, j);
	BWmNumber = j;
	return true;																 //���ؿ������
}																			 //ղ̫��

double Wav_File_Work::Get_BWmNumber(void)
{
	return BWmNumber;
}

double Wav_File_Work::Get_BWm(int n)
{
	if (n >= (Wav_File_Work::Get_BWmNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return BWm[n];
	}
}



/**********************************************************************
*      ��������Ԥ��ϵ����LPCϵ�� �Լ�����G��Ԥ�����E��ƽ����             *
* ����Ԥ��ϵ������������ double **Ai �У�����G���������� double *err ��  *
*						����LPCϵ������								  *
**********************************************************************/
bool Wav_File_Work::LPC_Levinson_Durbin(void)
{
	double E = 0;															 //Ԥ�����
	double **r = (double **)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));//���ϵ��
	Ai = (double**)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));	 //Ϊ����Ԥ��ϵ�������ڴ�ռ�
	if (Ai == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	err = (double *)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));
	if (err == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	double *p_tmp = (double *)malloc(sizeof(double)*(P + 1));
	double *p_buf = (double *)malloc(sizeof(double)*(P + 1));
	int i, n;
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)	//�źżӴ�,��֡,�������ϵ��
	{
		r[n] = (double*)malloc(sizeof(double)*(P + 1));
		for (int m = 0; m <= P; m++)
		{
			double sum = 0;
			for (int k = i, j = m + i; j < i + N; k++, j++)
			{
				sum += dataDouble[k] * dataDouble[j] * Hamming_window(i + N - 1 - k);
				r[n][m] = sum;
			}
		}
	}
	int j, m, k;
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++) //��������Ԥ��ϵ���Լ�����
	{
		Ai[n] = (double*)malloc(sizeof(double)*(P + 1));
		if (0 == r[n][0])
		{
			for (m = 0; m <= P; m++)
				Ai[n][m] = 0;
			return 0;
		}
		for (int m = 1; m <= P; m++)
		{
			Ai[n][m] = 0;
			p_buf[m] = 0;
		}
		Ai[n][0] = 1.0;
		p_buf[0] = 1;
		err[n] = r[n][0];
		for (m = 0; m <= P; m++)
		{
			double lambda = 0.0;
			for (j = 0; j < m; j++)
			{
				k = m - j;
				if (k < 0)
					k = -k;
				lambda -= Ai[n][j] * r[n][k];
			}
			lambda /= err[n];
			for (j = 1; j <= m; j++)
			{
				p_buf[j] = Ai[n][j] + lambda * Ai[n][m - j];
			}
			err[n] *= (1.0 - lambda*lambda);
			p_tmp = Ai[n];
			Ai[n] = p_buf;
			p_buf = p_tmp;
		}
		err[n] = sqrt(abs(err[n]));											 //����G
	}
	AiNumber = n;
	return true;														     //��������Ԥ��ϵ������
}																			 //ղ̫��


double Wav_File_Work::Get_AiNumber(void)
{
	return AiNumber;
}

double* Wav_File_Work::Get_Ai(int n)
{
	if (n >= (Wav_File_Work::Get_AiNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return NULL;
	}
	else {
		return Ai[n];
	}

}


/**********************************************************************
*                ��������Ԥ��ϵ����������Ԥ����                         *
*				 LPC��Ϊ�����ף�Ҳ��ΪLPC����			 		          *
*              ����Ԥ���ױ����ڽṹ�� struct compx *H ��                *
**********************************************************************/
bool Wav_File_Work::LPC(void)
{
	int i, n;
	H = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber() - N);		 //Ϊ����Ԥ���������ڴ�ռ�
	if (H == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		H[n] = 0;
		for (int j = 1; j <= P; j++)
		{
			H[n] += Ai[n][j] * cos(2 * PI*i*j / P);
		}
		H[n] = 1 - H[n];
		H[n] = err[n] / H[n];
	}
	Conversion_New(H, n);
	HNumber = n;
	return true;																 
}																			 //ղ̫��


double Wav_File_Work::Get_HNumber(void)
{
	return HNumber;
}

double Wav_File_Work::Get_H(int n)
{
	if (n >= (Wav_File_Work::Get_HNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return H[n];
	}
}


/**********************************************************************
*           ��������Ԥ��ϵ����������Ԥ�⵹��ϵ������LPCCϵ��               *
*					LPCCϵ������������ double **h ��                   *
*							����LPCCϵ������							  *
**********************************************************************/
bool Wav_File_Work::LPCC_Coefficien(void)
{
	int i, j, n;
	double **LPCEP = (double **)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));
	h = (double**)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));	 //Ϊ����Ԥ�⵹��ϵ�������ڴ�ռ�
	if (h == NULL) {														 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		LPCEP[n] = (double *)malloc(sizeof(double)*(P + 1));
		h[n] = (double *)malloc(sizeof(double)*(P + 1));
		for (j = 0; j <= P; j++)
			LPCEP[n][j] = 0;
		for (j = 0; j <= P; j++)
		{
			double sum = 0;
			for (int k = 0; k < j; k++)
				sum += (j - k)*Ai[n][k] * LPCEP[n][j - k];
			LPCEP[n][j] = -(Ai[n][j] + sum / (j + 1));
			h[n][j] = LPCEP[n][j];
		}
	}
	hNumber = n;
	return true;																
}																			 //ղ̫��


double Wav_File_Work::Get_hNumber(void)
{
	return hNumber;
}

double* Wav_File_Work::Get_h(int n)
{
	if (n >= (Wav_File_Work::Get_hNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return NULL;
	}
	else {
		return h[n];
	}

}


/**********************************************************************
*        ��������Ԥ�⵹��ϵ������LPCCϵ����������Ԥ�⵹�ס���LPCC��          *
*				LPCC��Ϊ�����ף�����Ҳ��ΪLPCC������                    *
*			����Ԥ�⵹�ױ����ڽṹ�� struct compx *h_Spectrum��         *
*						����LPCC�׸���								  *
**********************************************************************/
bool Wav_File_Work::LPCC(void)
{
	int i, n;
	hSpectrum = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber() - N);//ΪDouble�����������ڴ�ռ�
	if (hSpectrum == NULL) {												 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		hSpectrum[n] = 0;
		for (int k = 0; k <= P; k++)
		{
			hSpectrum[n] += h[n][k] * cos(2 * PI*i*k / P);
		}
	}
	Conversion_New(hSpectrum, n);
	hSpectrumNumber = n;
	return n;																 //��������Ԥ�⵹�׸���
}																			 //ղ̫��


double Wav_File_Work::Get_hSpectrumNumber(void)
{
	return hSpectrumNumber;
}

double Wav_File_Work::Get_hSpectrum(int n)
{
	if (n >= (Wav_File_Work::Get_hSpectrumNumber()) || n < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return hSpectrum[n];
	}
}

void Wav_File_Work::ShowData(void)                                           //���Ǹ����չʾ���ݺ���
{
	//DFT();
	//FFT();
	//spectralEnergy(Wav_File::Get_dataNumber());
	//spectralCentroid(Wav_File::Get_dataNumber());							 //Ƶ������
	//bandWidth(Wav_File::Get_dataNumber());									 //���
	//Subband_Energy_Ratio(Wav_File::Get_dataNumber());						 //�Ӵ�������
	//Subband_Spectral_Flux(Wav_File::Get_dataNumber());						 //�Ӵ�������
	//Spectral_Rolloff_Point(Wav_File::Get_dataNumber());					     //Ƶ�׹�����
	//LPC_Levinson_Durbin();													 //����Ԥ��ϵ��
	STFT();																	 //��ʱ����Ҷ�任
	/*LPC();																	 //����Ԥ����
	LPCC_Coefficien();														 //����Ԥ�⵹��ϵ��
	LPCC();																	 //����Ԥ�⵹��*/
}


