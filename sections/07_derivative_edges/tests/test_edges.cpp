#include "edges.hpp"
#include "dd/synth.hpp"
#include "dd/testing.hpp"

#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
  // --- 平坦画像では全微分がゼロ ---
  cv::Mat flat(8, 8, CV_8U, cv::Scalar(120));
  double mn, mx;
  cv::minMaxLoc(cv::abs(sobel_x(flat)), &mn, &mx);
  DD_CHECK_NEAR(mx, 0.0, 1e-4);
  cv::minMaxLoc(cv::abs(sobel_y(flat)), &mn, &mx);
  DD_CHECK_NEAR(mx, 0.0, 1e-4);
  cv::minMaxLoc(cv::abs(laplacian4(flat)), &mn, &mx);
  DD_CHECK_NEAR(mx, 0.0, 1e-4);

  // --- 垂直ステップエッジ（左 0 | 右 100、境界は x=4） ---
  cv::Mat step(8, 8, CV_8U, cv::Scalar(0));
  step.colRange(4, 8).setTo(100);

  cv::Mat gx = sobel_x(step);
  DD_CHECK_EQ(gx.type(), CV_32F);
  // エッジをまたぐ列で正（暗→明）、それ以外で 0。
  //   x=3: 近傍は [0,0,100] → (-1*0+0*0+1*100)*4行分の重み = 100*(1+2+1) = 400
  DD_CHECK_NEAR(gx.at<float>(4, 3), 400.0, 1e-3);
  DD_CHECK_NEAR(gx.at<float>(4, 4), 400.0, 1e-3);
  DD_CHECK_NEAR(gx.at<float>(4, 1), 0.0, 1e-3);   // エッジから離れた列
  DD_CHECK_NEAR(gx.at<float>(4, 6), 0.0, 1e-3);
  // 垂直エッジに sobel_y は反応しない。
  cv::minMaxLoc(cv::abs(sobel_y(step)), &mn, &mx);
  DD_CHECK_NEAR(mx, 0.0, 1e-4);

  // 明→暗にすると符号が反転する。
  cv::Mat step_rev(8, 8, CV_8U, cv::Scalar(100));
  step_rev.colRange(4, 8).setTo(0);
  DD_CHECK_NEAR(sobel_x(step_rev).at<float>(4, 3), -400.0, 1e-3);

  // --- gradient_magnitude: 3-4-5 の三角形 ---
  cv::Mat a(2, 2, CV_32F, cv::Scalar(3.0f));
  cv::Mat b(2, 2, CV_32F, cv::Scalar(4.0f));
  cv::Mat mag = gradient_magnitude(a, b);
  DD_CHECK_EQ(mag.type(), CV_32F);
  DD_CHECK_NEAR(mag.at<float>(0, 0), 5.0, 1e-5);
  DD_CHECK_NEAR(mag.at<float>(1, 1), 5.0, 1e-5);

  // --- OpenCV との一致（本命の検証） ---
  auto pat = dd::synth::make_line_space(64, 64, 16.0, 0.5, 1.0);
  cv::Mat noisy = dd::synth::add_gaussian(pat.clean, 8.0, /*seed=*/17);
  double max_diff = 0.0;

  cv::Mat cv_gx;
  cv::Sobel(noisy, cv_gx, CV_32F, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE);
  cv::minMaxLoc(cv::abs(sobel_x(noisy) - cv_gx), nullptr, &max_diff);
  DD_CHECK_NEAR(max_diff, 0.0, 1e-3);

  cv::Mat cv_gy;
  cv::Sobel(noisy, cv_gy, CV_32F, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE);
  cv::minMaxLoc(cv::abs(sobel_y(noisy) - cv_gy), nullptr, &max_diff);
  DD_CHECK_NEAR(max_diff, 0.0, 1e-3);

  cv::Mat cv_lap;
  cv::Laplacian(noisy, cv_lap, CV_32F, 1, 1, 0, cv::BORDER_REPLICATE);
  cv::minMaxLoc(cv::abs(laplacian4(noisy) - cv_lap), nullptr, &max_diff);
  DD_CHECK_NEAR(max_diff, 0.0, 1e-3);

  // --- エッジ位置の同定: 勾配強度のピークが正解エッジ位置に一致する ---
  //     （ぼけのない鋭いエッジなら、ピーク列は正解エッジの左右いずれかの列）
  cv::Mat clean_sharp =
      dd::synth::make_line_space(64, 64, 16.0, 0.5, /*edge_blur=*/0.0).clean;
  cv::Mat mag2 = gradient_magnitude(sobel_x(clean_sharp), sobel_y(clean_sharp));
  // 1 本目の正解エッジは x=8（synth の仕様: 立ち上がり位置）。
  // 行 32 で x=5..11 の最大値の位置を調べる。
  int peak_x = -1;
  float peak_v = -1.0f;
  for (int x = 5; x <= 11; ++x) {
    const float v = mag2.at<float>(32, x);
    if (v > peak_v) {
      peak_v = v;
      peak_x = x;
    }
  }
  DD_CHECK(peak_x == 7 || peak_x == 8);  // 画素中心の定義により左右どちらか

  return dd::testing::summary("section07.edges");
}
