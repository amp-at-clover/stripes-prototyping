% Test files from the directory with barcodes
fileList = dir('~/data/DCIM/*.jpg');

for i=1:length( fileList )
    cvBarcodeLocater( sprintf('~/data/DCIM/%s',fileList(i).name) );
end