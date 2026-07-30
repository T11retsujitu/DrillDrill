#include "brightness.hpp"

#include <opencv2/core.hpp>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。
cv::Mat apply_gain_bias(const cv::Mat& src, double gain, double bias) {
  CV_Assert(src.type() == CV_8U);

  // src を書き換えないよう、出力は別に確保する。
  cv::Mat dst(src.rows, src.cols, CV_8U);

  for (int y = 0; y < src.rows; ++y) {
    const uchar* s = src.ptr<uchar>(y);
    uchar* d = dst.ptr<uchar>(y);
    for (int x = 0; x < src.cols; ++x) {
      // saturate_cast が 0..255 へのクリップ（飽和）を行う。
      d[x] = cv::saturate_cast<uchar>(gain * s[x] + bias);
    }
  }
  return dst;
}
