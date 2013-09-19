function KMeansLocator( I )
    %[ y, x, val ] = find( I>0 );
    
    k = 3;
    
    
    P = [ x'; y' ]';
    
    [ labels, centers, compactness ] = cv.kmeans( P, k );
    
    imshow( I );
    
    I2 = zeros( size( I ) );
    
    hold on;
    for i=1:k
        I2( sub2ind( size(I2), y(labels==i-1), x(labels==i-1)) ) = mod(i*30,256);
    end
    
    image( I2 );
    colormap( hot(256) );
    ax = gca;
    linkaxes( ax, 'xy' );
    axis( ax, 'image' );
    for i=1:k
        plot( centers(i,1), centers(i,2),'r*','MarkerSize',14, ...
            'MarkerFaceColor','g');
    end
    
    hold off;
    
end