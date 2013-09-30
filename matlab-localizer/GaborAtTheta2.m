function img_out = GaborAtTheta2( Img, theta )
% GABORATTHETA2 apply the gabor function at Theta
% using mexopencv this time
img_in = im2double( Img ); 
    gb = cv.getGaborKernel( 'KSize',15,'Sigma',6, ... %3.3730, ...
        'Theta', theta, 'Lambda',6, 'Psi', 0, 'Gamma', 0.5 );
    img_out = cv.filter2D( img_in, gb );    
end