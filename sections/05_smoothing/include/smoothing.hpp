#pragma once
#include <opencv2/core.hpp>

// --- セクション 05 のドリル関数群 ---
// 平滑化はカーネル設計 + セクション 04 の convolve でできる。

// ksize x ksize の box（平均化）カーネルを返す。
//   要件: CV_32F、全要素が等しく、総和が 1。ksize は正の奇数。
cv::Mat make_box_kernel(int ksize);

// ksize x ksize のガウシアンカーネルを返す。
//   w(j,i) ∝ exp(-((i-c)^2 + (j-c)^2) / (2*sigma^2))   （c = ksize/2）
//   要件: CV_32F、総和が 1（正規化する）。ksize は正の奇数、sigma > 0。
cv::Mat make_gaussian_kernel(int ksize, double sigma);

// box カーネルによる平滑化。出力は CV_32F（convolve の仕様に従う）。
cv::Mat box_blur(const cv::Mat& src, int ksize);

// ガウシアンカーネルによる平滑化。出力は CV_32F。
cv::Mat gaussian_blur(const cv::Mat& src, int ksize, double sigma);
