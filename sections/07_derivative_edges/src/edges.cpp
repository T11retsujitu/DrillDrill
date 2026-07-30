#include "edges.hpp"

#include "convolve.hpp"

#include <opencv2/core.hpp>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。

cv::Mat sobel_x(const cv::Mat& src) {
  const cv::Mat k = (cv::Mat_<float>(3, 3) << -1, 0, 1,
                                              -2, 0, 2,
                                              -1, 0, 1);
  return convolve(src, k);
}

cv::Mat sobel_y(const cv::Mat& src) {
  const cv::Mat k = (cv::Mat_<float>(3, 3) << -1, -2, -1,
                                               0,  0,  0,
                                               1,  2,  1);
  return convolve(src, k);
}

cv::Mat gradient_magnitude(const cv::Mat& gx, const cv::Mat& gy) {
  CV_Assert(gx.type() == CV_32F && gy.type() == CV_32F);
  CV_Assert(gx.size() == gy.size());
  cv::Mat mag;
  cv::magnitude(gx, gy, mag);  // sqrt(gx^2 + gy^2) を要素ごとに計算
  return mag;
}

cv::Mat laplacian4(const cv::Mat& src) {
  const cv::Mat k = (cv::Mat_<float>(3, 3) << 0,  1, 0,
                                              1, -4, 1,
                                              0,  1, 0);
  return convolve(src, k);
}
