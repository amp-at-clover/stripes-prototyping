% Arvind's Localizer
clear all; close all;

Thresh = 0.05;

fileList = dir('~/data/DCIM/*.jpg');
for i =1:length(fileList)
    I = imread( sprintf('~/data/DCIM/%s',fileList(i).name) );
    figure; subplot(3,1,1),imshow(I),title(sprintf('Original Image: %s',fileList(i).name));
    subplot(3,1,2);
    Ihsv = rgb2gray( I );
    Icanny = edge( Ihsv, 'canny', Thresh );

    % Setup parameters for the Gabor test
    lambda  = 3;
    theta   = 0;
    psi     = [0 pi/2];
    gamma   = 0.5;
    bw      = 1;
    N       = 10;
    % Test it
    gabor_test( Icanny, lambda, theta, psi, gamma, bw, N );
end