// common/synth の検証（CI で実行される）。
#include "dd/metrics.hpp"
#include "dd/synth.hpp"
#include "dd/testing.hpp"

#include <cmath>
#include <opencv2/core.hpp>

int main() {
  using namespace dd;

  // pitch=40, duty=0.5 → linewidth=20, エッジは鋭い（blur=0）。
  auto pat = synth::make_line_space(/*w=*/200, /*h=*/64, /*pitch=*/40.0,
                                    /*duty=*/0.5, /*edge_blur=*/0.0,
                                    /*low=*/40.0, /*high=*/200.0);
  DD_CHECK_NEAR(pat.linewidth, 20.0, 1e-9);
  DD_CHECK_EQ(pat.clean.type(), CV_32F);

  // 列 5 はライン内(0..20)、列 25 はスペース内(20..40)。
  DD_CHECK_NEAR(pat.clean.at<float>(0, 5), 200.0, 1e-3);
  DD_CHECK_NEAR(pat.clean.at<float>(0, 25), 40.0, 1e-3);

  // ガウスノイズ: 残差の標準偏差が sigma に近いこと。
  const double sigma = 10.0;
  cv::Mat noisy = synth::add_gaussian(pat.clean, sigma, /*seed=*/123);
  cv::Mat resid = noisy - pat.clean;
  cv::Scalar m, s;
  cv::meanStdDev(resid, m, s);
  DD_CHECK_NEAR(m[0], 0.0, 0.5);
  DD_CHECK_NEAR(s[0], sigma, 1.0);

  // 同じ seed なら完全に再現できること。
  cv::Mat noisy2 = synth::add_gaussian(pat.clean, sigma, /*seed=*/123);
  DD_CHECK_EQ(cv::countNonZero(noisy != noisy2), 0);

  // ノイズを載せると PSNR は有限値になること。
  DD_CHECK(std::isfinite(psnr(pat.clean, noisy)));

  // Poisson: gain が大きいほど（高ドーズ）ノイズが小さいこと。
  cv::Mat lo = synth::add_poisson(pat.clean, /*gain=*/1.0, 7);
  cv::Mat hi = synth::add_poisson(pat.clean, /*gain=*/20.0, 7);
  DD_CHECK(mse(pat.clean, hi) < mse(pat.clean, lo));

  return dd::testing::summary("test_synth");
}
