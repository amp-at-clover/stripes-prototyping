function rects = findRectangles( Img )
rects = [];
    contours = cv.findContours( Img, 'Mode', 'CComp' );
    for i=1:length(contours)
        approxCurv = cv.approxPolyDP( contours{i}, 'Epsilon',3,'Closed',true);
        rct = cv.boundingRect( approxCurv );
        DrawBoundingRect( rct );
        rects = vertcat( rects, rct );
    end
end