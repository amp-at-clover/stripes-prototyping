function gabor_test( image, varargin )
% arguments:
% imageName is the image we apply the Gabor wavelet to
% lambda is the Gabor wavelength
% theta is the orientation angle in degrees
% psi is the phase offset
% gamma is the aspect ratio
% bw is the Bandwidth of the filter
% N is the number of orientations
% For an explanation of these parameters see:
% http://matlabserver.cs.rug.nl/edgedetectionweb/web/edgedetection_params.html
nargin = length(varargin);
if nargin<1
    lambda  = 8;
else
    lambda = varargin{1};
end
if nargin<2
    theta   = 0;
else
    theta = varargin{2};
end
if nargin<3
    psi     = [0 pi/2];
else
    psi =  varargin{3};
end
if nargin<4
    gamma   = 0.5;
else
    gamma = varargin{4};
end
if nargin<5
    bw      = 1;
else
    bw = varargin{5};
end
if nargin<6
    N       = 8;
else
    N = varargin{6};
end
    
    img_in = im2double( image );
    %img_in(:,:,2:3) = [];   % discard redundant channels, it's gray anyway
    img_out = zeros(size(img_in,1), size(img_in,2), N);
    for n=1:N
        gb = gabor_fn(bw,gamma,psi(1),lambda,theta)...
            + 1i * gabor_fn(bw,gamma,psi(2),lambda,theta);
        % gb is the n-th gabor filter
        img_out(:,:,n) = imfilter(img_in, gb, 'symmetric');
        % filter output to the n-th channel
        theta = theta + 2*pi/N;
        % next orientation
    end
    subplot( 3,1,2 ),imshow(img_in);
    title('input image');
    img_out_disp = sum(abs(img_out).^2, 3).^0.5;
    % default superposition method, L2-norm
    img_out_disp = img_out_disp./max(img_out_disp(:));
    % normalize
    subplot( 3,1,3 ),imshow(img_out_disp);
    title('gabor output, L-2 super-imposed, normalized');