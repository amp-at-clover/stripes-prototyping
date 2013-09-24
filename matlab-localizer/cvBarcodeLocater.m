function cvBarcodeLocater( fileName )
%
% Barcode localizer using OpenCV instead of Matlab's image processing
% toolbox.
% 
% Author: Arvind A. de Menezes Pereira
%
I = imread( fileName );
disp( fileName );

Igscale = cv.cvtColor( I, 'RGB2GRAY' );


Isharpened = Igscale - cv.Laplacian( Igscale, 'Scale',4 );

% Sharpen the image further with the sharpen mask
sharpMask = [ 0 -1 0 ; -1 5 -1 ; 0 -1 0 ];
Isharpened = cv.filter2D( Igscale, sharpMask );


%IgscaleBlurred = cv.GaussianBlur( Igscale, 'KSize',[ 3 3 ] );
% CV - addWeighted
%Iweighted = (1.5*double(Igscale) - 0.5 * double(IgscaleBlurred));

%Iweighted = uint8(Iweighted);
Iweighted = Isharpened;
Iequalized = Iweighted;
%Iequalized = cv.equalizeHist( Iweighted );
%I1 = 10; I2 = 4;
%Iequalized = uint8((I1 * double(Igscale) + I2 * double (Iequalized))/(I1+I2));

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
%KMeansLocator( Icanny2 );

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
    
    %Irotated2 = imrotate( Iequalized, 180/pi *rotAng2, 'bilinear','crop' );
    %subplot( 2, 3, 1), imshow( Irotated2 );
    
    [angle,ImgRotated, foundB,w1,w2,h1,h2 ] = GaborLocator( Iequalized );
    subplot( 2, 3, 1 ), imshow( ImgRotated );
    if foundB
        hold on;
        line([w1 w1],[h1 h2])
        line([w2 w2],[h1 h2])
        line([w1 w2],[h1 h1])
        line([w1 w2],[h2 h2])
        hold off;
    end
    title( sprintf('Rotated through Angle = %.1f',angle * 180/pi ) );
    
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