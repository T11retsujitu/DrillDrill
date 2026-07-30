// セクション 04 デモ: 各種カーネルの適用と、cv::filter2D との速度比較。
//   使い方: ./sec04_demo [出力ディレクトリ]   （省略時 .）
#include "convolve.hpp"
#include "dd/image_io.hpp"
#include "dd/synth.hpp"

#include <chrono>
#include <iostream>
#include <opencv2/imgproc.hpp>

namespace {

double ms_since(std::chrono::steady_clock::time_point t0) {
  return std::chrono::duration<double, std::milli>(
             std::chrono::steady_clock::now() - t0)
      .count();
}

}  // namespace

int main(int argc, char** argv) {
  const std::string dir = (argc > 1) ? argv[1] : ".";
  try {
    auto pat = dd::synth::make_line_space(512, 512, 64.0, 0.5, 1.5);
    cv::Mat noisy = dd::synth::add_gaussian(pat.clean, 12.0, /*seed=*/42);
    dd::save_view(dir + "/sec04_src.png", noisy);

    // --- 恒等カーネル（何もしない）: 実装の健全性確認に便利 ---
    cv::Mat identity = cv::Mat::zeros(3, 3, CV_32F);
    identity.at<float>(1, 1) = 1.0f;
    dd::save_view(dir + "/sec04_identity.png", convolve(noisy, identity));

    // --- 5x5 box（平均化）: ノイズは減るがエッジもなまる ---
    cv::Mat box(5, 5, CV_32F, cv::Scalar(1.0f / 25.0f));
    dd::save_view(dir + "/sec04_box5.png", convolve(noisy, box));

    // --- 横方向差分 [-1, 0, +1]: 縦エッジが浮き出る（符号付き出力に注意） ---
    cv::Mat dx = (cv::Mat_<float>(1, 3) << -1.0f, 0.0f, 1.0f);
    dd::save_view(dir + "/sec04_dx.png", convolve(noisy, dx));

    // --- 速度比較: 自作 vs cv::filter2D（同じ境界処理） ---
    auto t0 = std::chrono::steady_clock::now();
    cv::Mat mine = convolve(noisy, box);
    const double t_mine = ms_since(t0);

    cv::Mat theirs;
    t0 = std::chrono::steady_clock::now();
    cv::filter2D(noisy, theirs, CV_32F, box, cv::Point(-1, -1), 0,
                 cv::BORDER_REPLICATE);
    const double t_cv = ms_since(t0);

    double max_diff = 0.0;
    cv::minMaxLoc(cv::abs(mine - theirs), nullptr, &max_diff);

    std::cout << "5x5 box @512x512:  自作 " << t_mine << " ms,  filter2D "
              << t_cv << " ms\n"
              << "最大絶対差 = " << max_diff << " （小さいほど一致）\n"
              << "保存しました: " << dir << "/sec04_*.png\n";
  } catch (const std::exception& e) {
    std::cerr << "エラー: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
