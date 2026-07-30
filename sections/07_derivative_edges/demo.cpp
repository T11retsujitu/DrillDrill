// セクション 07 デモ: Sobel・勾配強度・ラプラシアンと、ノイズ増幅の観察。
//   使い方: ./sec07_demo [出力ディレクトリ]   （省略時 .）
#include "edges.hpp"
#include "dd/image_io.hpp"
#include "dd/synth.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    auto pat = dd::synth::make_line_space(512, 512, 64.0, 0.5, 1.5);
    cv::Mat noisy = dd::synth::add_gaussian(
        dd::synth::add_poisson(pat.clean, 2.0, /*seed=*/51), 6.0, /*seed=*/52);

    // --- 微分は「符号付き」: save_view は min..max を 0..255 に正規化するので
    //     ゼロが灰色、負が暗く、正が明るく見える ---
    cv::Mat gx = sobel_x(noisy);
    cv::Mat gy = sobel_y(noisy);
    cv::Mat mag = gradient_magnitude(gx, gy);
    cv::Mat lap = laplacian4(noisy);

    dd::save_view(dir + "/sec07_src.png", noisy);
    dd::save_view(dir + "/sec07_gx.png", gx);
    dd::save_view(dir + "/sec07_gy.png", gy);
    dd::save_view(dir + "/sec07_mag.png", mag);
    dd::save_view(dir + "/sec07_lap.png", lap);

    // --- 微分はノイズを増幅する: 平滑化してから微分すると何が変わるか ---
    cv::Mat smoothed;
    cv::GaussianBlur(noisy, smoothed, cv::Size(5, 5), 1.2, 1.2,
                     cv::BORDER_REPLICATE);
    cv::Mat mag_smoothed =
        gradient_magnitude(sobel_x(smoothed), sobel_y(smoothed));
    dd::save_view(dir + "/sec07_mag_after_smooth.png", mag_smoothed);

    // 平坦領域での勾配強度の平均 = ノイズ由来の偽応答。
    // ROI はエッジ（x=31.5 付近）の裾が届かない列だけに限定する。
    // エッジを含めてしまうと「本物のエッジ応答」が混ざり、数値の意味が崩れる。
    cv::Rect flat_roi(4, 0, 20, noisy.rows);  // 1 本目のライン内部 x=4..23
    const double fake_raw = cv::mean(mag(flat_roi))[0];
    const double fake_smooth = cv::mean(mag_smoothed(flat_roi))[0];
    std::cout << "平坦部の平均勾配強度（小さいほどノイズに強い）\n"
              << "  そのまま微分   : " << fake_raw << "\n"
              << "  平滑化→微分    : " << fake_smooth << "\n";

    // --- エッジ位置の推定: 中央行の |gx| ピークを正解と比較（CD 計測の入口） ---
    const int row = noisy.rows / 2;
    std::cout << "正解エッジ x = ";
    for (size_t i = 0; i < 4 && i < pat.edges.size(); ++i) {
      std::cout << pat.edges[i] << " ";
    }
    std::cout << "...\n推定ピーク x = ";
    cv::Mat absgx = cv::abs(gradient_magnitude(sobel_x(smoothed),
                                               sobel_y(smoothed)));
    int found = 0;
    for (int x = 1; x < absgx.cols - 1 && found < 4; ++x) {
      const float v = absgx.at<float>(row, x);
      if (v > 100.0f && v >= absgx.at<float>(row, x - 1) &&
          v > absgx.at<float>(row, x + 1)) {
        std::cout << x << " ";
        ++found;
      }
    }
    std::cout << "...\n保存しました: " << dir << "/sec07_*.png\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
