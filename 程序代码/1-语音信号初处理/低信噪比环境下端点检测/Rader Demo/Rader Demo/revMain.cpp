#include <iostream>
using namespace std;

const int N = 8;                                                             //����
int array[N] = { 0, 1, 2, 3, 4, 5, 6, 7 };                                   //��������

int main(void)
{

	int endNum;
	int temp;
	endNum = 0;

	for (int i = 0; i < N - 1; i++)                                          //��Ϊ��һ������Զ��0���䣬���Բ��Ҵ���ΪN-1
	{
		if (i < endNum)                                                      //���˳����С���������򽻻������Ƿ�ֹ�ظ�������һ���ֶΣ���Ϊ���ں�Ҳ��
		{
			temp = array[i];                                                 //��������
			array[i] = array[endNum];
			array[endNum] = temp;
		}

		int k = N >> 1;                                                      //100 ��������һ�룬�����ж����λ�Ƿ�Ϊ1

		while (k <= endNum)                                                  //��������������λΪ1
		{
			endNum = endNum - k;                                             //��ǰλ��0
			k >>= 1;                                                         //����/2
		}

		endNum = endNum + k;                                                 //��ǰλΪ0������תΪ1
	}

	for (int i = 0; i < N; i++)
		printf("%d ", array[i]);
	printf("\n");
	return 0;
}