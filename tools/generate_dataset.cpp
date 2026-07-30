// generate_dataset — 合成 CD-SEM 風データセットを生成する。
//
//   使い方: ./generate_dataset [出力ディレクトリ]
//   （省略時は data/generated）
//
// 出力（すべて合成。機密画像は一切使わない）:
//   clean.png            … ノイズなしの理想パターン（表示用 8bit）
//   noisy_gaussian.png   … 読み出しノイズ相当
//   noisy_poisson.png    … 低ドーズのショットノイズ相当
//   noisy_pg.png         … Poisson + Gaussian（CD-SEM の代表的なノイズ）
//   noisy_impulse.png    … インパルスノイズ
//   noisy_scanline.png   … 走査線状の周期ノイズ
//   ground_truth.yml     … pitch / linewidth / エッジ座標（CD 計測の正解）

#include "dd/image_io.hpp"
#include "dd/synth.hpp"

#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>

int main(int argc, char** argv) {
  namespace fs = std::filesystem;
  const std::string out = (argc > 1) ? argv[1] : "data/generated";

  try {
    fs::create_directories(out);

    // 代表的なライン&スペースパターン。
    auto pat = dd::synth::make_line_space(/*w=*/512, /*h=*/512, /*pitch=*/64.0,
                                          /*duty=*/0.5, /*edge_blur=*/1.5);

    dd::save_view(out + "/clean.png", pat.clean);
    dd::save_view(out + "/noisy_gaussian.png",
                  dd::synth::add_gaussian(pat.clean, 12.0, 1));
    dd::save_view(out + "/noisy_poisson.png",
                  dd::synth::add_poisson(pat.clean, 2.0, 2));

    // CD-SEM の代表: 低ドーズ Poisson の上に読み出し Gaussian。
    cv::Mat pg = dd::synth::add_poisson(pat.clean, 2.0, 3);
    pg = dd::synth::add_gaussian(pg, 6.0, 4);
    dd::save_view(out + "/noisy_pg.png", pg);

    dd::save_view(out + "/noisy_impulse.png",
                  dd::synth::add_impulse(pat.clean, 0.03, 5));
    dd::save_view(out + "/noisy_scanline.png",
                  dd::synth::add_scanline(pat.clean, 15.0, 8.0, 6));

    // 正解（ground truth）を書き出す。cv::FileStorage の使い方も兼ねる。
    cv::FileStorage gt(out + "/ground_truth.yml", cv::FileStorage::WRITE);
    gt << "pitch" << pat.pitch;
    gt << "linewidth" << pat.linewidth;
    gt << "edges" << pat.edges;
    gt.release();

    std::cout << "生成しました: " << out << "\n"
              << "  linewidth(CD 正解) = " << pat.linewidth << " px\n"
              << "  edges = " << pat.edges.size() << " 本\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
