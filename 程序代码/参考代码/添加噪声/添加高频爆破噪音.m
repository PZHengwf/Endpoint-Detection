[wav_file,fs,bits]=wavread('filename.wav');

len = length(wav_file);
wav_file_handle = wav_file;
wav_file_handle(20000:23000,1)=1;

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
wavwrite(wav_file_handle,fs,bits,'����������Ƶ������.wav');