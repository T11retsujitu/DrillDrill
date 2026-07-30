#include "convolve.hpp"

#include <algorithm>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。
cv::Mat convolve(const cv::Mat& src, const cv::Mat& kernel) {
  CV_Assert(src.channels() == 1);
  CV_Assert(src.depth() == CV_8U || src.depth() == CV_32F);
  CV_Assert(kernel.type() == CV_32F);
  CV_Assert(kernel.rows % 2 == 1 && kernel.cols % 2 == 1);

  // 入力の型によらず float で計算する。
  cv::Mat f;
  src.convertTo(f, CV_32F);

  const int kr = kernel.rows / 2;
  const int kc = kernel.cols / 2;
  cv::Mat dst(f.rows, f.cols, CV_32F);

  for (int y = 0; y < f.rows; ++y) {
    float* d = dst.ptr<float>(y);
    for (int x = 0; x < f.cols; ++x) {
      float acc = 0.0f;
      for (int j = -kr; j <= kr; ++j) {
        // 境界外は最も近い行/列に丸める（replicate）。
        const int yy = std::clamp(y + j, 0, f.rows - 1);
        const float* srow = f.ptr<float>(yy);
        const float* krow = kernel.ptr<float>(j + kr);
        for (int i = -kc; i <= kc; ++i) {
          const int xx = std::clamp(x + i, 0, f.cols - 1);
          acc += krow[i + kc] * srow[xx];
        }
      }
      d[x] = acc;
    }
  }
  return dst;
}
