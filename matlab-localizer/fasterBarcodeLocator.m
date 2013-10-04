directory='~/data/DCIM';
% if nargin>1
%     directory = '~/data/DCIM/new/AllFail';
% end

fileList = dir(sprintf('%s/*.jpg', directory) ); % - the BAD guys!!!


%for i=11:11
for i=1:length( fileList )
    I = imread( sprintf('%s/%s',directory,fileList(i).name) );
    [ang, r1]=FastSobeLocator( I, 5 );
end
