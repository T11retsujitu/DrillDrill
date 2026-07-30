// セクション 06 デモ: メディアン vs ガウシアン vs エッジ保存平滑化（OpenCV）。
//   使い方: ./sec06_demo [出力ディレクトリ]   （省略時 .）
#include "median.hpp"
#include "dd/image_io.hpp"
#include "dd/metrics.hpp"
#include "dd/synth.hpp"

#include <fstream>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    auto pat = dd::synth::make_line_space(512, 512, 64.0, 0.5, 1.5);
    cv::Mat clean8;
    pat.clean.convertTo(clean8, CV_8U);

    // --- ケース 1: インパルスノイズ（メディアンの独壇場） ---
    cv::Mat imp_f = dd::synth::add_impulse(pat.clean, 0.05, /*seed=*/31);
    cv::Mat imp;
    imp_f.convertTo(imp, CV_8U);

    cv::Mat med3 = median_filter(imp, 3);
    cv::Mat gau3;
    cv::GaussianBlur(imp, gau3, cv::Size(3, 3), 0.8, 0.8, cv::BORDER_REPLICATE);

    dd::save(dir + "/sec06_impulse.png", imp);
    dd::save(dir + "/sec06_impulse_median3.png", med3);
    dd::save(dir + "/sec06_impulse_gauss3.png", gau3);

    std::cout << "=== インパルスノイズ (5%) ===\n";
    std::cout << "  無処理     : " << dd::psnr(clean8, imp) << " dB\n";
    std::cout << "  median 3x3 : " << dd::psnr(clean8, med3) << " dB\n";
    std::cout << "  gauss  3x3 : " << dd::psnr(clean8, gau3) << " dB\n";

    // --- ケース 2: Poisson+Gaussian（CD-SEM 的なノイズ）でエッジ保存を比較 ---
    cv::Mat pg_f = dd::synth::add_gaussian(
        dd::synth::add_poisson(pat.clean, 2.0, /*seed=*/32), 6.0, /*seed=*/33);
    cv::Mat pg;
    pg_f.convertTo(pg, CV_8U);

    cv::Mat med5 = median_filter(pg, 5);
    cv::Mat gau5;
    cv::GaussianBlur(pg, gau5, cv::Size(5, 5), 1.1, 1.1, cv::BORDER_REPLICATE);
    cv::Mat bil;
    cv::bilateralFilter(pg, bil, /*d=*/9, /*sigmaColor=*/50.0, /*sigmaSpace=*/3.0);
    cv::Mat nlm;
    cv::fastNlMeansDenoising(pg, nlm, /*h=*/12.0f);

    dd::save(dir + "/sec06_pg.png", pg);
    dd::save(dir + "/sec06_pg_median5.png", med5);
    dd::save(dir + "/sec06_pg_gauss5.png", gau5);
    dd::save(dir + "/sec06_pg_bilateral.png", bil);
    dd::save(dir + "/sec06_pg_nlm.png", nlm);

    std::cout << "=== Poisson+Gaussian ノイズ ===\n";
    std::cout << "  無処理        : " << dd::psnr(clean8, pg) << " dB\n";
    std::cout << "  median 5x5    : " << dd::psnr(clean8, med5) << " dB\n";
    std::cout << "  gauss  5x5    : " << dd::psnr(clean8, gau5) << " dB\n";
    std::cout << "  bilateral     : " << dd::psnr(clean8, bil) << " dB\n";
    std::cout << "  NLM           : " << dd::psnr(clean8, nlm) << " dB\n";

    // エッジ保存の定性確認用に、中央行のラインプロファイルを CSV 出力。
    const int row = pg.rows / 2;
    std::ofstream csv(dir + "/sec06_profiles.csv");
    csv << "x,clean,noisy,median5,gauss5,bilateral,nlm\n";
    auto pc = dd::line_profile(clean8, row);
    auto pn = dd::line_profile(pg, row);
    auto pm = dd::line_profile(med5, row);
    auto pgs = dd::line_profile(gau5, row);
    auto pb = dd::line_profile(bil, row);
    auto pl = dd::line_profile(nlm, row);
    for (size_t x = 0; x < pc.size(); ++x) {
      csv << x << "," << pc[x] << "," << pn[x] << "," << pm[x] << ","
          << pgs[x] << "," << pb[x] << "," << pl[x] << "\n";
    }

    std::cout << "保存しました: " << dir
              << "/sec06_*.png, sec06_profiles.csv（プロファイルはプロットして比較）\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
