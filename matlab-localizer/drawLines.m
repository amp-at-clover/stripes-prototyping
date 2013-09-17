function drawLines( lines )
% Helper function that draws lines from all the lines
% detected in the image by cv.HoughLinesP
    hold on;
    lenThreshold = 10.0;
    for i=1:length(lines)
        x1 = lines{i}(1); x2 = lines{i}(3);
        y1 = lines{i}(2); y2 = lines{i}(4);       
        
        if sqrt( double((x1-x2)^2 + (y1-y2)^2) )> lenThreshold
            plot( [x1 x2], [y1 y2], 'y-' );
        end
    end
    hold off;
end