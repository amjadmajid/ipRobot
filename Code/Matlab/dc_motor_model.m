clc
clear

Period = 2000;

V = 2.2;
R = 14.5;
L = 70e-6;
%Jm = 10e-6;
Jm = 1.3e-6; % estimated from python motor graph
Jl = 0;
J = Jm + Jl;
%Bt = T / omega;
%Ke = 0.023;
%Kt = 0.015;
No_load_speed = 1200*(2*pi/60); % rad/s
No_load_current = 35e-3; % A
Rated_load_speed = 850*(2*pi/60); %rad/s
Rated_load = 0.8e-3; % Nm
Rated_load_current = 80e-3; % A
Rated_voltage = 3.0;
Ke = Rated_voltage / (No_load_speed);
Kt = Rated_load / (Rated_load_current - No_load_current);
Bm = Kt * No_load_current / No_load_speed;
Bl = 0; %Kt * Rated_load_current / Rated_load_speed; %0; %5e-6;
B = Bm + Bl;
%disp(Kt)


m = 10e-3;
r = 7e-3;
g = 9.81;
ur = 0.01;
Text = 0; %6.8e-5; %r * ur * m * g;


%sim('dc_motor') % running model from script*