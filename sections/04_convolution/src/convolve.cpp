#include "convolve.hpp"

#include <stdexcept>

// TODO(you): 汎用の畳み込み（相関）を実装してください
//   （仕様は include/convolve.hpp、解説は sections/04_convolution/README.md）。
//
//   要点:
//   - 出力は CV_32F。入力は convertTo で float 化してから計算すると楽。
//   - 境界は replicate。座標を 0..n-1 に丸める（<algorithm> の std::clamp が使える）。
//   - 4 重ループ（y, x, カーネル j, i）の素朴な実装でよい。まず正しく。
cv::Mat convolve(const cv::Mat& src, const cv::Mat& kernel) {
  (void)src;
  (void)kernel;
  throw std::logic_error("TODO(you): convolve を実装してください");
}
