function [foundB,w1,w2,h1,h2]=FindBarCodeInBinarizedImage( Img, bCodeWidth )
% Assumes we have already managed to get a fairly good estimate for 
% the rotated white portions of a rectangle.
    [ r,c ] = size( Img );
    
    bFound = false;
    % First pass - find number of white spaces.
    sweeps = zeros( r, 3 ); % numWhites, first, last
    for i=1:r
        I = find( Img(i,:)>0 );
        if length(I)>0
            first = find(Img(i,:)>0,1,'first');
            last =  find(Img(i,:)>0,1,'last');
            sweeps(i,:) = [ length(I) first last ]; 
        else
            sweeps(i,:) = [ 0 -1 -1 ];
        end
    end
    
    % Second pass - find candidate locations
    lastBoxWidth  = -1;
    lastBoxHeight = -1;
    h1=1; h2=r; w1=1; w2=c; i=1;
    foundB=false;
    while i<length(sweeps)
       [found, n,s,e,w]=findABox( sweeps, i, bCodeWidth );
       if found==true % We just found a box. Check if it is better.
           if (e-w)*(s-n)> lastBoxWidth * lastBoxHeight
                lastBoxWidth = (e-w);
                lastBoxHeight= (s-n);
                h1=n; h2=s; w1=w; w2=e;
                foundB=true;
           end
       end
       i=i+1;
    end

    function [found, n,s,e,w]=findABox( sweeps, r, bCodeWidth )
       onBox = false;
       n = length(sweeps); s=r; w=sweeps(r,2); e=sweeps(r,3);  
       maxWidth = 0;
       i=r;
       found=false;
       while i<length(sweeps)
           if sweeps(i,1) > bCodeWidth
              if onBox==false % First time we've found a box
                  n=i;
                  w=sweeps(i,2);
                  e=sweeps(i,3);
                  onBox=true;
                  found=true;
              else % we're on a box, but need to update bounds
                  s=i;
                  w=min(w,sweeps(i,2));
                  e=max(e,sweeps(i,3));
              end
           else
               %if sweeps(i,1)==0
               onBox = false;
               break;
               %end
           end
           i=i+1;
       end
    end

end