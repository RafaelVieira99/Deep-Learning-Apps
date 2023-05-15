
%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Features QR 
I1 = imread('2.jpg');
[hog1,visualization] = extractHOGFeatures(I1,'CellSize',[8 8]);
figure(2);
subplot(1,2,1);
imshow(I1);
subplot(1,2,2);
plot(visualization);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I2 = imread('1.jpg');
[hog2,visualization] = extractHOGFeatures(I2,'CellSize',[8 8]);

l1 = length(I1);
w1 = width(I1);
l2 = length(I2);
w2 = width(I2);

for i=1:l2
    for j=1:w2
        %if(I1(i:l1,i)==I2(i:l1,i))
        if(I1(i:l1,j)==I2(i:l1,j))
            fprintf('linhas done');
            %if(I1(i:w1)==I2(i:w1))
            qr(i) =1;
        else
            qr(i) = 0;
        end
       
    end
end