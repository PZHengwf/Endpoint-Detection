[wav_file,fs,bits]=wavread('filename.wav');

L=length(wav_file);                    %������Ƶ�źŵĳ���
noise=0.05*randn(L,1);                 %�����ȳ��ȵ���������ź�(����������Ĵ�Сȡ������������ķ��ȱ��������޸���ֵ 
wav_file_handle=wav_file+noise;        %�������źŵ��ӳ�һ���µ��źš�������������

figure(1)
subplot(2,1,1);
plot(wav_file);
title('ԭʼ�����źŲ���');
xlabel('ʱ����')
ylabel('��ֵ')
subplot(2,1,2);
plot(wav_file_handle);
title('ԭʼ�����źż������');
xlabel('ʱ����');
ylabel('��ֵ');

sound(wav_file_handle,fs);
wavwrite(wav_file_handle,fs,bits,'��������-�������.wav');