#pragma once
// dd/image_io.hpp — 画像の読み書き補助。
//
// 方針: WSL では cv::imshow が環境依存で不安定なため、本教材は「結果をファイルに書き出して
// 目で確認する」ことを基本にする。ここではその最小限の補助だけを提供する。

#include <opencv2/core.hpp>
#include <string>

namespace dd {

// グレースケール 1ch で読み込む。読み込めなければ std::runtime_error を投げる。
cv::Mat load_gray(const std::string& path);

// そのまま書き出す（8U/16U を想定）。失敗すれば std::runtime_error を投げる。
void save(const std::string& path, const cv::Mat& img);

// 表示用に書き出す。CV_32F など直接 imwrite できない型は、min/max を 0..255 に
// 正規化して 8bit にしてから保存する（値の絶対量は失われる点に注意）。
void save_view(const std::string& path, const cv::Mat& img);

}  // namespace dd
