#pragma once
#include <opencv2/core.hpp>

// 汎用の畳み込み（正確には相関 correlation）エンジン。
//
// 仕様:
//   - src    : 1ch 画像（CV_8U または CV_32F）。
//   - kernel : CV_32F、行数・列数ともに奇数。
//   - 戻り値 : CV_32F、src と同サイズ。
//   - 出力(y,x) = Σ_{j,i} kernel(j,i) * src(y + j - kr, x + i - kc)
//                 （kr = kernel.rows/2, kc = kernel.cols/2）
//   - 境界処理: 画像外は最も近い画素の値で埋める（BORDER_REPLICATE 相当）。
//   - src は変更しない。
//
// 注意: この定義は cv::filter2D と同じ「相関」であり、数学的な畳み込みは
//       カーネルを 180 度回転してから相関を取る。対称カーネルでは両者は一致する。
cv::Mat convolve(const cv::Mat& src, const cv::Mat& kernel);
