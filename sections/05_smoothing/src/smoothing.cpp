#include "smoothing.hpp"

#include "convolve.hpp"

#include <cmath>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。

cv::Mat make_box_kernel(int ksize) {
  CV_Assert(ksize >= 1 && ksize % 2 == 1);
  const float w = 1.0f / static_cast<float>(ksize * ksize);
  return cv::Mat(ksize, ksize, CV_32F, cv::Scalar(w)).clone();
}

cv::Mat make_gaussian_kernel(int ksize, double sigma) {
  CV_Assert(ksize >= 1 && ksize % 2 == 1);
  CV_Assert(sigma > 0.0);

  cv::Mat k(ksize, ksize, CV_32F);
  const int c = ksize / 2;
  double sum = 0.0;
  for (int j = 0; j < ksize; ++j) {
    for (int i = 0; i < ksize; ++i) {
      const double dy = j - c;
      const double dx = i - c;
      const double w = std::exp(-(dx * dx + dy * dy) / (2.0 * sigma * sigma));
      k.at<float>(j, i) = static_cast<float>(w);
      sum += w;
    }
  }
  // 総和 1 に正規化する（しないと画像全体が明るく/暗くなる）。
  k /= static_cast<float>(sum);
  return k;
}

cv::Mat box_blur(const cv::Mat& src, int ksize) {
  return convolve(src, make_box_kernel(ksize));
}

cv::Mat gaussian_blur(const cv::Mat& src, int ksize, double sigma) {
  return convolve(src, make_gaussian_kernel(ksize, sigma));
}
