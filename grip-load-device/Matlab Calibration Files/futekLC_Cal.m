% Futek Load Cell Calibration (1/22/2022)
clc, clear
A = readmatrix('1_22_2022_2_na.txt');
color2 = '#3399FF';
color1 = '#9933FF';


% --- load cell stage 1 ---- %
L1 = A(1:768,1)'; % 
L1m = mean(L1)

L2 = A(770:1537,1)'; % 
L2m = mean(L2)

L3 = A(1540:2306,1)'; % 
L3m = mean(L3)

L4 = A(2309:3075,1)'; % 
L4m = mean(L4)

L5 = A(3078:3845,1)'; % 
L5m = mean(L5)

L6 = A(3848:4615,1)'; % 
L6m = mean(L6)

L7 = A(4618:5385,1)'; % 
L7m = mean(L7)

L8 = A(5388:6155,1)'; % 
L8m = mean(L8)

L9 = A(6158:6924,1)'; % 
L9m = mean(L9)

L10 = A(6927:7694,1)'; % grams
L10m = mean(L10)

L11 = A(7699:8462,1)'; % grams
L11m = mean(L11)

L12 = A(8465:9232,1)'; % grams
L12m = mean(L12)

L13 = A(9235:10001,1)'; % grams
L13m = mean(L13)

L14 = A(10004:10771,1)'; % grams
L14m = mean(L14)

L15 = A(10774:11540,1)'; % grams
L15m = mean(L15)

L16 = A(11543:12310,1)'; % grams
L16m = mean(L16)

L17 = A(12313:13080,1)'; % grams
L17m = mean(L17)

L18 = A(13083:13850,1)'; % grams
L18m = mean(L18)

L19 = A(13853:14620,1)'; % grams
L19m = mean(L19)

L20 = A(14623:15390,1)'; % grams
L20m = mean(L20)

L21 = A(15393:16159,1)'; % grams
L21m = mean(L21)

L22 = A(16162:16928,1)'; % grams
L22m = mean(L22)

L23 = A(16931:17698,1)'; % grams
L23m = mean(L23)

Voltage = [L1m L2m L3m L4m L5m L6m L7m L8m L9m L10m L11m L12m L13m L14m L15m L16m L17m L18m L19m L20m L21m L22m L23m];
Load = [0 1.18 2.25 3.25 4.05 4.55 5.515 6.015 6.515 6.815 7.53 8.155];
flipLoad = flip(Load);
flipLoad(1) = [];
Loadd = [Load, flipLoad];

ax1 = subplot(1,1,1);
scatter(ax1,Voltage,Loadd,'b','Filled','LineWidth',1.5);
a1 = lsline(ax1); %aa1 = lsline(ax1);
a1.Color = color2; %aa1.Color = color2;
a1.LineWidth = 2;  %aa1.LineWidth = 2;
grid on; box on;
ax = gca;    ax.FontSize = 16;
legend('',' y = 2.484x + 0.0297 ','FontSize',18);
%title('RO: Force vs Vout','FontSize',22);
xlabel('Futek Vout [V]','FontSize',18);
ylabel('Applied Load [kg]','FontSize',18);



