#pragma once
#include <opencv2/core.hpp>

// 各画素に対して dst = gain * src + bias を適用した新しい 8bit 画像を返す。
//
// 要件:
//   - 入力 src は 8bit 1ch (CV_8U)。
//   - 出力も CV_8U。0..255 の範囲に飽和させる（saturate）。負や 255 超はクリップ。
//   - src は変更しないこと（const 参照で受け取る）。返すのは別の Mat。
cv::Mat apply_gain_bias(const cv::Mat& src, double gain, double bias);
