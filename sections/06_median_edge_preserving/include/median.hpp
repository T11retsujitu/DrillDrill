#pragma once
#include <opencv2/core.hpp>

// メディアン（中央値）フィルタ。
//
// 仕様:
//   - src   : CV_8U 1ch。
//   - ksize : 正の奇数（3 なら 3x3 近傍）。
//   - 出力  : CV_8U、src と同サイズ。各画素は近傍 ksize*ksize 個の**中央値**。
//   - 境界処理: replicate（セクション 04 と同じ。座標を画像内に丸める）。
//   - src は変更しない。
//
// ヒント: 近傍の値を std::vector<uchar> に集めて中央値を取る。
//         全ソートでもよいが、std::nth_element なら中央値だけを効率よく選べる。
cv::Mat median_filter(const cv::Mat& src, int ksize);
