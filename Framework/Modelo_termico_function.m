function [T1, T2, T3, T4, T5, T6, Tbat] = Modelo_termico_function(Qthermal1, Qthermal2, Qthermal3, Qthermal4, Qthermal5, Qthermal6, T1old, T2old, T3old, T4old, T5old, T6old, Tbatold, Qg) %incluir Qg como input num segundo momento
%Programa para modelo termico do artigo 2020
%T1 ate T6 sao as temperaturas dos paineis solares
%Tbat � a temperatura da bateria
%Qtot sao as irradiancias para cada caso [W/m�]
%Tset � a temperatura de setpoint para o heater da bateria

%% Constantes
epsilon=0.3;               %emissividade dos PVs [-]  
alpha=0.3;                 %absortividade dos Pvs [-]  
area_rad=0.01;           %area de troca por radiacao [m�]
area_cond=0.1*area_rad; %area de troca por condu��o [m�]                    ?
sigma=5.67*10^(-8);      %constante de Stefann-Boltzmann [W/m�K^4]
cond=0.5;                 %coeficiente de condutividade termica [W/mK]        ? 
length=0.05;             %comprimento de troca por condu��o [m]              ?
m_pv=0.06;               %massa de cada painel fotovoltaico [kg]             ?
m_bat=0.05;               %massa da bateria  [kg]                             ?
c_pv=1000;                %calor especifico do PV [J/kgK]                     ?  
c_bat=800;              %calor especifico da bateria [J/kgK]                ?
funcionais=7; %Quant. de funcionais

%% Outros Input
% Qg=1;     % Heater Power ( W)
erro=1E-3; %erro aceitavel no N-R
diff=1;    %inicializacao de variavel para criterio de parada do N-R
timestep=1;    %Incremento no tempo [s]    %Melhor se ler automatico      
iteracoes=586; %numero total de iteracoes. %Melhor se ler automatico

%% Condi��es iniciais em Kelvin

 T1=T1old+0.5;
 T2=T2old+0.5;
 T3=T3old+0.5;
 T4=T4old+0.5;
 T5=T5old+0.5;
 T6=T6old+0.5;
 Tbat=Tbatold+0.5;

% T1old=273+1;
% T2old=275+1;
% T3old=273+1;
% T4old=273+1;
% T5old=373+1;
% T6old=273+1;
% Tbatold=270+1;

%% Equacoes iniciais
R=epsilon*area_rad*sigma; %constante do termo de radiacao
D=cond*area_cond/length;  %constante do termo de condu��o
Mpv=m_pv*c_pv/timestep;    %constante do termo transiente no PV
Mbat=m_bat*c_bat/timestep; %constante do termo transiente na bateria

%for t=1:iteracoes; 
   
    while abs(diff)>erro
    %os primeiros 6 funcionais sao dos PVs e o 7 da bateria
    %como nao h� o instante i-1, fiz essa particularidade em i==1
%         if t==1
%             f(1)=area_rad*alpha*Qtot1-R*T1^4+D*(Tbat-T1);
%             f(2)=area_rad*alpha*Qtot2-R*T2^4+D*(Tbat-T2);
%             f(3)=area_rad*alpha*Qtot3-R*T3^4+D*(Tbat-T3);
%             f(4)=area_rad*alpha*Qtot4-R*T4^4+D*(Tbat-T4);
%             f(5)=area_rad*alpha*Qtot5-R*T5^4+D*(Tbat-T5);
%             f(6)=area_rad*alpha*Qtot6-R*T6^4+D*(Tbat-T6);
%             %%% essa parte � para calcular o somatorio dentro do volume da bateria
%             Dk=D*(T1-Tbat)+D*(T2-Tbat)+D*(T3-Tbat)+D*(T4-Tbat)+D*(T5-Tbat)+D*(T6-Tbat);
%             f(7)=Dk+Qg;
%         else
            %aqui � para qualquer tempo t>1
            f(1)=area_rad*alpha*Qthermal1-R*T1^4+D*(Tbat-T1)-Mpv*(T1-T1old);
            f(2)=area_rad*alpha*Qthermal2-R*T2^4+D*(Tbat-T2)-Mpv*(T2-T2old);
            f(3)=area_rad*alpha*Qthermal3-R*T3^4+D*(Tbat-T3)-Mpv*(T3-T3old);
            f(4)=area_rad*alpha*Qthermal4-R*T4^4+D*(Tbat-T4)-Mpv*(T4-T4old);
            f(5)=area_rad*alpha*Qthermal5-R*T5^4+D*(Tbat-T5)-Mpv*(T5-T5old);
            f(6)=area_rad*alpha*Qthermal6-R*T6^4+D*(Tbat-T6)-Mpv*(T6-T6old);
            Dk=D*(T1-Tbat)+D*(T2-Tbat)+D*(T3-Tbat)+D*(T4-Tbat)+D*(T5-Tbat)+D*(T6-Tbat);
            f(7)=Dk+Qg-Mbat*(Tbat-Tbatold);
    
%        end
    
        for n=1:funcionais
            F(n)=f(n);
        end
    
        %derivada dos funcionais
        df(1,1)=-4*R*T1^3-D-Mpv; %verificar se o ultimo termo est� correto
        df(1,2)=0;
        df(1,3)=0;
        df(1,4)=0;
        df(1,5)=0;
        df(1,6)=0;
        df(1,7)=D;
        df(2,1)=0;
        df(2,2)=-4*R*T2^3-D-Mpv; %verificar se o ultimo termo est� correto;
        df(2,3)=0;
        df(2,4)=0;
        df(2,5)=0;
        df(2,6)=0;
        df(2,7)=D;
        df(3,1)=0;
        df(3,2)=0;
        df(3,3)=-4*R*T3^3-D-Mpv; %verificar se o ultimo termo est� correto;
        df(3,4)=0;
        df(3,5)=0;
        df(3,6)=0;
        df(3,7)=D;
        df(4,1)=0;
        df(4,2)=0;
        df(4,3)=0;
        df(4,4)=-4*R*T4^3-D-Mpv; %verificar se o ultimo termo est� correto;
        df(4,5)=0;
        df(4,6)=0;
        df(4,7)=D;
        df(5,1)=0;
        df(5,2)=0;
        df(5,3)=0;
        df(5,4)=0;
        df(5,5)=-4*R*T5^3-D-Mpv; %verificar se o ultimo termo est� correto
        df(5,6)=0;
        df(5,7)=D;
        df(6,1)=0;
        df(6,2)=0;
        df(6,3)=0;
        df(6,4)=0;
        df(6,5)=0;
        df(6,6)=-4*R*T6^3-D-Mpv; %verificar se o ultimo termo est� correto
        df(6,7)=D;
        df(7,1)=D; %verificar se o ultimo termo est� correto
        df(7,2)=D;
        df(7,3)=D;
        df(7,4)=D;
        df(7,5)=D;
        df(7,6)=D;
        df(7,7)=-6*D-Mbat;
    
        for m=1:funcionais
            for n=1:funcionais
                J(m,n)=df(m,n);
            end
        end

        %%%Solu��o do sistema linear
        Jminus=inv(J);
        DeltaT=Jminus*(F');
    
        %%%Corre��o dos valores
       % for n=1:funcionais
            T1=T1-DeltaT(1);
            T2=T2-DeltaT(2);
            T3=T3-DeltaT(3);
            T4=T4-DeltaT(4);
            T5=T5-DeltaT(5);
            T6=T6-DeltaT(6);
            Tbat=Tbat-DeltaT(7);
            diff=DeltaT(1)+DeltaT(2)+DeltaT(3)+DeltaT(4)+DeltaT(5)+DeltaT(6)+DeltaT(7); %variacao para convergencia
        %end
    
    end
%     T1old=T1;
%     T2old=T2;
%     T3old=T3;
%     T4old=T4;
%     T5old=T5;
%     T6old=T6;
%     Tbatold=Tbat;
%     
%         %%para avancar no tempo
%        T1(t+1)=((area_rad*alpha*Qtot1-R*T1^4+D*(Tbat-T1))*timestep+Mpv*T1)/Mpv;
%        T2(t+1)=((area_rad*alpha*Qtot2-R*T2^4+D*(Tbat-T2))*timestep+Mpv*T2)/Mpv;
%        T3(t+1)=((area_rad*alpha*Qtot3-R*T3^4+D*(Tbat-T3))*timestep+Mpv*T3)/Mpv;
%        T4(t+1)=((area_rad*alpha*Qtot4-R*T4^4+D*(Tbat-T4))*timestep+Mpv*T4)/Mpv;
%        T5(t+1)=((area_rad*alpha*Qtot5-R*T5^4+D*(Tbat-T5))*timestep+Mpv*T5)/Mpv;
%        T6(t+1)=((area_rad*alpha*Qtot6-R*T6^4+D*(Tbat-T6))*timestep+Mpv*T6)/Mpv;    
%         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% verificar
%         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% se Dk esta
%         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% certo
%        Tbat(t+1)=((Dk+Qg)*timestep+Mbat*Tbat)/Mbat;
       
%end