function imgout = gaborAtTheta( Img, theta )
%GABORATTHETA get a Gabor function at angle theta
%
% 
%
    img_in = im2double( Img );
    
    %lambda = 4;
    lambda=6; 
    psi=[0 pi/2]; gamma=0.5; bw=1;
    gb = gabor_fn(bw,gamma,psi(1),lambda,theta)...
            + 1i * gabor_fn(bw,gamma,psi(2),lambda,theta);
    img_out = zeros(size(img_in,1), size(img_in,2), 1);
    imgout = imfilter( img_in, gb, 'symmetric' );
end