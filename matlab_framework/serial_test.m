close all;
clc;

s = serialport("COM4",9600,'Timeout',120);

txData = zeros(1, 2);
txData(1) = 50;
txData(2) = 30;
for n = 1:10
    tic;
    for i = 1:2
        write(s, txData(i), "char");
    end
    for i = 1:7
        rxData(n, i) = read(s, 1, "char") - '0';
    end
    rxData(n, 8) = read(s, 1, "uint16");
    T_HIL(n) = toc;
end
clear s;

T_HIL
rxData