% verifcation of omega load cell against futek (4/12/22 - 4/26/22)

% notes:
% figures 3 and 4 show how the omega sensor performed before the soldering
% change to the HX711 amplifer which improved the sample rate of the omega
% sensor hence reducing the time lag as shown in the figures 1 and 2
clc, clear
A = readmatrix('4_26_2022_1_nn.txt');
B = readmatrix('4_26_2022_2_nn.txt');
C = readmatrix('4_12_2022_3_na.txt');
D = readmatrix('4_12_2022_2_na.txt');

color1 = '#332288';
color2 = '#88CCEE';
color3 = '#44AA99';
color4 = '#117733';
color5 = '#999933';
color6 = '#DDCC77';
color7 = '#CC6677';
color8 = '#882255';
color9 = '#AA4499';
color10 = '#a6cee3';
color11 = '#1f78b4';
color12 = '#b2df8a';
color13 = '#1b9e77';
color14 = '#d95f02';
color15 = '#e9a3c9';
color16 = '#a1d76a';
color17 = '#d8b365';
color18 = '#5ab4ac';


figure(1)
kgFutek = A(3:end,3);
%kgFutek = -kgFutek;
kgOmega = A(3:end,1);
time1 = A(3:end,5);
plot(time1,kgFutek,'Color',color2,'Linewidth',1.50)
box on
hold on
plot(time1,kgOmega,'Color',color4,'Linewidth',1.50)

ax = gca;    ax.FontSize = 18;
xlabel('time (sec) ','FontSize',22,'Linewidth',1.25);
ylabel('mass (kg)','FontSize',22,'Linewidth',1.25);
legend('Futek','Omega','FontSize',22);

figure(2)
kgFutek2 = B(3:end,3);
%kgFutek2 = -kgFutek2;
kgOmega2 = B(3:end,1);
time2 = B(3:end,5);
plot(time2,kgFutek2,'Color',color2,'Linewidth',1.50)
box on
hold on
plot(time2,kgOmega2,'Color',color4,'Linewidth',1.50)

ax = gca;    ax.FontSize = 18;
xlabel('time (sec) ','FontSize',22,'Linewidth',1.25);
ylabel('mass (kg)','FontSize',22,'Linewidth',1.25);
legend('Futek','Omega','FontSize',22);

figure(3)
kgFutek3 = C(3:end,3);
kgFutek3 = -kgFutek3;
kgOmega3 = C(3:end,1);
time3 = C(3:end,5);
plot(time3,kgFutek3,'Color',color16,'Linewidth',1.50)
box on
hold on
plot(time3,kgOmega3,'Color',color1,'Linewidth',1.50)

ax = gca;    ax.FontSize = 18;
xlabel('time (sec) ','FontSize',22,'Linewidth',1.25);
ylabel('mass (kg)','FontSize',22,'Linewidth',1.25);
legend('Futek','Omega','FontSize',22);

figure(4)
kgFutek4 = D(3:end,3);
kgFutek4 = -kgFutek4;
kgOmega4 = D(3:end,1);
time4 = D(3:end,5);
plot(time4,kgFutek4,'Color',color16,'Linewidth',1.50)
box on
hold on
plot(time4,kgOmega4,'Color',color1,'Linewidth',1.50)

ax = gca;    ax.FontSize = 18;
xlabel('time (sec) ','FontSize',22,'Linewidth',1.25);
ylabel('mass (kg)','FontSize',22,'Linewidth',1.25);
legend('Futek','Omega','FontSize',22);