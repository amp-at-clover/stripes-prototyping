function KMeansLocator( I )
    [ y, x, val ] = find( I>0 );
    
    k = 4;
    
    
    P = [ x'; y' ]';
    
    [ labels, centers, compactness ] = cv.kmeans( P, k );
    
    imshow( I );
    
    I2 = zeros( size( I ) );
    
    hold on;
    for i=1:k
        pcolor( x(labels==i-1),y(labels==i-1), ones(size(labels))*i*10 );        
    end
    hold off;
    
end