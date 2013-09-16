function cvBarcodeLocater( fileName )
% Barcode localizer using OpenCV instead of Matlab's image processing
% toolbox.
% 
% Author: Arvind A. de Menezes Pereira
%
I = imread( fileName );

Igscale = cv.cvtColor( I, 'RGB2GRAY' );

figure,subplot(2,1,1),imshow(I);
subplot(2,1,2),imshow(Igscale);