
% %fname = 'images/stable21_Mean.pgm';
% fname = 'HIGH-LGD-Panel1-Gei3494702_slide1-X0_530_Y0_1298_W_7023_H_6039_AF_Removed.jpg';
% im = imread(fname);
% im = mean(im,3);
% % weber-contrast normalization
% im1 = localBrightnessDiv(im,12,0.5);
% % take a look at the histograms before and after normalization
% figure; 
% [n,s] = hist(im(:),101);
% subplot(2,1,1);semilogy(s,n);title('Before Normalization');
% [n1,s1] = hist(im1(:),101);
% subplot(2,1,2); semilogy(s1,n1); title('After Normalization');
% % how about the images?
% figure; imshow(im,[0 255]);
% figure; imshow(im1);
% figure; imshow(im);
% imwrite(im1, 'HIGH-LGD-Panel1-Gei3494702_slide1-X0_530_Y0_1298_W_7023_H_6039_LocalBrighness.jpg');
% imwrite(im/255, 'HIGH-LGD-Panel1-Gei3494702_slide1-X0_530_Y0_1298_W_7023_H_6039_Mean.jpg');

fid = fopen('files.txt');
mydata = textscan(fid, '%s');
numSamples = size(mydata{1},1);
for i=1:numSamples
fnames{i} = ['Images\',mydata{1}{i}];
end

for i=1:numSamples
fname = fnames{i};
im = imread(fname);
h = fspecial('gaussian',64, 1);
im = imfilter(im,h,'replicate');
im = ordfilt2(im,4,[0 1 0; 1 1 1; 0 1 0]);
%im=im2double(im);
noiseMask = im < 50;
im(noiseMask) =0;
im = mean(im,3);
% weber-contrast normalization
im1 = localBrightnessDiv(im,12,0.5);
% take a look at the histograms before and after normalization
%[n,s] = hist(im(:),101);
%subplot(2,1,1);semilogy(s,n);title('Before Normalization');
%[n1,s1] = hist(im1(:),101);
%subplot(2,1,2); semilogy(s1,n1); title('After Normalization');
% how about the images?
%figure; imshow(im,[0 255]);
%figure; imshow(im1);
level = graythresh(im1);
Ik = im2bw(im1,level * .75);
Ik = bwareaopen(Ik, 50);
Ik = imfill(Ik,8,'holes');
%figure; imshow(Ik);

tname =strrep(mydata{1}{i},'.tif','-StructureMask.jpg');
imwrite(im1,[pwd,'\Results\',tname]);
end
