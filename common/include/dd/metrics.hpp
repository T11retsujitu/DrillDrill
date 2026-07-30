#pragma once
// dd/metrics.hpp — ノイズ低減結果の評価指標。
//
// 本教材の評価方針: PSNR / SSIM だけに頼らず、ラインプロファイル等の定性確認や
// タスク指向（CD 計測誤差など）と組み合わせて判断する。詳細は docs/evaluation.md。

#include <opencv2/core.hpp>
#include <vector>

namespace dd {

// 平均二乗誤差。a, b は同じサイズ・同じチャンネル数であること。
double mse(const cv::Mat& a, const cv::Mat& b);

// PSNR [dB]。max_val は信号のとりうる最大値（8bit なら 255）。
// a == b（mse==0）のときは +infinity を返す。
double psnr(const cv::Mat& a, const cv::Mat& b, double max_val = 255.0);

// 平均 SSIM（Wang et al. 2004、11x11 ガウス窓）。1.0 に近いほど構造が保たれている。
double ssim(const cv::Mat& a, const cv::Mat& b);

// 指定行のラインプロファイル（画素値の並び）を返す。エッジ保存性の目視確認に使う。
std::vector<double> line_profile(const cv::Mat& img, int row);

}  // namespace dd
