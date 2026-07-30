#pragma once
#include <opencv2/core.hpp>

// cv::Mat の「浅いコピー（ヘッダ共有）」と「深いコピー」を理解するためのドリル。

// a と b が同じ画素データ（メモリ）を共有しているかを返す。
// ヒント: cv::Mat はデータ本体をポインタ(.data)で参照し、参照カウントで管理する。
bool shares_data(const cv::Mat& a, const cv::Mat& b);

// src とは独立した（データを共有しない）コピーを返す。
// 返り値を書き換えても src が変化しないこと。
cv::Mat independent_copy(const cv::Mat& src);
