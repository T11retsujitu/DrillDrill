// セクション 08 デモ: 二値化 + モルフォロジーで、線幅（CD）を測る最初の一歩。
//   使い方: ./sec08_demo [出力ディレクトリ]   （省略時 .）
#include "binarize.hpp"
#include "dd/image_io.hpp"
#include "dd/synth.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>

namespace {

// 1 行あたりの白画素数 / ライン本数 = 平均線幅 [px]。素朴な CD 計測。
double mean_linewidth(const cv::Mat& binary, int n_lines) {
  const double white = cv::countNonZero(binary);
  return white / binary.rows / n_lines;
}

}  // namespace

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    // pitch=64, duty=0.5 → 真の線幅 32 px、512 幅に 8 本。
    auto pat = dd::synth::make_line_space(512, 512, 64.0, 0.5, 1.5);
    const int n_lines = 8;
    cv::Mat noisy_f = dd::synth::add_gaussian(
        dd::synth::add_poisson(pat.clean, 2.0, /*seed=*/61), 6.0, /*seed=*/62);
    cv::Mat noisy;
    noisy_f.convertTo(noisy, CV_8U);

    // --- 1) ノイズ画像を直接 大津二値化 ---
    const int t_raw = otsu_threshold(noisy);
    cv::Mat bin_raw = threshold_binary(noisy, t_raw);

    // --- 2) 平滑化してから 大津二値化 ---
    cv::Mat smoothed;
    cv::GaussianBlur(noisy, smoothed, cv::Size(5, 5), 1.2, 1.2,
                     cv::BORDER_REPLICATE);
    const int t_sm = otsu_threshold(smoothed);
    cv::Mat bin_sm = threshold_binary(smoothed, t_sm);

    // --- 3) 直接二値化の残りノイズをオープニングで除去 ---
    cv::Mat bin_opened = open3(bin_raw);

    dd::save(dir + "/sec08_noisy.png", noisy);
    dd::save(dir + "/sec08_bin_raw.png", bin_raw);
    dd::save(dir + "/sec08_bin_smoothed.png", bin_sm);
    dd::save(dir + "/sec08_bin_opened.png", bin_opened);

    std::cout << "大津しきい値: 直接 = " << t_raw << ", 平滑化後 = " << t_sm
              << "\n";
    std::cout << "平均線幅（真値 " << pat.linewidth << " px）\n"
              << "  直接二値化        : " << mean_linewidth(bin_raw, n_lines)
              << " px\n"
              << "  平滑化→二値化     : " << mean_linewidth(bin_sm, n_lines)
              << " px\n"
              << "  直接→オープニング : "
              << mean_linewidth(bin_opened, n_lines) << " px\n";
    std::cout << "保存しました: " << dir << "/sec08_*.png\n"
              << "二値画像の「縁のギザギザ」も拡大して比較すること。\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
