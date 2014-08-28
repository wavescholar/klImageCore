function resIm = localBrightnessDiv(images, sigmaBlur, sigmaLogit);
% function resIm = localBrightnessDiv(images, sigmaBlur, sigmaLogit)
%  Input:
%    images is a nrow x ncol x nImages matrix containing grey levels >=0.
%    sigmaBlur = std dev of gaussian blur (3.0)
%    sigmaLogit = sigma for logit response (0.3);
%  Output:
%    resIm(iy, ix, k) = logit(images(iy,ix,k)/blurImage(iy,ix,k) - 1)
%    where logit(x) = (1 - exp(-lam x))/(1+exp(-lam x)) and
%    lam is chosen such that logit(sigmaLogit) = 0.5
  
  sizeIm = size(images);
  nImages = size(images,3);
  sizeIm = sizeIm(1:2);

  lam = -log(0.5/1.5)/sigmaLogit;
  
  %% Gaussian Filter
  sigmaSqr = sigmaBlur*sigmaBlur;
  gFiltSize = 2 * round(3.0 * sigmaBlur) + 1;
  x = [1:gFiltSize] - round((gFiltSize+1)/2);
  gFilt = exp(- x .* x / (2.0*sigmaSqr));
  gFilt = gFilt/ sum(gFilt(:));

  %%% Local Brightness division on non-eyes
  resIm = zeros([sizeIm, nImages]);
  for k = 1:nImages
    resIm(:,:, k) = rconv2sep(images(:,:,k), gFilt, gFilt);
    resIm(:,:,k) = max(resIm(:,:,k),eps);
    resIm(:,:,k) = images(:,:,k)./resIm(:,:,k);
  end
  resIm = reshape(resIm, prod(sizeIm), nImages);

  idx = resIm(:) > 1+10*sigmaLogit;
  if any(idx)
    resIm(idx) = 1+10*sigmaLogit;
  end
  idx = resIm(:) < 1-10*sigmaLogit;
  if any(idx)
    resIm(idx) = 1-10*sigmaLogit;
  end
  resIm = resIm - 1;
  resIm = (1 - exp(-lam*resIm))./(1+exp(-lam*resIm));

  resIm = reshape(resIm, [sizeIm nImages]);
  return;
