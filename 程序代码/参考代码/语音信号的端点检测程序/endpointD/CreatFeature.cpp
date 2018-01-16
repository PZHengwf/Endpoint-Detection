/*******************************************************************
****	�ļ���		CreatFeature.cpp			****
****	���ܣ�		������������				****
**** 			 					****
****	�޸ļ�¼��	2001.7.23 ժ��G.732.1          		****
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "Head.h"
#include "Read_Dat.h"
#include "mfcc_xhg.h"



int ExtractFeature(char *filename_in, char *filename_VadOut);
//������ȡ����

void main(int argc,char **argv)
{
/*	argc = 3;
	char	sourcefilename[]	= "E:\\Program_Data\\temp_data\\zx.dat";
	char	 Vadfilename[]		= "E:\\Program_Data\\temp_data\\zxvad.dat";
//*/
	  
//*
	//if(argc!= 3) {
		//printf("Usage: endpointD <sourcefilename.dat> <endpoint.txt>\n");
		//exit(0);
	//}
  char sourcefilename[100],  endpointfilename[100];
  
  strcpy(sourcefilename,argv[1]);
  strcpy(endpointfilename, argv[2]);
//*/
  int returnflag;
  
  returnflag=ExtractFeature(sourcefilename, endpointfilename);
  
  printf("Finished !\n");
}



//��������ExtractFeature
//������������������ļ��� filename_in,����ļ���filename_Cepout
//         ��ӵ��ļ�ͷ�еĶ�����Ϣ
//�����Ƿ�ɹ��ı�־,0Ϊʧ�ܣ�1Ϊ�ɹ�
//˵������ȡLPCC�������ݺ���

int ExtractFeature(char *filename_in, char *filename_Vadout)
{
    FILE *fp_in, *fp_Vadout;
	
	//������������صı���
	short	testflag;			//Ϊ�˸���ʹ�ã������г������Ѿ���ʹ����
	int		TotalNumSentence;	//�ܾ�����
    int		SentenceCount;		//����ѭ��������
	long	SentenceLength_Byte;
	long	datahead; 
	long	lStartPnt, lEndPnt;
	long	lSampleRate=16;
    
	
	//��������ȡ��صı���
	short	*VoiceData_Short	= NULL;//16bit 

	dataHEAD filehead;
	
	//--------------------------------------
	//ȡϵͳ����
	char datebuff[128];
	_tzset();
	_strdate(datebuff);
	//--------------------------------------
	

    fp_in		= fopen(filename_in,"rb");
	fp_Vadout	= fopen(filename_Vadout, "w+");
	if((fp_in==NULL)||(fp_Vadout == NULL))
		return 0;

	fread(&filehead,sizeof(dataHEAD),1,fp_in);
	printf("Now the voice data come from file: %s\n",filename_in);
	printf("Sample Rate: %d    Quantity byte: %d \n", filehead.SampleRate, filehead.SampleType );
	printf("output: %s\n",filename_Vadout);
	
	TotalNumSentence=Read_Sentence_Amount(filename_in);
	
	fprintf(fp_Vadout, "TotalNumSentence= %d\n", TotalNumSentence);
    
	for(SentenceCount=0; SentenceCount<TotalNumSentence; SentenceCount++)
	{
		//-------------------------------------------------------------------------------------
		//�õ�ÿ�����ӵĳ��ȣ���䵽��Ӧ��SentenceLength_Byte,SentenceLength_Short
		//��ȥ��ΪVoiceData_Short��VoiceData_Byte�����������ڴ�
		//ͬʱ�����ݶ��뵽��Ӧ��VoiceData_Short��VoiceData_Byte������
		fseek(fp_in,200+SentenceCount*2*sizeof(long),SEEK_SET);
		fread(&datahead,sizeof(long),1,fp_in);
		fread(&SentenceLength_Byte,sizeof(long),1,fp_in);

		fseek(fp_in,datahead,SEEK_SET);
		VoiceData_Short=(short *)malloc(SentenceLength_Byte);	
		testflag=fread(VoiceData_Short,SentenceLength_Byte,1,fp_in);
	    
		StartEnd(VoiceData_Short, SentenceLength_Byte/2, 
			&lStartPnt, &lEndPnt,
			lSampleRate);

		//����˵���
		fprintf(fp_Vadout, "%ld\t%ld\n", lStartPnt / LPCCFIX_FRAME, lEndPnt / LPCCFIX_FRAME);

		//ɾ���ڴ�ķ���
		if(VoiceData_Short!=NULL)
		{
			free(VoiceData_Short);
			VoiceData_Short=NULL;
		}

		//fprintf(stdout, "Done: %3d\r", SentenceCount);

	}

	//�ر��ļ�
	printf("\n");
	fclose(fp_in);
	fclose(fp_Vadout);
  
	return 1;
}


