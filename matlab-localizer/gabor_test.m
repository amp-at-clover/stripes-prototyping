function gabor_test( image, varargin )
    % gabor_test( image, varargin ):
    % A function that takes as input a matrix specifying a 2D image
    % and applies a 2D Gabor wavelet filter to this function.
    % The optional arguments specify the type of the Gabor function.
    % 
    % arguments:
    % imageName is the image we apply the Gabor wavelet to. (required)
    % Optional arguments to be passed: eg. ...,'lambda',8,...
    % lambda is the Gabor wavelength
    % theta is the orientation angle in degrees
    % psi is the phase offset
    % gamma is the aspect ratio
    % bw is the Bandwidth of the filter
    % N is the number of orientations
    % For an explanation of these parameters see:
    % http://matlabserver.cs.rug.nl/edgedetectionweb/web/edgedetection_params.html  
    
    % Create an input parser object.
    p = inputParser;
    % Setup Default values.
    defaultLambda = 8;
    defaultTheta = 0;
    defaultPsi = [ 0 pi/2 ];
    defaultGamma = 0.5;
    defaultBW = 1;
    defaultN = 8;
    
    % Setup Validation
    addRequired( p, 'image' );
    addOptional( p, 'lambda', defaultLambda, @isnumeric );
    addOptional( p, 'theta', defaultTheta, @isnumeric );
    addOptional( p, 'psi', defaultPsi );
    addOptional( p, 'gamma', defaultGamma, @isnumeric );
    addOptional( p, 'bw', defaultBW, @isnumeric );
    addOptional( p, 'N' , defaultN, @isnumeric );
    
    % Set properties to adjust parsing
    p.KeepUnmatched = true;
    
    % Parse the inputs.
    parse( p, image, varargin{:});
    
    % Assign the results to the variables we want to use
    image  = p.Results.image;
    lambda = p.Results.lambda;
    theta  = p.Results.theta;
    psi    = p.Results.psi;
    gamma  = p.Results.gamma;
    bw     = p.Results.bw;
    N      = p.Results.N;
    
    if ~isempty(fieldnames(p.Unmatched))
       disp('Extra inputs:')
       disp(p.Unmatched)
    end
    if ~isempty(p.UsingDefaults)
       disp('Using defaults: ')
       disp(p.UsingDefaults)
    end
    
    % Use the inputs in our function.
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
    subplot( 2,2,3 ),imshow(img_in);
    title('input image');
    img_out_disp = sum(abs(img_out).^2, 3).^0.5;
    % default superposition method, L2-norm
    img_out_disp = img_out_disp./max(img_out_disp(:));
    % normalize
    subplot( 2,2,4 ),imshow(img_out_disp);
    title('gabor output, L-2 super-imposed, normalized');