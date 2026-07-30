#pragma once
#include <opencv2/core.hpp>

// --- セクション 08 のドリル関数群 ---
// 二値画像は「0 または 255 の CV_8U」で表す（OpenCV の慣習に合わせる）。
// すべての関数は入力（src / binary）を変更せず、新しい Mat を返す。

// 固定しきい値による二値化。src(y,x) > thresh なら 255、それ以外は 0。
//   src は CV_8U 1ch。src は変更しない。
cv::Mat threshold_binary(const cv::Mat& src, int thresh);

// 大津の方法によるしきい値の自動決定。
//   ヒストグラム（セクション 03 の compute_histogram を再利用）から、
//   クラス間分散 σ_B²(t) = ω0(t)·ω1(t)·(μ0(t) − μ1(t))² を最大にする t を返す。
//   t でしきい値処理するとき「v > t が前景」となる定義（cv::threshold の THRESH_OTSU と
//   同じ側）。最大が複数ある場合は最小の t を返す。
int otsu_threshold(const cv::Mat& src);

// 3x3 全要素の構造要素による収縮（erosion）。
//   binary は 0/255 の CV_8U。近傍 3x3 の**最小値**を出力する
//   （1 画素でも 0 があれば 0 になる = 白領域が痩せる）。
//   境界は replicate（画像外は最も近い画素と同じ扱い）。
cv::Mat erode3(const cv::Mat& binary);

// 3x3 全要素の構造要素による膨張（dilation）。近傍 3x3 の**最大値**。白領域が太る。
cv::Mat dilate3(const cv::Mat& binary);

// オープニング = 収縮 → 膨張。孤立した白ノイズ（salt）を除去する。
cv::Mat open3(const cv::Mat& binary);

// クロージング = 膨張 → 収縮。白領域内の小さな黒穴を埋める。
cv::Mat close3(const cv::Mat& binary);
