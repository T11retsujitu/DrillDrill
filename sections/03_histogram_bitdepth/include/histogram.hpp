#pragma once
#include <opencv2/core.hpp>
#include <vector>

// --- セクション 03 のドリル関数群 ---

// 8bit(CV_8U) 1ch 画像のヒストグラム（256 ビンの度数分布）を返す。
//   hist[v] = 画素値 v を持つ画素の個数。総和は rows*cols に一致する。
std::vector<int> compute_histogram(const cv::Mat& src);

// 任意ビット深度の 1ch 画像を、区間 [in_min, in_max] → [0, 255] の線形写像で
// 8bit(CV_8U) に変換して返す。区間外の値は 0 / 255 に飽和させる。
//   例: 16bit 画像なら in_min=0, in_max=65535 で「全レンジ表示」になる。
//   要件: in_max > in_min。src は変更しない。
cv::Mat rescale_to_8u(const cv::Mat& src, double in_min, double in_max);

// 8bit 画像の線形コントラスト伸長（min-max ストレッチ）。
//   画像内の最小値→0、最大値→255 になるよう線形変換する。
//   全画素が同じ値（max==min）のときは、そのままのコピーを返す。
//   ヒント: 最小値・最大値を求めてから rescale_to_8u を再利用できる。
cv::Mat stretch_contrast(const cv::Mat& src);
