clear all; close all;
serialPort = 'COM4';
s = serial(serialPort, 'baudrate', 57600);
fopen(s);
counter = 1;
figure;
hold on
while counter > 0
    first(counter) = fscanf(s,'%d');
    second(counter) = fscanf(s,'%d');
    thresh = 8190;
    offset = 50;
    if (first(counter) > thresh || second(counter) > thresh)
    else
    plot(counter,first(counter),'ro',counter,second(counter),'bo');
    if (counter <= 100)
        fmax = max(first(1:counter));
        smax = max(second(1:counter));
        fmin = min(first(1:counter));
        smin = min(second(1:counter));
        maxlimit = max([fmax,smax]);
        minlimit = min([fmin,smin]);
        if minlimit <= offset
            minlimit = 50;
        end
        axis([0, counter, minlimit-offset, maxlimit+offset]);
    else
        fmax = max(first(counter-100:counter));
        smax = max(second(counter-100:counter));
        fmin = min(first(counter-100:counter));
        smin = min(second(counter-100:counter));
        maxlimit = max([fmax,smax]);
        minlimit = min([fmin,smin]);
        if minlimit <= offset
            minlimit = 50;
        end
        axis([counter - 100, counter, minlimit-offset, maxlimit+offset]);
    end
    drawnow;
    counter = counter + 1;
    end
    
end
hold off
fclose(s);