#include "median.hpp"
#include "dd/metrics.hpp"
#include "dd/synth.hpp"
#include "dd/testing.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
  // --- 孤立したインパルス（salt）を完全に除去できること ---
  cv::Mat flat(7, 7, CV_8U, cv::Scalar(50));
  flat.at<uchar>(3, 3) = 255;  // 中央に 1 画素だけ salt
  cv::Mat m = median_filter(flat, 3);
  DD_CHECK_EQ(m.type(), CV_8U);
  double mn, mx;
  cv::minMaxLoc(m, &mn, &mx);
  DD_CHECK_EQ((int)mn, 50);  // 全画素 50 に戻る
  DD_CHECK_EQ((int)mx, 50);

  // --- 理想的なステップエッジを「まったく」なまさないこと ---
  //     （box/ガウシアンとの決定的な違い。中間値が生まれない）
  cv::Mat step(8, 8, CV_8U, cv::Scalar(0));
  step.colRange(4, 8).setTo(200);
  cv::Mat ms = median_filter(step, 3);
  DD_CHECK_EQ(cv::countNonZero(ms != step), 0);

  // --- 入力を変更しないこと ---
  cv::Mat before = flat.clone();
  (void)median_filter(flat, 3);
  DD_CHECK_EQ(cv::countNonZero(flat != before), 0);

  // --- cv::medianBlur と一致すること ---
  auto pat = dd::synth::make_line_space(64, 64, 16.0, 0.5, 1.0);
  cv::Mat noisy_f = dd::synth::add_impulse(
      dd::synth::add_gaussian(pat.clean, 8.0, /*seed=*/21), 0.05, /*seed=*/22);
  cv::Mat noisy;
  noisy_f.convertTo(noisy, CV_8U);

  for (int k : {3, 5}) {
    cv::Mat mine = median_filter(noisy, k);
    cv::Mat theirs;
    cv::medianBlur(noisy, theirs, k);
    DD_CHECK_EQ(cv::countNonZero(mine != theirs), 0);
  }

  // --- インパルスノイズには、メディアンがガウシアンより強いこと ---
  cv::Mat clean8;
  pat.clean.convertTo(clean8, CV_8U);
  cv::Mat med = median_filter(noisy, 3);
  cv::Mat gau;
  cv::GaussianBlur(noisy, gau, cv::Size(3, 3), 0.8, 0.8, cv::BORDER_REPLICATE);
  DD_CHECK(dd::psnr(clean8, med) > dd::psnr(clean8, gau));

  return dd::testing::summary("section06.median");
}
