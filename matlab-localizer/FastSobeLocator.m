function [angle,rects1] = FastSobeLocator( Img, subSamp )
    Img = cv.cvtColor( Img, 'RGB2GRAY' );
    IgPyr = cv.buildPyramid( Img );
    N = 180;
    degRes = 5;
    
    if nargin==1
        subSamp = 4;
    end
    degs = linspace( 0, 179, N/degRes );
    Responses = zeros( 1, N/degRes );
    sobThresh = 30;
    SobMask = [ -1 -2 -1 ; 0 0 0 ; 1 2 1 ]';
    ImSob = cv.filter2D( IgPyr{ subSamp }, SobMask );
    ImSob = cv.threshold( ImSob, sobThresh );
    Responses( 1 ) = sum( ImSob(:) );
    for i=2:length( degs )
        ImRot = imrotate( IgPyr{ subSamp }, degs( i )-1, 'bilinear', 'crop' );
        % Now find the best Sobel response
        ImSob = cv.filter2D( ImRot, SobMask );
        ImSob = cv.threshold( ImSob, sobThresh );
        Responses( i ) = sum( ImSob(:) );
    end
    [Val, Ind] = max( abs( Responses ) );
    angle = (Ind-1) * degRes;
    
    % Orientation found. Now for the locations:
    % Actual orientation is either at angle or angle+90 degrees
    
    % Find Gabor response to each of these
    ImRot1 = imrotate( Img, angle+90, 'bilinear', 'crop' );
    %ImRot2 = imrotate( Img, angle, 'bilinear', 'crop' );
    
    It1 = getGabResp( ImRot1 );
    %It2 = getGabResp( ImRot2 );
    
    figure,imshow(ImRot1);
    rects1 = findRectangles( It1 );
    
    %figure,imshow(ImRot2);
    %rects2 = findRectangles( It2 );
    
    function Ithr = getGabResp( Img )
       numIter = 3;
       thresh = 25;
       Igab = GaborAtTheta2( Img, 0 );
       Ilap = cv.Laplacian( Img );
       Idil = cv.dilate( Ilap, 'Iterations', numIter );
       Idil = uint8( Idil );
       Ithr = cv.threshold( Idil, thresh, 'Method','Binary','MaxValue',255 );
    end
end