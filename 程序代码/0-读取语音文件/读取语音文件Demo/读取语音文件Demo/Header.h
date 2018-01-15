#pragma once
#include <iostream>
#include <windows.h>
using namespace std;

class Wav_File
{
private:	
	FILE *fp;
	char *fileName;
	char id_RIFF[5];
	unsigned long fileSize;
	char id_WAVE[5];
	char id_FMT[5];
	unsigned long formatLength;
	short formatTag;
	short channelsNumber;
	unsigned long sampleRate;
	unsigned long secondBytes;
	short chunkAlign;
	short sampleBits;
	short appendInformation;

	char appendFlag[5];
	unsigned long appendLength;
	unsigned long appendNone;

	char id_DATA[5];
	unsigned long dataSize;
	char *data;
	long *dataTuple;
	unsigned long dataNumber;
	unsigned long dataMax;
	unsigned long dataMin;

	bool Conversion_Tuple(void){
		try 
		{
			dataMax = 0;
			dataMin = 1000000;
			dataNumber = dataSize * 8 / sampleBits;
			dataTuple = (long*)malloc(sizeof(long)*dataNumber);
			long tempNum = 0;
			for (int i = 0; i < dataNumber; ++i) {
				for (int j = 0; j < sampleBits / 8; ++j) {
					tempNum = MAKEWORD(data[i*sampleBits / 8 + j], tempNum);
				}
				dataTuple[i] = tempNum;
				tempNum = 0;
				if (dataMax < dataTuple[i]) {
					dataMax = dataTuple[i];
				}
				if (dataMin > dataTuple[i]) {
					dataMin = dataTuple[i];
				}
			}
		}
		catch (invalid_argument &e) {
			cerr << e.what() << endl;
			return false;
		}
		return true;
	}
public:
	Wav_File(FILE *f) {
		fp = f;
	}
	~Wav_File(){
		exit(0);
	}
	bool Read_File() {
		try
		{
			fread(id_RIFF, sizeof(char), 4, fp);                                   //��ȡ'RIFF'
			if (strcmp(id_RIFF, "RIFF")) {
				throw invalid_argument("ERROR : File not RIFF file");
			}
			id_RIFF[4] = 0;                                                        //ĩβ����
			fread(&fileSize, sizeof(unsigned long), 1, fp);                        //��ȡ�ļ��Ĵ�С
			fread(id_WAVE, sizeof(char), 4, fp);                                   //��ȡ'WAVE'
			id_WAVE[4] = 0;
			if (strcmp(id_WAVE, "WAVE")) {
				throw invalid_argument("ERROR : File not WAVE file");
			}
			fread(id_FMT, sizeof(char), 4, fp);                                    //��ȡ'FMT'
			id_FMT[3] = 0;
			fread(&formatLength, sizeof(unsigned long), 1, fp);                    //
			fread(&formatTag, sizeof(short), 1, fp);                               //��ȡ�ļ���ǩ
			fread(&channelsNumber, sizeof(short), 1, fp);                          //��ȡͨ����Ŀ
			fread(&sampleRate, sizeof(unsigned long), 1, fp);                      //��ȡ����Ƶ��
			fread(&secondBytes, sizeof(unsigned long), 1, fp);                     //��ȡÿ��������
			fread(&chunkAlign, sizeof(short), 1, fp);                              //��ȡ�����
			fread(&sampleBits, sizeof(short), 1, fp);                              //��ȡ������С
			if (formatLength > 16) {
				fread(&appendInformation, sizeof(short), 1, fp);                   //��ȡ������Ϣ
			}

			//���ӿ�
			/*
			fread(appendFlag, sizeof(char), 4, fp);
			appendFlag[4] = 0;
			fread(&appendLength, sizeof(unsigned long), 1, fp);
			fread(&appendNone, sizeof(unsigned long), 1, fp);
			*/

			fread(id_DATA, sizeof(char), 4, fp);                                   //��ȡ'DATA'
			id_DATA[4] = 0;
			fread(&dataSize, sizeof(unsigned long), 1, fp);                        //��ȡ���ݴ�С
			data = (char*)malloc(sizeof(char)*dataSize);                           //�������ݵĴ洢�ռ�
			fread(data, sizeof(char), dataSize, fp);                               //��ȡ����
		}
		catch (invalid_argument &e)
		{
			cerr << e.what() << endl;
			return false;
		}
		cout << "TIP : File read ok !" << endl;
		if (Conversion_Tuple() == true) {
			cout << "TIP : Data change OK !" << endl;
		}
		else {
			return false;
		}
		
		return true;
	}

	void ShowData(void) {
		cout << id_RIFF << endl;
		cout << fileSize << endl;
		cout << id_WAVE << endl << endl;

		cout << id_FMT << endl;
		cout << formatLength << endl;
		cout << formatTag << endl;
		cout << channelsNumber << endl;
		cout << sampleRate << endl;
		cout << secondBytes << endl;
		
		cout << chunkAlign << endl;
		cout << sampleBits << endl;
		cout << appendInformation << endl << endl;

		cout << appendFlag << endl;
		cout << appendLength << endl;
		cout << appendNone << endl << endl;

		cout << id_DATA << endl;
		cout << dataSize << endl;

		for (int i = 0; i < dataNumber; ++i) {
			cout << dataTuple[i] << "\t";
		}
		cout << endl;
		cout << "MAX :" << dataMax << endl;
		cout << "MIN :" << dataMin << endl;
	}
};