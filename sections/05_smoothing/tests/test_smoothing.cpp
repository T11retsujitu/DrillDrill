#include "smoothing.hpp"
#include "dd/metrics.hpp"
#include "dd/synth.hpp"
#include "dd/testing.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
  // --- box カーネル: 全要素等価・総和 1 ---
  cv::Mat bk = make_box_kernel(5);
  DD_CHECK_EQ(bk.type(), CV_32F);
  DD_CHECK_EQ(bk.rows, 5);
  DD_CHECK_EQ(bk.cols, 5);
  DD_CHECK_NEAR(bk.at<float>(0, 0), 1.0f / 25.0f, 1e-6);
  DD_CHECK_NEAR(cv::sum(bk)[0], 1.0, 1e-5);

  // --- ガウシアンカーネル: 総和 1・中心が最大・対称 ---
  cv::Mat gk = make_gaussian_kernel(7, 1.5);
  DD_CHECK_EQ(gk.type(), CV_32F);
  DD_CHECK_NEAR(cv::sum(gk)[0], 1.0, 1e-5);
  double mn, mx;
  cv::Point mx_at;
  cv::minMaxLoc(gk, &mn, &mx, nullptr, &mx_at);
  DD_CHECK_EQ(mx_at.x, 3);  // 中心 (3,3) が最大
  DD_CHECK_EQ(mx_at.y, 3);
  DD_CHECK_NEAR(gk.at<float>(0, 0), gk.at<float>(6, 6), 1e-6);  // 対称
  DD_CHECK_NEAR(gk.at<float>(0, 3), gk.at<float>(3, 0), 1e-6);
  DD_CHECK(gk.at<float>(3, 3) > gk.at<float>(0, 0));

  // --- 一様画像は平滑化しても一様のまま（総和 1 の帰結） ---
  cv::Mat flat(16, 16, CV_8U, cv::Scalar(100));
  cv::Mat fb = box_blur(flat, 3);
  cv::Mat fg = gaussian_blur(flat, 5, 1.0);
  double fb_mn, fb_mx, fg_mn, fg_mx;
  cv::minMaxLoc(fb, &fb_mn, &fb_mx);
  cv::minMaxLoc(fg, &fg_mn, &fg_mx);
  DD_CHECK_NEAR(fb_mn, 100.0, 1e-3);
  DD_CHECK_NEAR(fb_mx, 100.0, 1e-3);
  DD_CHECK_NEAR(fg_mn, 100.0, 1e-3);
  DD_CHECK_NEAR(fg_mx, 100.0, 1e-3);

  // --- cv::GaussianBlur(BORDER_REPLICATE) と一致すること ---
  auto pat = dd::synth::make_line_space(64, 64, 16.0, 0.5, 1.0);
  cv::Mat noisy = dd::synth::add_gaussian(pat.clean, 10.0, /*seed=*/9);
  cv::Mat mine = gaussian_blur(noisy, 7, 1.5);
  cv::Mat theirs;
  cv::GaussianBlur(noisy, theirs, cv::Size(7, 7), 1.5, 1.5,
                   cv::BORDER_REPLICATE);
  double max_diff = 0.0;
  cv::minMaxLoc(cv::abs(mine - theirs), nullptr, &max_diff);
  DD_CHECK_NEAR(max_diff, 0.0, 1e-2);

  // --- タスク指向の確認: 平滑化で PSNR が改善すること ---
  const double p_noisy = dd::psnr(pat.clean, noisy);
  const double p_gauss = dd::psnr(pat.clean, gaussian_blur(noisy, 5, 1.0));
  DD_CHECK(p_gauss > p_noisy);

  return dd::testing::summary("section05.smoothing");
}
