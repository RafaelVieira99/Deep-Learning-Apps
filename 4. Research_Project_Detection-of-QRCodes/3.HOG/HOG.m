I2 = imread('1.jpg');

corners   = detectFASTFeatures(im2gray(I2));
strongest = selectStrongest(corners,15);

[hog2,validPoints,ptVis] = extractHOGFeatures(I2,strongest);

figure;
imshow(I2);
hold on;
plot(ptVis,'Color','green');
hold off;


[hog1,visualization] = extractHOGFeatures(I2,'CellSize',[8 8]);
figure(2);
subplot(1,2,1);
imshow(I2);
subplot(1,2,2);
plot(visualization);


