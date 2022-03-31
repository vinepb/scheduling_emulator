function Ia = solar_cellAzur(Va,Suns,TaC)
    % Ia,Va = current and voltage vectors [A] and [V]
    % G = number of Suns [] (1 Sun = 1000 W/mˆ2)
    % T = temperature of the cell [°C]
    k = 1.38e-23; % Boltzmann constant [J/K]
    q = 1.60e-19; % Elementary charge [C]
    n = 3.25; % Quality factor for the diode []. n=2 for crystaline, <2 for amorphous
    Vg = 1.12; % Voltage  of the Crystaline Silicon [eV], 1.75eV for Amorphous Silicon
    T1 = 273 + 28; % Normalised temperature [K]
  
    
    Ncell=1.71;       %Nombre de cellule en série dans un module
    Ns=1;          %Nombre de module en parallèle
    
    % AzurSpace values
    Voc_T1 = 2.564-((10/1000)*(TaC-(28+273))); % Open-current voltage at T1 [V]. input kelvin
  %  Voc_T1 = 2.564-((10/1000)*(TaC-28)); % Open-current voltage at T1 [V]. input Celsius
    Voc_T1 = Voc_T1 * Ncell;
    Isc_T1 = 0.41*Ns; % Short-circuit current at T1 [A]. 
    K0 = 0.35/1000; % Current/Temperature coefficient [A/K]. 
    dVdI_Voc = -0.43; % dV/dI coefficient at Voc [A/V]. See Tests.xlsx 
   
    TaK = TaC; % Convert cell's temperature from Celsius to Kelvin [K]
    
    IL_T1 = Isc_T1 * Suns/1000; % Compute IL depending the suns at T1. Equation (3)
    IL = IL_T1 + K0*(TaK - T1); % Apply the temperature effect. Equation (2)
    I0_T1 = Isc_T1/(exp(q*Voc_T1/(n*k*T1))-1); % Equation (6)
    I0 = I0_T1*(TaK/T1).^(3/n).*exp(-q*Vg/(n*k).*((1./TaK)-(1/T1))); % Equation (5)
    Xv = I0_T1*q /(n*k*T1) * exp(q*Voc_T1/(n*k*T1)); % Equation (8)
    Rs = - dVdI_Voc - 1/Xv; %Compute the Rs Resistance. Equation (7)
    Vt_Ta = n * k * TaK / q; % Equation (9)
    Ia = zeros(size(Va)); %Initialize Ia vector
    % Compute Ia with Newton method
    for j=1:5
        Ia = Ia - (IL - Ia - I0.*( exp((Va+Ia.*Rs)./Vt_Ta) -1))./(-1 - (I0.*( exp((Va+Ia.*Rs)./Vt_Ta) -1)).*Rs./Vt_Ta);
    end    
end