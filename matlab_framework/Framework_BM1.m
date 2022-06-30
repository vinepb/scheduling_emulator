%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Framework with Task Scheduler: Perturb & Observe method and Knapsack problem %%%  
% Description: 
% Input:                   
% Output:
% Author: Laio O. Seman e Sara Vega Martinez  
% UFSC 
% Created Avril 2020
close all;
clear all;
clc;

taskName = 'tarefas11';
fileName = 'sim/sim1_case11.txt';

%% Input 
load('Detumbling_Irradiance.mat');

expLut = [1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 13, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 32, 33, 35, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 60, 63, 66, 69, 72, 76, 79, 83, 87, 91, 95, 100];
invLut = [1, 6, 10, 14, 18, 22, 25, 29, 32, 35, 38, 41, 43, 46, 49, 51, 53, 55, 57, 59, 61, 63, 65, 66, 68, 69, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 84, 85, 86, 87, 87, 88, 88, 89, 90, 90, 91, 91, 91, 92, 92, 93, 93, 93, 94, 94, 94, 95, 95, 95, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 98, 98, 98, 98, 98, 98, 98, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100];

StepTimeVector = IrradianceTotal(:,1);
Time_length = size(StepTimeVector);
Qtot1 = IrradianceTotal(:,2);
Qtot2 = IrradianceTotal(:,3);
Qtot3 = IrradianceTotal(:,4);
Qtot4 = IrradianceTotal(:,5);
Qtot5 = IrradianceTotal(:,6);
Qtot6 = IrradianceTotal(:,7);

Qthermal1 = IrradianceTotal(:,2);
Qthermal2 = IrradianceTotal(:,3);
Qthermal3 = IrradianceTotal(:,4);
Qthermal4 = IrradianceTotal(:,5);
Qthermal5 = IrradianceTotal(:,6);
Qthermal6 = IrradianceTotal(:,7);

% Power Load
VT_set_bat = [0; 0; 0; 0; 0] + 273;
TableSoC = [0.95; 0.9; 0.85; 0.8; 0];

%% Parameters
Rh = 10;                        % Heater Resistor
StepTime = StepTimeVector(2,:) - StepTimeVector(1,:);
BatteryCapacity = 3;            % Battery Capacity Ah 
perturb = 1;

%% Initialization
histerese = 0;
BTotal = 0;
PspTotalM = 0 ;
PhM = 0;
PmpptM = 0;
P_TPayloadM  = 0;
 P_TPayload3M =  0;
P_BeaconM  = 0;
u_anterior = 0;
erro_anterior = 0;
count_L = 0;
trocou_L = 0;
count_B = 0;

SoC(1) = 0.9;
SoC(2) = 0.9;

VBat(1) = 4.2;   % voltage battery
VBat(2) = 4.2;   % voltage battery

load(taskName); % igual que a 2 mas ajustava payloads e beacon
J = length(unnamed3);
n=J;
j = unnamed3(1:n,1);  % numero tarefa
U = unnamed3(1:n,6);  % prioridade da tarefa
R = unnamed3(1:n,4); % recurso da tarefa
Dl = unnamed3(1:n,3); % Deadline e periodo
C = unnamed3(1:n,2);  % tempo de execuçao
X = unnamed3(1:n,5);  % tarefa executada ou não
a = unnamed3(1:n,5);  % quanto tempo esta executando

M = 65535;

Dl_lost = 0;

for i = 1:J
    d(i) =  Dl(i) + 3;
    R(i) = round(R(i));
    Cont(i) = 0;
    perdeu(i) = 0;
    Ex(i) = 0;
    Contador(i) = 0;
end
Max_W = sum(R);

perturb(1) = 1;
perturb(2) = 1;
ILoad(1) = 0; 
ILoad(2) = 0;
TSet(1) = VT_set_bat(1,:);
TSet(2) = VT_set_bat(1,:);
TSet(3) = VT_set_bat(1,:);
D(1) = 0;    % Duty cycle
D(2) = 0;    % Duty cycle
D(3) = 0;    % Duty cycle
Ih(1) = VBat(1) * D(1)/Rh;      % current heater 
Ph(1) = Ih(1) * VBat(1);          % Power Heater
Ih(2) = Ih(1);      % current heater
Ph(2) = Ph(1);      % Power Heater
Tini = 275; %Precisa para poder entrar na funcao

% Solar Panel Battery Temperaures 
[Tsp1(1),Tsp2(1),Tsp3(1),Tsp4(1),Tsp5(1),Tsp6(1),TBat(1)] = ...
Modelo_termico_function(Qthermal1(1),Qthermal2(1),Qthermal3(1)...
,Qthermal4(1),Qthermal5(1),Qthermal6(1),Tini,Tini,Tini,Tini,Tini,Tini,Tini,Ph(1)); %incluir Qg como input num segundo momento

[Tsp1(2),Tsp2(2),Tsp3(2),Tsp4(2),Tsp5(2),Tsp6(2),TBat(2)] = ... 
Modelo_termico_function(Qthermal1(2),Qthermal2(2),Qthermal3(2)...
,Qthermal4(2),Qthermal5(2),Qthermal6(2),Tini,Tini,Tini,Tini,Tini,Tini,Tini,Ph(2)); %incluir Qg como input num segundo momento

% Current solar panel
V = [0:0.01:4.5];  % V for MPPT

Isp1(1) = solar_cellAzur(VBat(1),Qtot1(1),Tsp1(1));
Isp2(1) = solar_cellAzur(VBat(1),Qtot2(1),Tsp2(1));
Isp3(1) = solar_cellAzur(VBat(1),Qtot3(1),Tsp3(1));
Isp4(1) = solar_cellAzur(VBat(1),Qtot4(1),Tsp4(1));
Isp5(1) = solar_cellAzur(VBat(1),Qtot5(1),Tsp5(1));
Isp6(1) = solar_cellAzur(VBat(1),Qtot6(1),Tsp6(1));

Isp(1) = Isp1(1) + Isp2(1) + Isp3(1) + Isp4(1) + Isp5(1) + Isp6(1);

if (Isp(1) < 0)                   % Ecplipse, solar current negative
   Isp(1) = 0.00001;
end

Isp1(2) = Isp1(1);
Isp2(2) = Isp2(1);
Isp3(2) = Isp3(1);
Isp4(2) = Isp4(1);
Isp5(2) = Isp5(1);
Isp6(2) = Isp6(1);


Isp(2) = Isp(1);
PspTotal(1) = Isp(1) * VBat(1);
PspTotal(2) = PspTotal(1);

Ibat(1) = Ih(1) + ILoad(1) - Isp(1) ;      % Current Battery,   Ibat = Ih + Iload - Isp
SoC(1)  = SoC(1) -(Ibat(1) * StepTime / (BatteryCapacity * 3600));
VBat(1) = battery(SoC(1),Ibat(1),20,StepTime);
Ibat(2) = Ibat(1);                          % Current Battery,   Ibat = Ih + Iload - Isp
SoC(2)  = SoC(1);
VBat(2) = VBat(1);
W(1) = 0;
W(2) = 0;
W(3) = 0;
% Power balance
B(1) = (Isp(1) - Ih(1) - ILoad(1)) * VBat(1);
B(2) = B(1);
PLoad(1) = ILoad(1) *  VBat(1) ;
PLoad(2) = ILoad(1) *  VBat(1) ;
PLoadTotal = PLoad(2) + PLoad(1);
P_TPayload(1) = 0;
P_TPayload(2) = 0;
P_TPayload3(1) = 0;
P_TPayload3(2) = 0;
P_Beacon(1) = 0;
P_Beacon(2) = 0;

%% Framework
Time_length = round(Time_length);
for t = 3:Time_length % 

TimerS(t) = t;    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Compute with Newton method
Vbat_actual = VBat(t-1);
Vbat_old = Vbat_actual + 0.5;
 
while (abs(Vbat_actual - Vbat_old)> 1e-3) 
     Vbat_old = Vbat_actual;
      
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Load
       
    PLoad(t) = 0;
    Ph(t) = 0;
    P_TPayload3(t) = 0;
    P_TPayload(t) = 0;
    P_Beacon(t) = 0;
    for i = 1:J
       if X(i) == 1
           if i == 22
             Ph(t) = R(i);  % Heater           
           else   
             PLoad(t) = PLoad(t) + R(i);   % Load        
           end
       end
    end
    

    if X(1)  == 1
        P_TPayload(t) = R(1); 
    end
    if X(2)  == 1  
        P_TPayload(t) =  P_TPayload(t) + R(2);
    end
%        
    if X(31)  == 1  
        P_Beacon(t) =  R(31);
    end
    
    P_TPayload(t) = P_TPayload(t)/1000;
    P_Beacon(t) = P_Beacon(t)/1000;
    PLoad(t) = PLoad(t)/1000;
    Ph(t) = Ph(t)/1000;

    ILoad(t) = (PLoad(t) + P_TPayload3(t)) / Vbat_actual;%3.3;
   Ih(t) = Ph(t)/Vbat_actual;
   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Temperature (calculo da temperatura)

    [Tsp1(t),Tsp2(t),Tsp3(t),Tsp4(t),Tsp5(t),Tsp6(t),TBat(t)] = ...
     Modelo_termico_function(Qthermal1(t),Qthermal2(t),Qthermal3(t),Qthermal4(t),...
     Qthermal5(t),Qthermal6(t),Tsp1(t-1),Tsp2(t-1),Tsp3(t-1),Tsp4(t-1),Tsp5(t-1),...
     Tsp6(t-1),TBat(t-1),Ph(t)); %incluir Qg como input num segundo momento

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Solar panel 
    
    Isp1(t) = solar_cellAzur(Vbat_actual,Qtot1(t),Tsp1(t));
    Isp2(t) = solar_cellAzur(Vbat_actual,Qtot2(t),Tsp2(t));
    Isp3(t) = solar_cellAzur(Vbat_actual,Qtot3(t),Tsp3(t));
    Isp4(t) = solar_cellAzur(Vbat_actual,Qtot4(t),Tsp4(t));
    Isp5(t) = solar_cellAzur(Vbat_actual,Qtot5(t),Tsp5(t));
    Isp6(t) = solar_cellAzur(Vbat_actual,Qtot6(t),Tsp6(t));
    
    Isp(t) = 0;
    if (Isp1(t) > 0)                   % solar current negative
        Isp(t) = Isp(t) + Isp1(t);
    end
    if (Isp2(t) > 0)                   % solar current negative
        Isp(t) = Isp(t) + Isp2(t);
    end
    if (Isp3(t) > 0)                   % solar current negative
        Isp(t) = Isp(t) + Isp3(t);
    end
    if (Isp4(t) > 0)                   % solar current negative
        Isp(t) = Isp(t) + Isp4(t);
    end
    if (Isp5(t) > 0)                   % solar current negative
        Isp(t) = Isp(t) + Isp5(t);
    end
    if (Isp6(t) > 0)                   % solar current negative
        Isp(t) = Isp(t) + Isp6(t);
    end

    PspTotal(t) = Isp(t) * Vbat_actual;
   
    %MPPT 
    I1 = solar_cellAzur(V,Qtot1(t),Tsp1(t));
    I2 = solar_cellAzur(V,Qtot2(t),Tsp2(t));
    I3 = solar_cellAzur(V,Qtot3(t),Tsp3(t));
    I4 = solar_cellAzur(V,Qtot4(t),Tsp4(t));
    I5 = solar_cellAzur(V,Qtot5(t),Tsp5(t));
    I6 = solar_cellAzur(V,Qtot6(t),Tsp6(t));
    
     P = 0;
     if (I1(1) > 0)
         P = P + I1;
     end
     if (I2(1) > 0)
         P = P + I2;
     end
     if (I3(1) > 0)
         P = P + I3;
     end
      if (I4(1) > 0)
         P = P + I4;
     end
     if (I5(1) > 0)
         P = P + I5;
     end
     if (I6(1) > 0)
         P = P + I6;
     end
      
     
    P = P .* V;
    Pmppt(t) = max (P);
    idx = find(P == Pmppt(t));
    
     if P < 0.0001
        Vmppt(t) = 0; % Open-current voltage at T1 [V]. input Celsius
     else
        Vmppt(t)=V(idx);
     end
   
    I1mppt(t) = solar_cellAzur(Vmppt(t),Qtot1(t),Tsp1(t));
    I2mppt(t) = solar_cellAzur(Vmppt(t),Qtot2(t),Tsp2(t));
    I3mppt(t) = solar_cellAzur(Vmppt(t),Qtot3(t),Tsp3(t));
    I4mppt(t) = solar_cellAzur(Vmppt(t),Qtot4(t),Tsp4(t));
    I5mppt(t) = solar_cellAzur(Vmppt(t),Qtot5(t),Tsp5(t));
    I6mppt(t) = solar_cellAzur(Vmppt(t),Qtot6(t),Tsp6(t));
    
     
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Battery
    
    
   Ibat(t) = Isp(t) - Ih(t) - ILoad(t) ;      % Current Battery,   Ibat = Ih + Iload - Isp
    
    % Proteccion undarvoltage 2.4V
    if Vbat_actual == 2.4
         Ibat(t) = 0;  
     end  
    
    SoC(t) = SoC(t-1) + (Ibat(t) * StepTime / (BatteryCapacity * 3600));
    
    if (SoC(t) > 1)                   % Ecplipse, solar current negative
        SoC(t) = 1;
    end
    
    VBat(t) = battery(SoC(t),Ibat(t),TBat(t),StepTime);
       
    Vbat_actual = VBat(t);
end

    % Power balance
     B(t) = (Isp(t) - Ih(t) - ILoad(t)) * VBat(t);
    
     BTotal = BTotal + B(t) * StepTime ;
     PspTotalM = PspTotalM + PspTotal(t) * StepTime ;
     PmpptM = PmpptM + Pmppt(t)  * StepTime ;
     PLoadTotal = PLoadTotal +  PLoad(t) * StepTime ;
     PhM = PhM + Ph(t) * StepTime;
     P_TPayloadM =  P_TPayloadM + P_TPayload(t) * StepTime;
     P_TPayload3M =  P_TPayload3M + P_TPayload3(t) * StepTime;
     P_BeaconM  = P_BeaconM + P_Beacon(t) * StepTime;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
%% Controls
%  Perturb & Observe method 
% Task Scheduler: Knapsack problem

     
%% Perturb and observe method
% perturb = 1 =  Increase Vbat (decrease load power)
% perturb = 0 =  Decreases Vbat (increases load power)

if (PspTotal(t-1) > PspTotal(t)) 
    if (perturb(t-1) == 0)
        perturb(t) = 1;
    else
        perturb(t) = 0;
    end
else
    perturb(t) = perturb(t-1);
end
	
%% Dynamic power step

% %% Tasks Organization -2 -  As tarefas podem ser mais vezes ligadas antes da seguinte chamada.

for i = 1:J
    aa_deadline2(t,i) =0; 
    % a tarefa terminou
    if (C(i) - a(i))  <= 0
        X(i) = 0;
        a(i) = 0;
        U(i) = 1;
		Ex(i) = 1 ;
        Contador(i) = Contador(i) + 1;
    end

	% Perdeu el deadline
	
	if (t > 0) && (t == d(i)) && (Ex(i) == 0) 
        perdeu(i) = perdeu(i) + 1;
    end

	% calculo del nuevo dedline
    if t == d(i)
		aa_deadline2(t,i) =1; 
		d(i) = t + Dl(i);
        Ex(i) = 0 ;  % variavel que indica se foi executada ou nao a tarefa
    end
    
    % calculo da prioridade
    if X(i) == 1 % si a tarefa esta ligada
        a(i) =  a(i) + 1;
    end
             
    if (d(i) - C(i) - t) <= 0
        U(i) = M;    % perdida proxima de Deadline
    else
            U(i) = round(max( (100 - ( 100 * (d(i) - t) / Dl(i) ) ), 1));
            % U(i) = expLut(round(max( (100 - ( 100 * (d(i) - t) / Dl(i) ) ), 1)));
            % U(i) = invLut(round(max( (100 - ( 100 * (d(i) - t) / Dl(i) ) ), 1)));
    end
    
        if Ex(i) == 1  % si ya fue ejecutad a tarefa prioridade baixa
            if i == 22
                U(i) = 0;
            else
                U(i) = 1;
            end
        end
    end

    Dl_lost(t) = sum(perdeu);

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % %% Knapsack problem solver
% % 

Step(t) = 0.01;

if (perturb(t) == 1) 
    W(t) = (W(t-1) - Step(t));
else
    W(t) = (W(t-1) + Step(t));
end
 
 if  W(t) <= 0.5
     W(t) = 0.5;
 end
 
  if Isp(t) < 0.001
       W(t) = 1;
  end
 
  W_knapsack =  round(W(t) * 1000);

 % Knapsack problem
     [best, X] = knapsack(R,U, W_knapsack);    
     
    for i = 1:J        
        aa_Activar(t,i) = X(i);
        aa_Activada(t,i) = a(i);
        aa_prioridade(t,i) = U(i);
        aa_deadline(t,i) = d(i);
        aa_Executo(t,i) = Ex(i);
    end
end

% PLoadTotal 
 PLoadTotalmedia =  PLoadTotal / Time_length(1)



PmpptM
PspTotalM 
BTotal
PLoadTotal
%PLoadTotal = PLoadTotal + P_TPayloadM +P_BeaconM
PhM 
P_TPayloadM
P_TPayload3M
P_BeaconM
%PCargaTotal =  PLoadTotal + PhM +  P_TPayloadM + P_BeaconM + P_TPayload3M
PCargaTotal =  PLoadTotal + PhM
PCargaTotalM = PCargaTotal/ Time_length(1)
PspTotalM - PCargaTotal 
%%%%%%%%%%%% anotacoes %%%%%%%%%

%%%%%%%%%% txt %%%%%%%%%%%%

f = fopen(fileName, 'w');
fprintf(f, '%d,', length(TimerS));
fprintf(f, '\r\n');
fprintf(f, '%d,', TimerS);
fprintf(f, '\r\n');
fprintf(f, '%d,', Pmppt);
fprintf(f, '\r\n');
fprintf(f, '%d,', PspTotal);
fprintf(f, '\r\n');
fprintf(f, '%d,', Vmppt);
fprintf(f, '\r\n');
fprintf(f, '%d,', VBat);
fprintf(f, '\r\n');
fprintf(f, '%d,', SoC);
fprintf(f, '\r\n');
fprintf(f, '%d,', Dl_lost);
for i = 1:J
    fprintf(f, '\r\n');
    fprintf(f, '%d,', aa_Activar(:,i));
end
fprintf(f, '\r\n');
fprintf(f, '%d,', PLoad);
fprintf(f, '\r\n');
fprintf(f, '%d,', Ph);
fprintf(f, '\r\n');
fprintf(f, '%d,', W);
fprintf(f, '\r\n');
fprintf(f, '%d,', TBat);
fclose(f);


