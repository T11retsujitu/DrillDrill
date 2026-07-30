#include "smoothing.hpp"

#include "convolve.hpp"

#include <stdexcept>

// TODO(you): 4 つの関数を実装してください（sections/05_smoothing/README.md 参照）。
//   box_blur / gaussian_blur は「カーネルを作って セクション 04 の convolve を呼ぶだけ」です。

// 要件: 全要素 1/(ksize*ksize) の CV_32F。ksize は正の奇数。
cv::Mat make_box_kernel(int ksize) {
  (void)ksize;
  throw std::logic_error("TODO(you): make_box_kernel を実装してください");
}

// 要件: w(j,i) ∝ exp(-((i-c)^2+(j-c)^2)/(2 sigma^2))、総和 1 に正規化。
cv::Mat make_gaussian_kernel(int ksize, double sigma) {
  (void)ksize;
  (void)sigma;
  throw std::logic_error("TODO(you): make_gaussian_kernel を実装してください");
}

cv::Mat box_blur(const cv::Mat& src, int ksize) {
  (void)src;
  (void)ksize;
  throw std::logic_error("TODO(you): box_blur を実装してください");
}

cv::Mat gaussian_blur(const cv::Mat& src, int ksize, double sigma) {
  (void)src;
  (void)ksize;
  (void)sigma;
  throw std::logic_error("TODO(you): gaussian_blur を実装してください");
}
