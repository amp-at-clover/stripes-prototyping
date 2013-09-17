% Test files from the directory with barcodes
close all; clear all;

fileList = dir('~/data/DCIM/*.jpg');

for i=1:length( fileList )
    [lines,binranges,bincounts] = cvBarcodeLocater( sprintf('~/data/DCIM/%s',fileList(i).name) );
end