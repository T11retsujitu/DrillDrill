#include "convolve.hpp"
#include "dd/synth.hpp"
#include "dd/testing.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
  // --- 恒等カーネル: 出力 == 入力 ---
  cv::Mat src = (cv::Mat_<uchar>(3, 3) << 10, 20, 30, 40, 50, 60, 70, 80, 90);
  cv::Mat identity = cv::Mat::zeros(3, 3, CV_32F);
  identity.at<float>(1, 1) = 1.0f;
  cv::Mat out = convolve(src, identity);
  DD_CHECK_EQ(out.type(), CV_32F);
  DD_CHECK_EQ(out.rows, 3);
  DD_CHECK_EQ(out.cols, 3);
  DD_CHECK_NEAR(out.at<float>(1, 1), 50.0, 1e-4);
  DD_CHECK_NEAR(out.at<float>(0, 0), 10.0, 1e-4);

  // --- シフトカーネル: kernel(0,1)=1 は「1 行上の画素」を持ってくる ---
  //     出力(y,x) = src(y-1, x)。y=0 では replicate により src(0,x) のまま。
  cv::Mat shift = cv::Mat::zeros(3, 3, CV_32F);
  shift.at<float>(0, 1) = 1.0f;
  cv::Mat sh = convolve(src, shift);
  DD_CHECK_NEAR(sh.at<float>(1, 0), 10.0, 1e-4);  // src(0,0)
  DD_CHECK_NEAR(sh.at<float>(2, 2), 60.0, 1e-4);  // src(1,2)
  DD_CHECK_NEAR(sh.at<float>(0, 1), 20.0, 1e-4);  // 境界: src(0,1) を複製

  // --- 1x3 カーネルの境界処理を手計算と照合 ---
  //     src=[10,20,30], kernel=[0.25,0.5,0.25]
  //     x=0: 10*0.25+10*0.5+20*0.25 = 12.5 （左端は 10 を複製）
  //     x=1: 10*0.25+20*0.5+30*0.25 = 20
  //     x=2: 20*0.25+30*0.5+30*0.25 = 27.5 （右端は 30 を複製）
  cv::Mat row = (cv::Mat_<uchar>(1, 3) << 10, 20, 30);
  cv::Mat k13 = (cv::Mat_<float>(1, 3) << 0.25f, 0.5f, 0.25f);
  cv::Mat r = convolve(row, k13);
  DD_CHECK_NEAR(r.at<float>(0, 0), 12.5, 1e-4);
  DD_CHECK_NEAR(r.at<float>(0, 1), 20.0, 1e-4);
  DD_CHECK_NEAR(r.at<float>(0, 2), 27.5, 1e-4);

  // --- 入力を変更しないこと ---
  cv::Mat before = src.clone();
  (void)convolve(src, identity);
  DD_CHECK_EQ(cv::countNonZero(src != before), 0);

  // --- cv::filter2D(BORDER_REPLICATE) と一致すること（本命の検証） ---
  auto pat = dd::synth::make_line_space(64, 64, 16.0, 0.5, 1.0);
  cv::Mat noisy = dd::synth::add_gaussian(pat.clean, 10.0, /*seed=*/7);
  cv::Mat box(5, 5, CV_32F, cv::Scalar(1.0f / 25.0f));
  cv::Mat mine = convolve(noisy, box);
  cv::Mat theirs;
  cv::filter2D(noisy, theirs, CV_32F, box, cv::Point(-1, -1), 0,
               cv::BORDER_REPLICATE);
  double max_diff = 0.0;
  cv::minMaxLoc(cv::abs(mine - theirs), nullptr, &max_diff);
  DD_CHECK_NEAR(max_diff, 0.0, 1e-3);

  // 非対称カーネルでも一致すること（相関として実装している証拠）。
  cv::Mat asym = (cv::Mat_<float>(1, 3) << -1.0f, 0.0f, 2.0f);
  cv::Mat mine2 = convolve(noisy, asym);
  cv::Mat theirs2;
  cv::filter2D(noisy, theirs2, CV_32F, asym, cv::Point(-1, -1), 0,
               cv::BORDER_REPLICATE);
  cv::minMaxLoc(cv::abs(mine2 - theirs2), nullptr, &max_diff);
  DD_CHECK_NEAR(max_diff, 0.0, 1e-3);

  return dd::testing::summary("section04.convolve");
}
