function [angle,center] = GaborLocator( Img )
    center=[ 0, 0 ];
    angle =0;
    
    % First do a search through the image with Gabor
    N = 90;
    ThetaRes = pi/N;
    
    k = 2;
    
    IgPyr = cv.buildPyramid( Img, 'MaxLevel', k+1 );
    Responses = zeros(1,N);
    i=1;
    for theta = 0:ThetaRes:pi
       Igabor = gaborAtTheta( IgPyr{ k } , theta );
       Responses( i ) = sum( Igabor(:) );
       i=i+1;
    end
    [Val,Ind] = max( Responses );
    
    % The angle should be pretty easy to find now:
    angle = Ind*ThetaRes;
    bestResponse = gaborAtTheta( IgPyr{1}, angle );
    
    bb = max( bestResponse(:) );
    
    bestResponse = real(bestResponse./bb);

    Out = zeros( size(bestResponse) );
    % Now let us threshold the best response.
    Ind = find( bestResponse > 0.01*bb );
    Out( Ind ) = 255;
    
    imshow( Out );    
end