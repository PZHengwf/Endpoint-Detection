[wav_file,fs,bits]=wavread('filename.wav');
wav_file_handle=awgn(wav_file,3,'measured');

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

sound(wav_file_handle,fs);
wavwrite(wav_file_handle,fs,bits,'��������-��˹������3db.wav');