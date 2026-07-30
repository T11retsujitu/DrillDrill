#include "binarize.hpp"

#include "histogram.hpp"  // セクション 03 の compute_histogram を再利用できる

#include <stdexcept>

// TODO(you): 6 つの関数を実装してください
//   （仕様は include/binarize.hpp、解説は sections/08_threshold_morphology/README.md）。

// 要件: v > thresh なら 255、それ以外 0。src は変更しない。
cv::Mat threshold_binary(const cv::Mat& src, int thresh) {
  (void)src;
  (void)thresh;
  throw std::logic_error("TODO(you): threshold_binary を実装してください");
}

// 要件: クラス間分散を最大にする t（同点なら最小の t）。
//   ヒント: compute_histogram の結果から、クラス0(<=t)/クラス1(>t) の
//   画素数と輝度和を t のループで逐次更新すると O(256) で書ける。
int otsu_threshold(const cv::Mat& src) {
  (void)src;
  throw std::logic_error("TODO(you): otsu_threshold を実装してください");
}

// 要件: 3x3 近傍の最小値（境界は replicate）。
cv::Mat erode3(const cv::Mat& binary) {
  (void)binary;
  throw std::logic_error("TODO(you): erode3 を実装してください");
}

// 要件: 3x3 近傍の最大値（境界は replicate）。
cv::Mat dilate3(const cv::Mat& binary) {
  (void)binary;
  throw std::logic_error("TODO(you): dilate3 を実装してください");
}

// 要件: erode → dilate の合成。
cv::Mat open3(const cv::Mat& binary) {
  (void)binary;
  throw std::logic_error("TODO(you): open3 を実装してください");
}

// 要件: dilate → erode の合成。
cv::Mat close3(const cv::Mat& binary) {
  (void)binary;
  throw std::logic_error("TODO(you): close3 を実装してください");
}
