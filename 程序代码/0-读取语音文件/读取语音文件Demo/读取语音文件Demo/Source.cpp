#include "Header.h"

int main()
{
	FILE *fp;
	if ((fp = fopen("E:\\����\\�Ŷ���Ŀ\\���Ի��ı�����ת��ϵͳ\\����\\΢��Ĭ��.wav", "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
	}
	Wav_File f1(fp);
	if (f1.Read_File() == false) {
		exit(-1);
	}
	f1.ShowData();
	return 0;
}

