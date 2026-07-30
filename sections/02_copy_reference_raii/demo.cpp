// セクション 02 デモ: 浅いコピー / ROI / 深いコピーの違いを表示する。
#include "reference_lab.hpp"

#include <iostream>
#include <opencv2/core.hpp>

int main() {
  cv::Mat a = (cv::Mat_<uchar>(4, 4) << 1, 2, 3, 4, 5, 6, 7, 8,
               9, 10, 11, 12, 13, 14, 15, 16);

  cv::Mat shallow = a;                 // 代入は浅いコピー（ヘッダのみ複製）
  cv::Mat roi = a(cv::Rect(0, 0, 2, 2));  // ROI もデータを共有する
  cv::Mat deep = independent_copy(a);  // 深いコピー

  std::cout << std::boolalpha;
  std::cout << "shares_data(a, shallow) = " << shares_data(a, shallow) << "\n";
  std::cout << "shares_data(a, roi)     = " << shares_data(a, roi) << "\n";
  std::cout << "shares_data(a, deep)    = " << shares_data(a, deep) << "\n";

  // 浅いコピー経由で書き換えると a も変わる。
  shallow.at<uchar>(0, 0) = 99;
  std::cout << "after shallow write, a(0,0) = " << (int)a.at<uchar>(0, 0)
            << " (99 になるはず)\n";
  return 0;
}
