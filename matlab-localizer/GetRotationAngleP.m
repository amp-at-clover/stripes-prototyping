function [rotAng, Theta, LineL] = GetRotationAngleP( lines )
    binWidth=0.2

    % Compute the rotation angle for the bar-code.
    Theta = zeros( 1, length(lines) );
    LineL = zeros( 1, length(lines) );
    X1 = zeros( 1, length( lines ) ); Y1 = X1; X2 = X1; Y2 = X1;
    
    for i=1:length( lines )
       x1 = lines{i}(1); x2 = lines{i}(3);
       y1 = lines{i}(2); y2 = lines{i}(4);
       X1(i) = x1; X2(i) = x2; Y1(i) = y1; Y2(i) = y2;
       
       Theta( i ) = atan2( double(y2-y1), double(x2-x1) );
       LineL( i ) = sqrt( double((x1-x2)^2 + (y1-y2)^2) );
    end
    %disp(Theta)

    % Compute the most likely rotations for these candidate lines
    binranges = [ 0:binWidth:pi ];
    bincounts = histc( Theta, binranges );

    % Find the angle with the maximum lines
    [mCount, angInd] = max( bincounts );
    disp( angInd );
    disp( binranges( angInd ) );

    rotAng = binranges( angInd );
end