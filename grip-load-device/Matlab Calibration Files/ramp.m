% ramp generation example
function [] = ramp(width,height)
x = 0:0.01:29.99;  % this would be time from the file
length(x)
sw = 0;
g = 250;
width = 5;
height = 1;
slope = height/width;

    for i = 1:length(x)

        if sw == 0
            y(i) = 0;
           if length(y) == g
               sw = 1;
           end

        elseif sw == 1
            y(i) = y(i-1) + slope*(0.01);
            f = y(i);
            if y(i) >= height
                sw = 0;
                g = 500 + i;
            end
        end

    end
plot(x,y,'r')
hold on
%k = 0:0.05:50;
%plot(c,k)
A = [x; y];
A(1,2749)  % x
A(2,2749)  % y
end