#include "binarize.hpp"
#include "dd/synth.hpp"
#include "dd/testing.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
  // --- threshold_binary: 「>」の定義と 0/255 のみの出力 ---
  cv::Mat a = (cv::Mat_<uchar>(1, 4) << 0, 100, 101, 255);
  cv::Mat b = threshold_binary(a, 100);
  DD_CHECK_EQ(b.type(), CV_8U);
  DD_CHECK_EQ((int)b.at<uchar>(0, 0), 0);
  DD_CHECK_EQ((int)b.at<uchar>(0, 1), 0);    // ちょうど thresh は 0（> の定義）
  DD_CHECK_EQ((int)b.at<uchar>(0, 2), 255);
  DD_CHECK_EQ((int)b.at<uchar>(0, 3), 255);
  // 入力は変更しない。
  DD_CHECK_EQ((int)a.at<uchar>(0, 1), 100);

  // --- otsu_threshold: OpenCV の THRESH_OTSU と一致 ---
  auto pat = dd::synth::make_line_space(128, 128, 32.0, 0.5, 1.5);
  cv::Mat noisy_f = dd::synth::add_gaussian(pat.clean, 12.0, /*seed=*/41);
  cv::Mat noisy;
  noisy_f.convertTo(noisy, CV_8U);

  const int mine_t = otsu_threshold(noisy);
  cv::Mat dummy;
  const double cv_t =
      cv::threshold(noisy, dummy, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  DD_CHECK_EQ(mine_t, (int)cv_t);

  // しきい値は 2 つの山（40 と 200）の間に来るはず。
  DD_CHECK(mine_t > 60);
  DD_CHECK(mine_t < 180);

  // タイブレーク: 値が {50, 200} だけの画像では t=50..199 のクラス間分散が
  // すべて同値になる。仕様（と cv::THRESH_OTSU）は最小の t=50 を返す。
  cv::Mat two(64, 64, CV_8U, cv::Scalar(50));
  two.rowRange(0, 32).setTo(200);
  DD_CHECK_EQ(otsu_threshold(two), 50);

  // --- erode3 / dilate3: 孤立画素での基本動作 ---
  cv::Mat single(7, 7, CV_8U, cv::Scalar(0));
  single.at<uchar>(3, 3) = 255;
  cv::Mat er = erode3(single);
  double mn, mx;
  cv::minMaxLoc(er, &mn, &mx);
  DD_CHECK_EQ((int)mx, 0);  // 孤立白画素は収縮で消える
  cv::Mat single_before = single.clone();
  cv::Mat di = dilate3(single);
  // 入力を変更しないこと。
  DD_CHECK_EQ(cv::countNonZero(single != single_before), 0);
  DD_CHECK_EQ(cv::countNonZero(di), 9);  // 3x3 に太る
  DD_CHECK_EQ((int)di.at<uchar>(2, 2), 255);
  DD_CHECK_EQ((int)di.at<uchar>(4, 4), 255);
  DD_CHECK_EQ((int)di.at<uchar>(1, 1), 0);

  // --- cv::erode / cv::dilate（3x3 矩形、デフォルト境界）と完全一致 ---
  cv::Mat noisy_bin = threshold_binary(noisy, 120);
  const cv::Mat k3 = cv::getStructuringElement(cv::MORPH_RECT, {3, 3});
  cv::Mat cv_er, cv_di;
  cv::erode(noisy_bin, cv_er, k3);
  cv::dilate(noisy_bin, cv_di, k3);
  DD_CHECK_EQ(cv::countNonZero(erode3(noisy_bin) != cv_er), 0);
  DD_CHECK_EQ(cv::countNonZero(dilate3(noisy_bin) != cv_di), 0);

  // --- open3: 幅の広い白バンドを保ちつつ、孤立ノイズを除去する ---
  cv::Mat bands(32, 32, CV_8U, cv::Scalar(0));
  bands.colRange(8, 16).setTo(255);   // 幅 8 の白バンド
  cv::Mat dirty = bands.clone();
  dirty.at<uchar>(4, 25) = 255;       // バンドから離れた孤立 salt
  dirty.at<uchar>(20, 2) = 255;
  cv::Mat opened = open3(dirty);
  DD_CHECK_EQ(cv::countNonZero(opened != bands), 0);  // 完全に復元される

  // --- close3: バンド内の小さな黒穴を埋める ---
  cv::Mat holey = bands.clone();
  holey.at<uchar>(10, 12) = 0;        // バンド内部の 1 画素穴
  cv::Mat closed = close3(holey);
  DD_CHECK_EQ(cv::countNonZero(closed != bands), 0);

  return dd::testing::summary("section08.binarize");
}
