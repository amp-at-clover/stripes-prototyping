function [lines, binranges, bincounts] = cvBarcodeLocater( fileName )
% Barcode localizer using OpenCV instead of Matlab's image processing
% toolbox.
% 
% Author: Arvind A. de Menezes Pereira
%
I = imread( fileName );

Igscale = cv.cvtColor( I, 'RGB2GRAY' );

CannyThresh = 120;
ApertureSize = 5;
L2Gradient = true;
Icanny  = cv.Canny( Igscale, CannyThresh );

figure,subplot(2,2,1),imshow(I),title('Original');
subplot(2,2,2),imshow(Igscale),title('RGB2GRAY');
subplot(2,2,3),imshow(Icanny),title('Canny');
lines = cv.HoughLines( Icanny, 'Rho',1, 'Threshold', 80 );
%drawLines( lines );

    
linesInMat = cell2mat( lines ); % Convert from Cell array to Matrix
Rho = linesInMat( 1:2:end );
Theta = linesInMat( 2:2:end );
% Compute the most likely rotations for these candidate lines
binranges = [ 0:0.05:pi ];
bincounts = histc( Theta, binranges );
subplot(2,2,2),bar( binranges, bincounts, 'histc' ), xlim( [ 0, pi ]),title('Histogram');

% Find the angle with the maximum lines
[mCount, angInd] = max( bincounts );
disp( binranges( angInd ) );
Irotated = imrotate( Icanny, -180/pi * binranges( angInd ), 'bilinear', 'crop' );
subplot( 2, 2, 4), imshow( Irotated );