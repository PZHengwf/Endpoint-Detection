class Wav_File_Work :protected Wav_File
{
private:
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
	static const int Sub_band;												 //�Ӵ�����3
	static const int P;														 //LPC����
};


const int Wav_File_Work::Sub_band = 16;										 //�Ӵ�����
const int Wav_File_Work:: P=14;												 //LPC����


/**********************************************************************
*        �����жϽ��п��ٸ���Ҷ�任�����ݸ����Ƿ�Ϊ2�����η�              *
*		 ����ǣ��򷵻�0�����򷵻ظ�����ӽ�2�������ݵ�����               *
*                          ���ߣ�ղ̫��                                *
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
}


/**********************************************************************
*              �������ݸ�������2��������ʱ������ȱλ�ò���               *
*                  int  i  :	2�������ݵ�����                        *
*				     �������ز�������ݵ��ܸ���						  *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
long Wav_File_Work::Add_Zero(int i)
{
	for (int j = 0; j < Wav_File::Get_dataNumber(); j++)					 //��ԭ�������ݸ�ֵ����������
		New_dataDouble[j] = dataDouble[j];
	for (int j= Wav_File::Get_dataNumber(); j < pow(2, i); j++)			     //��ԭ���������ϲ���
		New_dataDouble[j] = 0;
	return pow(2, i);
}


/**********************************************************************
*      ���ڴ��ҽ��п��ٸ���Ҷ�任������˳���Դﵽ���������˳���      *
* struct compx *s : �Խṹ����ʽ�洢���������ݣ����ٸ���Ҷ�任�Ľ��ͬ�������ڴ˽ṹ���� *
*                   int len : ���ݵ��ܸ���                            *
*                          ���ߣ�ղ̫��                                *
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
}

/**********************************************************************
*                       ���п��ٸ���Ҷ�任                             *
*					���ؿ��ٸ���Ҷ�任������ݸ���                      *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::FFT(void)
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

	double *W = (double *)malloc(sizeof(double)*len/2);						 //������ת����
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
				cw = W[k * len / ulGroupLength], dataFFT[j + k + ulHalfLength];
				c1 = dataFFT[j + k] + cw;
				c2 = dataFFT[j + k] - cw;
				dataFFT[j + k] = c1;
				dataFFT[j + k + ulHalfLength] = c2;
			}
		}
	}
	for (int i = 0; i < len; i++)
		cout << i << " : " << dataFFT[i] << " ";
	return len;																 //���ؿ��ٸ���Ҷ�任���ݵĳ���
}


/**********************************************************************
*                       ����Ƶ������                                   *
*             int  len  :����Ƶ���������ݵĸ���                          *
*				Ƶ���������������� double *Em ��                        *
*				        ����Ƶ�������ĸ���                             *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::spectralEnergy(int len)
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
			//Em[j]+=(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i);	 //������ٸ���Ҷ�任������ݵ�Ƶ������
			Em[j]+=(dataDFT[i]*dataDFT[i])*Hamming_window(k + N - 1 - i);	 //������ɢ����Ҷ�任������ݵ�Ƶ������
		}
	}
	return j;																 //����Ƶ����������
}


/**********************************************************************
*						 ����Ƶ������						             *
*				Ƶ�����ı��������� double *SCm ��                       *
*				int  len :����Ƶ���������ݵĸ���                        *
*					    ����Ƶ�����ĸ���							   *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::spectralCentroid(int len)
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
			//SCm[j]+=(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i)*i;//������ٸ���Ҷ�任������ݵ�Ƶ������
			SCm[j]+=(dataDFT[i]*dataDFT[i])*Hamming_window(k + N - 1 - i)*i; //������ɢ����Ҷ�任������ݵ�Ƶ������
		}
		SCm[j] = SCm[j] / Em[j];
	}
	return j;																 //����Ƶ�����ĸ���
}


/**********************************************************************
*                           �������                                  *
*					int  len :����������ݵĸ���						 *
*					������������ double *BWm ��                     *
*							���ش������								 *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::bandWidth(int len)
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
			//sum+=pow((i-SCm[j]),2)*(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i);//������ٸ���Ҷ�任������ݵĿ��
			sum+=pow((i-SCm[j]),2)*(dataDFT[i]*dataDFT[i])*Hamming_window(k + N - 1 - i);//������ɢ����Ҷ�任������ݵĿ��
		}
		BWm[j] = sqrt(sum / Em[j]);
	}
	
	return j;																 //���ؿ������
}


/**********************************************************************
*                         �����Ӵ�������                               *
*				int  len :�����Ӵ������ȵ����ݵĸ���					   *
*				�Ӵ������ȱ��������� double **SERmi ��                  *
*							�����Ӵ������ȸ���						  *
*                          ���ߣ�ղ̫��                                *
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
}


/**********************************************************************
*                        ��Ƶ���źŹ�һ��                              *
*				 int  k �� ��һ���������±�                            *
* ��һ����Ƶ���źű������µ����� double *Nor_real�� double *Nor_imag �� *
*			*Nor_real ���ʵ�����ݣ�*Nor_imag ����鲿����		      *
*                          ���ߣ�ղ̫��                                *
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
}


/**********************************************************************
*							�����Ӵ�������							  *
*					int  len�������Ӵ������������ݵĸ���                *
*					�Ӵ������������� double **SFmi ��                  *
*							�����Ӵ�����������                         *
*                          ���ߣ�ղ̫��                                *
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
}


/**********************************************************************
*							����Ƶ�׹�����                             * 
*					int  len :����Ƶ�׹���������ݵĸ���				  *	
*                          ��������㲻����                            * 
*                          ���ߣ�ղ̫��                                *
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
*      ��������Ԥ��ϵ����LPCϵ�� �Լ�����G��Ԥ�����E��ƽ����             *
* ����Ԥ��ϵ������������ double **Ai �У�����G���������� double *err ��  *
*						����LPCϵ������								  *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::LPC_Levinson_Durbin(void)
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
	return n;																 //��������Ԥ��ϵ������
}


/**********************************************************************
*                       ���ж�ʱ����Ҷ�任                             *
*			��ʱ����Ҷ�任��������ڽṹ��struct compx *Xn ��			 *
*					���ض�ʱ����Ҷ�任������ݸ���			              *
*                          ���ߣ�ղ̫��                                *
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
	return n;																 //���ض�ʱ����Ҷ�任���ݸ���
}


/**********************************************************************
*                ��������Ԥ��ϵ����������Ԥ����                         *
*				 LPC��Ϊ�����ף�Ҳ��ΪLPC����			 		          *
*              ����Ԥ���ױ����ڽṹ�� struct compx *H ��               *
*							����LPC�׸���							  *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::LPC(void)
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
	return n;																 //��������Ԥ���׸���
}


/**********************************************************************
*           ��������Ԥ��ϵ����������Ԥ�⵹��ϵ������LPCCϵ��               *
*					LPCCϵ������������ double **h ��                   *
*							����LPCCϵ������							  *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::LPCC_Coefficien(void)
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
	return n;																 //��������Ԥ�⵹��ϵ������
}



/**********************************************************************
*        ��������Ԥ�⵹��ϵ������LPCCϵ����������Ԥ�⵹�ס���LPCC��          *
*				LPCC��Ϊ�����ף�����Ҳ��ΪLPCC������                    *
*			����Ԥ�⵹�ױ����ڽṹ�� struct compx *h_Spectrum��         *
*						����LPCC�׸���								  *
*                          ���ߣ�ղ̫��                                *
**********************************************************************/
int Wav_File_Work::LPCC(void)
{
	int i, n;
	h_Spectrum = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber() - N);//ΪDouble�����������ڴ�ռ�
	if (h_Spectrum == NULL) {												 //����ռ�δ�ɹ�
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		h_Spectrum[n] = 0;
		for (int k = 0; k <= P; k++)
		{
			h_Spectrum[n] += h[n][k] * cos(2 * PI*i*k / P);
		}
	}
	return n;																 //��������Ԥ�⵹�׸���
}

void Wav_File_Work::ShowData(void)                                           //���Ǹ����չʾ���ݺ���
{
	//DFT();
	//FFT();
	//spectralEnergy(Wav_File::Get_dataNumber());
	//spectralCentroid(Wav_File::Get_dataNumber());							   //Ƶ������
	//bandWidth(Wav_File::Get_dataNumber());								   //���
	//Subband_Energy_Ratio(Wav_File::Get_dataNumber());                        //�Ӵ�������
	//Subband_Spectral_Flux(Wav_File::Get_dataNumber());					   //�Ӵ�������
	//Spectral_Rolloff_Point(Wav_File::Get_dataNumber());					   //Ƶ�׹�����
	//LPC_Levinson_Durbin();												   //����Ԥ��ϵ��
	STFT();																	 //��ʱ����Ҷ�任
	/*
	LPC();																	 //����Ԥ����
	LPCC_Coefficien();														 //����Ԥ�⵹��ϵ��
	LPCC();																	 //����Ԥ�⵹��
	*/
}