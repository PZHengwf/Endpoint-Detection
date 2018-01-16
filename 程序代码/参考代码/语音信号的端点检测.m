% �����źŵĶ˵���matlabʵ��:
[x,fs,nbits]=wavread('E:\����\�Ŷ���Ŀ\���Ի��ı�����ת��ϵͳ\����\΢��Ĭ��.wav');%���ȴ򿪾�¼�õ��ź�
x = x / max(abs(x));%���ȹ�һ����[-1,1]

%��������
FrameLen = 256;     %֡��
inc = 90;           %δ�ص����֣������漰���źŷ�֡�����⣬�ں���ٽ��͡�

amp1 = 10;          %��ʱ������ֵ
amp2 = 2;           %���趨������������ֵ��
zcr1 = 10;          %��������ֵ
zcr2 = 5;           %�����ʵ�������ֵ���о���һ��û���õ���
 
minsilence = 6;     %�������ĳ������ж������Ƿ����
minlen  = 15;       %�ж�����������С����

status  = 0;        %��¼�����ε�״̬
count   = 0;        %�������еĳ���
silence = 0;        %�����ĳ���
 
%���������
tmp1  = enframe(x(1:end-1), FrameLen,inc);                      %��x���з�֡��ÿ֡��FrameLen,�ֵõ�֡����fix(��x�ĳ���-FrameLen+step)/step��
tmp2  = enframe(x(2:end)  , FrameLen,inc);
signs = (tmp1.*tmp2)<0;
diffs = (tmp1 - tmp2)>0.02;
zcr   = sum(signs.*diffs,2);
 
%�����ʱ����
amp = sum((abs(enframe( x, FrameLen, inc))).^2, 2);             %ͨ����filter��ȥ�������ֽ����࣬���Ը��˸о�û��Ҫ��һ���˲������ϱ߳��ֵ�enframe�����ŵ���߷���������֪�������x��֡������ֵ���С�
 
%������������
amp1 = min(amp1, max(amp)/4);
amp2 = min(amp2, max(amp)/8);                                   %min����������Сֵ�ģ�û��Ҫ˵�ˡ�
 
%��ʼ�˵���
for n=1:length(zcr)                                             %�����￪ʼ�������������˼·��Length��zcr���õ����������źŵ�֡����
   goto = 0;
   switch status
   case {0,1}                                              % 0 = ����, 1 = ���ܿ�ʼ
      if amp(n) > amp1                                     % ȷ�Ž���������
         x1 = max(n-count-1,1);                            % ��¼�����ε���ʼ��
         status  = 2;
         silence = 0;
         count   = count + 1;
      else 
          if amp(n) > amp2 || zcr(n) > zcr2                % ���ܴ���������
              status = 1;
              count  = count + 1;
          else                                             % ����״̬
              status  = 0;
              count   = 0;
          end
      end
   case 2,                                                 % 2 = ������
       if amp(n) > amp2 ||zcr(n) > zcr2                    % ������������
           count = count + 1;
       else                                                % ����������
           silence = silence+1;
           if silence < minsilence                         % ����������������δ����
               count  = count + 1;
           else
               if count < minlen                           % ��������̫�̣���Ϊ������
                   status  = 0;
                   silence = 0;
                   count = 0;
               else                                        % ��������
                   status  = 3;
               end
           end
       end
   case 3,
       break;
   end
end  
 
count = count-silence/2;
x2 = x1 + count -1;              %��¼�����ν�����
%��ߵĳ������ҳ������ˣ�Ȼ���ú��߸��������û���ټ����������Ͳ���˵�ˡ�
subplot(3,1,1)
plot(x);
axis([1 length(x) -1 1])%����x����y��ķ�Χ��
ylabel('Speech');
line([x1*inc x1*inc], [-1 1], 'Color', 'red');
line([x2*inc x2*inc], [-1 1], 'Color', 'red');%ע����line�������÷���������������һ��ֱ�ߣ�������ˡ�
 
subplot(3,1,2)
plot(amp);
axis([1 length(amp) 0 max(amp)])
ylabel('Energy');
line([x1 x1], [min(amp),max(amp)], 'Color', 'red');
line([x2 x2], [min(amp),max(amp)], 'Color', 'red');
 
subplot(3,1,3)
plot(zcr);
axis([1 length(zcr) 0 max(zcr)])
ylabel('ZCR');
line([x1 x1], [min(zcr),max(zcr)], 'Color', 'red');
line([x2 x2], [min(zcr),max(zcr)], 'Color', 'red');