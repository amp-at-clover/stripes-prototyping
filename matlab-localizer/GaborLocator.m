function [angle,ImgRotated, foundB, w1,w2,h1,h2] = GaborLocator( Img )
    center=[ 0, 0 ];
    angle =0;
    
    % First do a search through the image with Gabor
    N = 180;
    ThetaRes = pi/N;
    
    k = 3;
    thresh = 0.2;
    
    IgPyr = cv.buildPyramid( Img, 'MaxLevel', k+1 );
    Responses = zeros(1,N);
    i=1;
    %k =1;
    for theta = 0:ThetaRes:pi
       Igabor = GaborAtTheta2( IgPyr{ k } , theta );
       % Now let us threshold the best response.
       %Ind = find( Igabor > thresh );
       %Igabor( Ind ) = 255;
       Responses( i ) = sum( Igabor(:) );
       i=i+1;
    end
    [Val,Ind] = max( abs(Responses) );
    
    % The angle should be pretty easy to find now:
    angle = (Ind-1)*ThetaRes;
    bestResponse = GaborAtTheta2( IgPyr{1}, angle );
    
    bb = max( bestResponse(:) );
    
    bestResponse = real(bestResponse./bb);
    Out = cv.Sobel( bestResponse );
    
    Ind = find( Out > thresh );
    Out = zeros( size(bestResponse) );
    Out( Ind ) = 255;
    
    % Now run a sobel mask on this image.
    
    imshow( Out );
    ImgRotated = imrotate( Img, 180/pi * angle, 'bilinear', 'crop' );
    OutRotated = imrotate( Out, 180/pi * angle, 'bilinear', 'crop' );
    
    %gabResp = gaborAtTheta( ImgRotated, 0 );
    
    IrotPyr = cv.buildPyramid( OutRotated );
    
    k2=4; % Image Pyramid indexed for location finding.
    [ r, c ] = size( IrotPyr{ k2 } );
    [foundB, w1,w2, h1, h2] = FindBarCodeInBinarizedImage( IrotPyr{4},4 );
    if foundB==true
        h1=h1-1; h2=h2+1; w1=w1-1; w2=w2+1;
        h1=max(1,h1); h2=min(h2,r); w1=max(1,w1); w2=min(w2,c);
        scaleFactor=2^(k2-1);
        
        h1 = h1*scaleFactor;
        h2 = h2*scaleFactor;
        w1 = w1*scaleFactor;
        w2 = w2*scaleFactor;
    end
end