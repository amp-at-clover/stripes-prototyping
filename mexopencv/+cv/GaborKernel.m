%MYGABOR  Generate a Gabor function in Matlab
%
%    dst = cv.GaborKernel( bw, gamma, psi, lambda, theta )
%
% ## Input
% * __bw__ Bandwidth ( 1 by default )
% * __gamma__ Aspect Ratio ( 0.5 by default )
% * __psi__ Phase shift ( 0 by default )
% * __lambda__ Wave length ( should be >=2 )
% * __theta__ Angle in radians ( should be [0 pi) )
%
% ## Output
% * __dst__ Kernel matrix of size determined by 8*max(sigma_y, sigma_x).
%
% ## Options
%
%
% The function dilates the source image using the specified structuring element
% that determines the shape of a pixel neighborhood over which the maximum is
% taken. The function supports the in-place mode. Dilation can be applied
% several ( iterations ) times. In case of multi-channel images, each channel
% is processed independently.
%
% See also cv.erode
%
