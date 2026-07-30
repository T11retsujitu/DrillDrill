// セクション 00 デモ: 画像を 1 枚作って保存する。
//   使い方: ./sec00_demo [出力パス]   （省略時 first_image.png）
#include "first_image.hpp"
#include "dd/image_io.hpp"

#include <iostream>

int main(int argc, char** argv) {
  const std::string path = (argc > 1) ? argv[1] : "first_image.png";
  try {
    cv::Mat img = make_first_image();
    std::cout << "size = " << img.cols << "x" << img.rows
              << ", channels = " << img.channels()
              << ", type(CV_8U=" << CV_8U << ") = " << img.type() << "\n";
    dd::save(path, img);
    std::cout << "保存しました: " << path << "\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
