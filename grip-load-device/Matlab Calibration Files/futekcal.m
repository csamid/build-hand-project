clc, clear
%% updated futek calibration (push and pull) 4/11/2022
pull = [2485.917, 2485.534, 2485.691, 2550.309, 2551.467, 2550.301,2620.657,2621.568,2620.600,2691.071,2691.332,2690.724,2753.173,2753.399,2752.996,2862.251,2862.960,2862.098,2976.843,2977.266,2976.275,3089.896,3090.237,3089.641,3207.297,3207.797,3207.040,3332.587,3333.009,3332.344];
kg_pull = [0,0,0,0.61,0.61,0.61,1.265,1.265,1.265,1.915,1.915,1.915,2.34,2.34,2.34,3.36,3.36,3.36,4.425,4.425,4.425,5.48,5.48,5.48,6.575,6.575,6.575,7.74,7.74,7.74];
kg_pull(13:end) = kg_pull(13:end) + 0.155;
push = [2459.563,2450.355,2400.980,2402.984,2330.740,2332.417,2260.420,2262.373,2197.323,2199.394,1955.406,1957.704,1842.747,1845.294,1786.275,1788.406,1677.167,1678.768];
kg_push = [-.1,-.1,-0.61,-0.61,-1.265,-1.265,-1.915,-1.915,-2.34,-2.34,-4.6,-4.6,-5.655,-5.655,-6.19,-6.19,-7.21,-7.21];
kg_push(3:8) = kg_push(3:8) + 0.155; % removing basket weight that was not used here
kg_push(3:end) = kg_push(3:end) - .100; % adding the weight of the yellow part and screw that lay on top of the sensor
voltage = [pull,push]/1000.0;
kg = [kg_pull,kg_push];

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


%% plot data and obtain linear regression
ax1 = subplot(1,1,1);
scatter(ax1,voltage,kg,'MarkerEdgeColor',color11,'LineWidth',1.5);
a1 = lsline(ax1); %aa1 = lsline(ax1);
a1.Color = color2; %aa1.Color = color2;
a1.LineWidth = 3;  %aa1.LineWidth = 2;
box on;
ax = gca;    ax.FontSize = 32;
legend('',' kg = 9.132*Vout - 22.6 ','FontSize',32);
xlabel('Futek Vout [V]','FontSize',32);
ylabel('Applied Load [kg]','FontSize',32);
xlim([1.6  3.4]);
