[wav_file,fs,bits]=wavread('filename.wav');

f=fs*(0:511)/1024;
t=0:1/fs:(size(wav_file)-1)/fs;        %�����������źŵĵ�����������ԭʼ�ź���ͬ
Au=0.03;
noise=[Au*cos(2*pi*5000*t)]';          %����Ϊ5kHz�������ź�            ���޸���ֵ
wav_file_handle=wav_file+noise;

figure(1)
subplot(2,1,1);
plot(wav_file);
title('ԭʼ�����źŲ���');
xlabel('ʱ����');
ylabel('��ֵ');

subplot(2,1,2);
plot(wav_file_handle)
title('ԭʼ�����źż������');
xlabel('ʱ����');
ylabel('��ֵ');

sound(wav_file_handle,fs);             %���ż�������������ź�
wavwrite(wav_file_handle,fs,bits,'����������Ƶ��������.wav');