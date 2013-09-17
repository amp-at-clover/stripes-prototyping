function cvBarcodeLocater( fileName )
% Barcode localizer using OpenCV instead of Matlab's image processing
% toolbox.
% 
% Author: Arvind A. de Menezes Pereira
%
I = imread( fileName );

Igscale = cv.cvtColor( I, 'RGB2GRAY' );

Iequalized = cv.equalizeHist( Igscale );

CannyThresh = 120;
ApertureSize = 6;
L2Gradient = true;
Icanny1  = cv.Canny( Igscale, CannyThresh );
Icanny2 = cv.Canny( Iequalized, CannyThresh );


figure,subplot(2,3,1),imshow(I),title('Original');
subplot(2,3,2),imshow(Igscale),title('RGB2GRAY');
subplot(2,3,3),imshow(Iequalized),title('Equalized');
subplot(2,3,4),imshow(Icanny1),title('Canny-NE');
subplot(2,3,5),imshow(Icanny2),title('Canny-EQ');
lines1 = cv.HoughLines( Icanny1, 'Rho',1, 'Threshold', 150 );
lines2 = cv.HoughLines( Icanny2, 'Rho',1, 'Threshold', 150 );

%drawLines( lines );
rotAng1 = GetRotationAngle( lines1 );
rotAng2 = GetRotationAngle( lines2 );
    
Irotated1 = imrotate( Igscale, 180/pi * rotAng1 , 'bilinear', 'crop' );
subplot( 2, 3, 6), imshow( Irotated1 );

Irotated2 = imrotate( Iequalized, 180/pi *rotAng2, 'bilinear','crop' );
subplot( 2, 3, 1), imshow( Irotated2 );