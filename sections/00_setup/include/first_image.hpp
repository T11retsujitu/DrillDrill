#pragma once
#include <opencv2/core.hpp>

// 256x256 の 8bit グレースケール画像を 1 枚作って返す。
// このセクションの目的は「環境が正しく動く」ことの確認なので、中身は簡単でよい。
cv::Mat make_first_image();
