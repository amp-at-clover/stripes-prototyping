function cvBarcodeLocater( fileName )
% Barcode localizer using OpenCV instead of Matlab's image processing
% toolbox.
% 
% Author: Arvind A. de Menezes Pereira
%
I = imread( fileName );

Igscale = cv.cvtColor( I, 'RGB2GRAY' );

Iequalized = cv.equalizeHist( Igscale );

I1 = 10; I2 = 5;
Iequalized = uint8((I1 * double(Igscale) + I2 * double (Iequalized))/(I1+I2));

CannyThresh = 50;
ApertureSize = 3;
L2Gradient = true;

Icanny1  = cv.Canny( Igscale, CannyThresh, 'ApertureSize', ApertureSize,...
    'L2Gradient', L2Gradient );
Icanny2 = cv.Canny( Iequalized, 1.5*CannyThresh,'ApertureSize', ApertureSize,...
    'L2Gradient', L2Gradient );

figure,subplot(2,3,1),imshow(I),title('Original');
subplot(2,3,2),imshow(Igscale),title('RGB2GRAY');
subplot(2,3,3),imshow(Iequalized),title('Equalized');
KMeansLocator( Iequalized );

useHoughLinesP = 1;

HoughThreshold = 45;
HoughRho =1.5;
HoughMaxLineGap = 3;
HoughMinLineLength = 40;

if useHoughLinesP % Advanced probabilistic Hough (line) space search
    lines1 = cv.HoughLinesP( Icanny1, 'Rho',HoughRho,'Threshold',HoughThreshold, ...
        'MinLineLength', HoughMinLineLength, ...
        'MaxLineGap',HoughMaxLineGap );
    lines2 = cv.HoughLinesP( Icanny2, 'Rho',HoughRho,'Threshold',HoughThreshold, ...
        'MinLineLength', HoughMinLineLength, ...
        'MaxLineGap',HoughMaxLineGap );
    [ rotAng1, Theta1, LineL1 ] = GetRotationAngleP( lines1 );
    [ rotAng2, Theta2, LineL2 ] = GetRotationAngleP( lines2 );
    
    Irotated1 = imrotate( Igscale, 180/pi * rotAng1 , 'bilinear', 'crop' );
    subplot( 2, 3, 6), imshow( Irotated1 );
    
    Irotated2 = imrotate( Iequalized, 180/pi *rotAng2, 'bilinear','crop' );
    subplot( 2, 3, 1), imshow( Irotated2 );
      
else % Generalized Hough line space search    
    lines1 = cv.HoughLines( Icanny1, 'Rho',HoughRho, 'Threshold', HoughThreshold );
    lines2 = cv.HoughLines( Icanny2, 'Rho',HoughRho, 'Threshold', HoughThreshold );
    rotAng1 = GetRotationAngle( lines1 );
    rotAng2 = GetRotationAngle( lines2 );
    
    Irotated1 = imrotate( Igscale, 180/pi * rotAng1 , 'bilinear', 'crop' );
    subplot( 2, 3, 6), imshow( Irotated1 );

    Irotated2 = imrotate( Iequalized, 180/pi *rotAng2, 'bilinear','crop' );
    subplot( 2, 3, 1), imshow( Irotated2 );
end
    
subplot(2,3,4),imshow(Icanny1),title('Canny-NE');
if useHoughLinesP,  drawLines( lines1 ); end
subplot(2,3,5),imshow(Icanny2),title('Canny-EQ');
if useHoughLinesP, drawLines( lines2 ); end