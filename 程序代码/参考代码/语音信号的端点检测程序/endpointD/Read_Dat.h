//	�������ܣ�	�ӣ�dat�ļ��ж������ж��پ仰
short Read_Sentence_Amount(char *File_Name);
//	�������ܣ�	�ӣ�dat�ļ��ж���һ�仰�Ĳ�����
long Read_Wave_Dat(char *File_Name,short Sentence_Num,short *Wave_Dat);

//		�������ܣ�	�ӣ�dat�ļ��ж���һ�仰������
long Read_Dat(char *File_Name,short Sentence_Num,float **Feature,short Feature_Order);
//		�������ܣ�	�ӣ�dat�ļ��ж���һ�仰������֡���������ֽ���
long Read_Length(char *File_Name,short Sentence_Num);