// セクション 05 デモ: box / ガウシアン平滑化と、PSNR による効果の定量化。
//   使い方: ./sec05_demo [出力ディレクトリ]   （省略時 .）
#include "smoothing.hpp"
#include "dd/image_io.hpp"
#include "dd/metrics.hpp"
#include "dd/synth.hpp"

#include <iostream>

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    // CD-SEM を意識した Poisson + Gaussian ノイズ。
    auto pat = dd::synth::make_line_space(512, 512, 64.0, 0.5, 1.5);
    cv::Mat noisy = dd::synth::add_poisson(pat.clean, 2.0, /*seed=*/11);
    noisy = dd::synth::add_gaussian(noisy, 6.0, /*seed=*/12);

    dd::save_view(dir + "/sec05_clean.png", pat.clean);
    dd::save_view(dir + "/sec05_noisy.png", noisy);

    std::cout << "PSNR vs clean（大きいほど clean に近い）\n";
    std::cout << "  無処理           : " << dd::psnr(pat.clean, noisy) << " dB\n";

    for (int k : {3, 5, 9}) {
      cv::Mat b = box_blur(noisy, k);
      cv::Mat g = gaussian_blur(noisy, k, /*sigma=*/k / 6.0 + 0.5);
      dd::save_view(dir + "/sec05_box" + std::to_string(k) + ".png", b);
      dd::save_view(dir + "/sec05_gauss" + std::to_string(k) + ".png", g);
      std::cout << "  box " << k << "x" << k << "          : "
                << dd::psnr(pat.clean, b) << " dB\n";
      std::cout << "  gaussian " << k << "x" << k << "     : "
                << dd::psnr(pat.clean, g) << " dB\n";
    }

    std::cout << "保存しました: " << dir << "/sec05_*.png\n"
              << "PSNR が最大の設定でも、エッジ（ラインの縁）を拡大して確認すること。\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
