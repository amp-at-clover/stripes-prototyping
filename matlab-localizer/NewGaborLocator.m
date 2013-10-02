function [angle,ImgRotated] = NewGaborLocator( Img )
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
    ImgRotated = imrotate( Img, 180/pi *angle, 'bilinear', 'crop' );
    
    bestResponse = GaborAtTheta2( IgPyr{1}, angle );
    bestResponse = imrotate( bestResponse, 180/pi * angle, 'bilinear', 'crop' );
    
    % --- Now the new magic ---
    Ig = cv.Laplacian( bestResponse, 'Scale', 0.5 );
    Idil = cv.dilate( Ig, 'Iterations', 8 );
    imshow( ImgRotated );
    Rcts = findRectangles( Idil );
    
    foundB = true;
    

end