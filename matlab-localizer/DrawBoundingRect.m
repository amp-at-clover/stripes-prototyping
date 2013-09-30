function DrawBoundingRect( coods )
    hold on;
    
    w = coods(3); h = coods(4);
    x1=coods(1); x2 = x1+w;
    y1=coods(2); y2 = y1+h;
    plot( [x1 x1], [y1 y2], 'r-','LineWidth',2 );
    plot( [x1 x2], [y1 y1], 'r-','LineWidth',2 );
    plot( [x2 x2], [y1 y2], 'r-','LineWidth',2 );
    plot( [x2 x1], [y2 y2], 'r-','LineWidth',2 );
    hold off;
end