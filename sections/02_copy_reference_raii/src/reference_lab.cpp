#include "reference_lab.hpp"

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。

bool shares_data(const cv::Mat& a, const cv::Mat& b) {
  // 同じデータ本体を指していれば .data ポインタが一致する。
  return a.data == b.data;
}

cv::Mat independent_copy(const cv::Mat& src) {
  // clone() はデータ本体まで複製する（深いコピー）。
  return src.clone();
}
