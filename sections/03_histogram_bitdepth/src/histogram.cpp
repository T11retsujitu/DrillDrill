#include "histogram.hpp"

#include <stdexcept>

// TODO(you): 3 つの関数を実装してください（sections/03_histogram_bitdepth/README.md 参照）。

// 要件: 256 ビンの度数分布。総和が画素数に一致すること。src は CV_8U。
std::vector<int> compute_histogram(const cv::Mat& src) {
  (void)src;
  throw std::logic_error(
      "TODO(you): compute_histogram を実装してください");
}

// 要件: [in_min,in_max] → [0,255] の線形写像で CV_8U 化。区間外は飽和。src は変更しない。
//   小数は最近接の整数に丸める（cv::saturate_cast<uchar> が丸めと飽和を一度に行う）。
//   ヒント: まず convertTo で CV_64F 化すると、どのビット深度でも同じループで書ける。
cv::Mat rescale_to_8u(const cv::Mat& src, double in_min, double in_max) {
  (void)src;
  (void)in_min;
  (void)in_max;
  throw std::logic_error(
      "TODO(you): rescale_to_8u を実装してください");
}

// 要件: min-max ストレッチ。一様画像（max==min）は clone を返す。
//   ヒント: cv::minMaxLoc で最小・最大を求め、rescale_to_8u を再利用する。
cv::Mat stretch_contrast(const cv::Mat& src) {
  (void)src;
  throw std::logic_error(
      "TODO(you): stretch_contrast を実装してください");
}
