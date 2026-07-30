#pragma once
#include <opencv2/core.hpp>

// --- セクション 07 のドリル関数群 ---
// すべてセクション 04 の convolve（相関・replicate 境界）で実装できる。
// 微分は負の値を持つので、出力はすべて CV_32F（8bit にすると符号が失われる）。
// すべての関数は入力を変更せず、新しい Mat を返す。

// 横方向 Sobel（縦エッジ検出）。カーネル:
//   [-1 0 +1]
//   [-2 0 +2]
//   [-1 0 +1]
// 暗→明（左が暗く右が明るい）の縦エッジで正の値になる。
cv::Mat sobel_x(const cv::Mat& src);

// 縦方向 Sobel（横エッジ検出）。sobel_x のカーネルを転置したもの。
// 暗→明（上が暗く下が明るい）の横エッジで正の値になる。
cv::Mat sobel_y(const cv::Mat& src);

// 勾配強度 |∇I| = sqrt(gx^2 + gy^2)。gx, gy は同サイズの CV_32F。出力も CV_32F。
cv::Mat gradient_magnitude(const cv::Mat& gx, const cv::Mat& gy);

// 4 近傍ラプラシアン。カーネル:
//   [ 0  1  0]
//   [ 1 -4  1]
//   [ 0  1  0]
// 平坦部で 0、エッジの両側で符号が反転する（ゼロ交差）。
cv::Mat laplacian4(const cv::Mat& src);
