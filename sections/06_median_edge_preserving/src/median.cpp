#include "median.hpp"

#include <stdexcept>

// TODO(you): メディアンフィルタを実装してください
//   （仕様は include/median.hpp、解説は sections/06_median_edge_preserving/README.md）。
//
//   要点:
//   - CV_8U 入力 → CV_8U 出力。境界は replicate（std::clamp 方式、セクション 04 と同じ）。
//   - 近傍 ksize*ksize 個を std::vector<uchar> に集めて中央値を取る。
//   - バッファはループの外で 1 回だけ確保して使い回す。
cv::Mat median_filter(const cv::Mat& src, int ksize) {
  (void)src;
  (void)ksize;
  throw std::logic_error("TODO(you): median_filter を実装してください");
}
