#include "reference_lab.hpp"

#include <stdexcept>

// TODO(you): a と b が同じ画素データ（メモリ）を共有していれば true を返す。
//   ヒント: cv::Mat はデータ本体を .data で指す。
bool shares_data(const cv::Mat& a, const cv::Mat& b) {
  (void)a;
  (void)b;
  throw std::logic_error(
      "TODO(you): shares_data を実装してください（sections/02_copy_reference_raii/README.md 参照）");
}

// TODO(you): src と共有しない独立コピーを返す。返り値を書き換えても src は不変。
//   ヒント: 深いコピーを行う Mat のメンバ関数がある。
cv::Mat independent_copy(const cv::Mat& src) {
  (void)src;
  throw std::logic_error(
      "TODO(you): independent_copy を実装してください（sections/02_copy_reference_raii/README.md 参照）");
}
