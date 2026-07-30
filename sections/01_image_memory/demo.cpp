// セクション 01 デモ: 合成画像にゲイン/バイアスを適用して保存する。
//   使い方: ./sec01_demo [出力ディレクトリ]   （省略時 .）
#include "brightness.hpp"
#include "dd/image_io.hpp"
#include "dd/synth.hpp"

#include <iostream>

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    auto pat = dd::synth::make_line_space(256, 256, 32.0, 0.5, 1.5);
    cv::Mat src;
    pat.clean.convertTo(src, CV_8U);

    cv::Mat brighter = apply_gain_bias(src, 1.0, 40.0);   // 明るく
    cv::Mat contrast = apply_gain_bias(src, 1.6, -30.0);  // コントラスト強調

    dd::save(dir + "/sec01_src.png", src);
    dd::save(dir + "/sec01_brighter.png", brighter);
    dd::save(dir + "/sec01_contrast.png", contrast);
    std::cout << "保存しました: " << dir << "/sec01_*.png\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
