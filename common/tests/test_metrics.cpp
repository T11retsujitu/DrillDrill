// common/metrics の検証（CI で実行される）。
#include "dd/metrics.hpp"
#include "dd/testing.hpp"

#include <cmath>
#include <opencv2/core.hpp>

int main() {
  using namespace dd;

  cv::Mat a = (cv::Mat_<uchar>(2, 3) << 10, 20, 30, 40, 50, 60);
  cv::Mat b = a.clone();

  // 同一画像: mse=0, psnr=inf, ssim≈1
  DD_CHECK_EQ(mse(a, b), 0.0);
  DD_CHECK(std::isinf(psnr(a, b)));
  DD_CHECK_NEAR(ssim(a, b), 1.0, 1e-6);

  // 既知の差: 全画素 +2 → mse = 4 → psnr = 10*log10(255^2/4)
  cv::Mat c = a + 2;
  DD_CHECK_NEAR(mse(a, c), 4.0, 1e-9);
  const double expected = 10.0 * std::log10((255.0 * 255.0) / 4.0);
  DD_CHECK_NEAR(psnr(a, c), expected, 1e-6);

  // ラインプロファイル
  auto prof = line_profile(a, 1);
  DD_CHECK_EQ((int)prof.size(), 3);
  DD_CHECK_NEAR(prof[0], 40.0, 1e-9);
  DD_CHECK_NEAR(prof[2], 60.0, 1e-9);

  return dd::testing::summary("test_metrics");
}
