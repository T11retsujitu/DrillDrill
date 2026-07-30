#include "median.hpp"

#include <algorithm>
#include <vector>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。
cv::Mat median_filter(const cv::Mat& src, int ksize) {
  CV_Assert(src.type() == CV_8U);
  CV_Assert(ksize >= 1 && ksize % 2 == 1);

  const int r = ksize / 2;
  const int n = ksize * ksize;
  cv::Mat dst(src.rows, src.cols, CV_8U);

  // ループの外で確保してバッファを使い回す（毎画素 new しない）。
  std::vector<uchar> window(n);

  for (int y = 0; y < src.rows; ++y) {
    uchar* d = dst.ptr<uchar>(y);
    for (int x = 0; x < src.cols; ++x) {
      int idx = 0;
      for (int j = -r; j <= r; ++j) {
        const int yy = std::clamp(y + j, 0, src.rows - 1);
        const uchar* srow = src.ptr<uchar>(yy);
        for (int i = -r; i <= r; ++i) {
          const int xx = std::clamp(x + i, 0, src.cols - 1);
          window[idx++] = srow[xx];
        }
      }
      // 中央値だけが欲しいので全ソートは不要。nth_element で十分。
      std::nth_element(window.begin(), window.begin() + n / 2, window.end());
      d[x] = window[n / 2];
    }
  }
  return dst;
}
