#include "edges.hpp"

#include "convolve.hpp"

#include <stdexcept>

// TODO(you): 4 つの関数を実装してください
//   （仕様は include/edges.hpp、解説は sections/07_derivative_edges/README.md）。
//   sobel_x / sobel_y / laplacian4 は「カーネルを cv::Mat_<float> で定義して
//   セクション 04 の convolve を呼ぶだけ」です。カーネルの数値は hpp に書いてあります。

cv::Mat sobel_x(const cv::Mat& src) {
  (void)src;
  throw std::logic_error("TODO(you): sobel_x を実装してください");
}

cv::Mat sobel_y(const cv::Mat& src) {
  (void)src;
  throw std::logic_error("TODO(you): sobel_y を実装してください");
}

// 要件: 要素ごとに sqrt(gx^2 + gy^2)。ループでも cv::magnitude でもよい。
cv::Mat gradient_magnitude(const cv::Mat& gx, const cv::Mat& gy) {
  (void)gx;
  (void)gy;
  throw std::logic_error("TODO(you): gradient_magnitude を実装してください");
}

cv::Mat laplacian4(const cv::Mat& src) {
  (void)src;
  throw std::logic_error("TODO(you): laplacian4 を実装してください");
}
