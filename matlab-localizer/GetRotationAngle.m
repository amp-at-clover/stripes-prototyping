function rotAng = GetRotationAngle( lines )
    
linesInMat = cell2mat( lines ); % Convert from Cell array to Matrix
Rho = linesInMat( 1:2:end );
Theta = linesInMat( 2:2:end );
% Compute the most likely rotations for these candidate lines
binranges = [ 0:0.1:pi ];
bincounts = histc( Theta, binranges );
%subplot(3,2,2),bar( binranges, bincounts, 'histc' ), xlim( [ 0, pi ]),title('Histogram');

% Find the angle with the maximum lines
[mCount, angInd] = max( bincounts );
disp( angInd );
disp( binranges( angInd ) );

rotAng = binranges( angInd );

end