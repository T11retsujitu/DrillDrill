#include "histogram.hpp"

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。

std::vector<int> compute_histogram(const cv::Mat& src) {
  CV_Assert(src.type() == CV_8U);
  std::vector<int> hist(256, 0);
  for (int y = 0; y < src.rows; ++y) {
    const uchar* p = src.ptr<uchar>(y);
    for (int x = 0; x < src.cols; ++x) {
      ++hist[p[x]];
    }
  }
  return hist;
}

cv::Mat rescale_to_8u(const cv::Mat& src, double in_min, double in_max) {
  CV_Assert(src.channels() == 1);
  CV_Assert(in_max > in_min);

  // どのビット深度でも同じコードで扱えるよう、まず double に変換する。
  cv::Mat f;
  src.convertTo(f, CV_64F);

  cv::Mat dst(src.rows, src.cols, CV_8U);
  const double scale = 255.0 / (in_max - in_min);
  for (int y = 0; y < f.rows; ++y) {
    const double* s = f.ptr<double>(y);
    uchar* d = dst.ptr<uchar>(y);
    for (int x = 0; x < f.cols; ++x) {
      // saturate_cast が最近接丸め（.5 ちょうどは偶数側へ丸める cvRound 方式）と
      // 0..255 への飽和を一度に行う。
      d[x] = cv::saturate_cast<uchar>((s[x] - in_min) * scale);
    }
  }
  return dst;
}

cv::Mat stretch_contrast(const cv::Mat& src) {
  CV_Assert(src.type() == CV_8U);
  double mn = 0.0, mx = 0.0;
  cv::minMaxLoc(src, &mn, &mx);
  if (mx <= mn) {
    return src.clone();  // 一様な画像は伸長しようがない。
  }
  return rescale_to_8u(src, mn, mx);
}
