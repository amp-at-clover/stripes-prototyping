function rects = findRectangles( Img )
rects = [];
    Img = uint8( Img );
    contours = cv.findContours( Img, 'Mode', 'CComp' );
    for i=1:length(contours)
        approxCurv = cv.approxPolyDP( contours{i}, 'Epsilon',3,'Closed',true);
        rct = cv.boundingRect( approxCurv );
        if filterRect( rct )
            DrawBoundingRect( rct );
            rects = vertcat( rects, rct );
        end
    end
    
    
    function includeIt = filterRect( rct )
        lengthLim = 15; widthLim = 15;
    
        if rct(3) > lengthLim && rct(4) > widthLim
            includeIt = true;
        else
            includeIt = false;
        end
    end
    
end

