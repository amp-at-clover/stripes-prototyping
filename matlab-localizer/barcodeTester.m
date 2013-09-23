% Test files from the directory with barcodes
close all; 
%clear all; % This can delete all breakpoints. - Beware!!!

fileList = dir('~/data/DCIM/*.jpg');

%for i=2:4
for i=1:length( fileList )
    cvBarcodeLocater( sprintf('~/data/DCIM/%s',fileList(i).name) );
end