// セクション 03 デモ: ヒストグラム・コントラスト伸長・16bit→8bit スケーリング。
//   使い方: ./sec03_demo [出力ディレクトリ]   （省略時 .）
#include "histogram.hpp"
#include "dd/image_io.hpp"
#include "dd/synth.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <opencv2/imgproc.hpp>

namespace {

// ヒストグラムを棒グラフ画像として描く（観察用の補助）。
cv::Mat render_histogram(const std::vector<int>& hist, int height = 200) {
  const int max_count = *std::max_element(hist.begin(), hist.end());
  cv::Mat img(height, 256, CV_8U, cv::Scalar(255));
  if (max_count == 0) return img;
  for (int v = 0; v < 256; ++v) {
    const int h = static_cast<int>(
        static_cast<double>(hist[v]) / max_count * (height - 1));
    if (h > 0) {
      cv::line(img, {v, height - 1}, {v, height - 1 - h}, cv::Scalar(0));
    }
  }
  return img;
}

}  // namespace

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    // 低コントラスト（80..170 付近）のノイズ入りパターンを作る。
    auto pat = dd::synth::make_line_space(/*w=*/512, /*h=*/512, /*pitch=*/64.0,
                                          /*duty=*/0.5, /*edge_blur=*/1.5,
                                          /*low=*/80.0, /*high=*/170.0);
    cv::Mat noisy_f = dd::synth::add_gaussian(pat.clean, 8.0, /*seed=*/42);
    cv::Mat src8;
    noisy_f.convertTo(src8, CV_8U);  // 8bit 化（飽和つき丸め）

    // --- 1) ヒストグラムを CSV と棒グラフ画像で出力 ---
    auto hist = compute_histogram(src8);
    std::ofstream csv(dir + "/sec03_hist.csv");
    csv << "value,count\n";
    for (int v = 0; v < 256; ++v) csv << v << "," << hist[v] << "\n";
    dd::save(dir + "/sec03_hist.png", render_histogram(hist));

    // --- 2) コントラスト伸長の前後を保存 ---
    dd::save(dir + "/sec03_src.png", src8);
    cv::Mat stretched = stretch_contrast(src8);
    dd::save(dir + "/sec03_stretched.png", stretched);
    dd::save(dir + "/sec03_hist_stretched.png",
             render_histogram(compute_histogram(stretched)));

    // --- 3) 16bit 画像のスケーリング体験 ---
    // 0..255 の float 画像を 0..65535 の 16bit に引き伸ばして保存し、
    // 「全レンジ」と「実データ範囲」の 2 通りで 8bit 表示してみる。
    cv::Mat img16;
    pat.clean.convertTo(img16, CV_16U, 65535.0 / 255.0);
    dd::save(dir + "/sec03_16bit.png", img16);  // 16bit PNG として保存される

    cv::Mat view_full = rescale_to_8u(img16, 0, 65535);
    double mn, mx;
    cv::minMaxLoc(img16, &mn, &mx);
    cv::Mat view_fit = rescale_to_8u(img16, mn, mx);
    dd::save(dir + "/sec03_16bit_view_fullrange.png", view_full);
    dd::save(dir + "/sec03_16bit_view_fitrange.png", view_fit);

    std::cout << "保存しました: " << dir << "/sec03_*.png, sec03_hist.csv\n"
              << "16bit 実データ範囲: [" << mn << ", " << mx << "]\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
