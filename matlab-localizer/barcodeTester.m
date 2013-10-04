% Test files from the directory with barcodes
close all; 
%clear all; % This can delete all breakpoints. - Beware!!!

dataDir = '~/data/DCIM';            % - the original test set.
%dataDir = '~/data/DCIM/new/AllFail';

fileList = dir(sprintf('%s/*.jpg', dataDir) ); % - the BAD guys!!!


%for i=11:11
for i=1:length( fileList )
    cvBarcodeLocater( sprintf('%s/%s',dataDir,fileList(i).name) );
end