#include "histogram.hpp"
#include "dd/testing.hpp"

#include <numeric>
#include <opencv2/core.hpp>

int main() {
  // --- compute_histogram ---
  cv::Mat a = (cv::Mat_<uchar>(2, 3) << 0, 0, 5, 5, 5, 255);
  auto hist = compute_histogram(a);
  DD_CHECK_EQ((int)hist.size(), 256);
  DD_CHECK_EQ(hist[0], 2);
  DD_CHECK_EQ(hist[5], 3);
  DD_CHECK_EQ(hist[255], 1);
  DD_CHECK_EQ(hist[7], 0);
  // 総和は画素数に一致する。
  DD_CHECK_EQ(std::accumulate(hist.begin(), hist.end(), 0), 6);

  // --- rescale_to_8u: 16bit 全レンジ → 8bit ---
  cv::Mat m16 = (cv::Mat_<ushort>(1, 3) << 0, 32768, 65535);
  cv::Mat v = rescale_to_8u(m16, 0, 65535);
  DD_CHECK_EQ(v.type(), CV_8U);
  DD_CHECK_EQ((int)v.at<uchar>(0, 0), 0);
  DD_CHECK_EQ((int)v.at<uchar>(0, 1), 128);  // 32768*255/65535 = 127.502 → 128
  DD_CHECK_EQ((int)v.at<uchar>(0, 2), 255);

  // 区間外は飽和する（in_min より小さい値 → 0、in_max より大きい値 → 255）。
  cv::Mat m2 = (cv::Mat_<ushort>(1, 3) << 50, 400, 300);
  cv::Mat v2 = rescale_to_8u(m2, 100, 300);
  DD_CHECK_EQ((int)v2.at<uchar>(0, 0), 0);    // 50 < in_min
  DD_CHECK_EQ((int)v2.at<uchar>(0, 1), 255);  // 400 > in_max（ラップさせない）
  DD_CHECK_EQ((int)v2.at<uchar>(0, 2), 255);  // ちょうど in_max

  // 「任意ビット深度」の仕様: CV_8U / CV_32F 入力でも正しく動くこと。
  cv::Mat m8 = (cv::Mat_<uchar>(1, 3) << 0, 128, 255);
  cv::Mat v8 = rescale_to_8u(m8, 0, 255);
  DD_CHECK_EQ((int)v8.at<uchar>(0, 0), 0);
  DD_CHECK_EQ((int)v8.at<uchar>(0, 1), 128);
  DD_CHECK_EQ((int)v8.at<uchar>(0, 2), 255);
  cv::Mat mf = (cv::Mat_<float>(1, 3) << 0.0f, 0.5f, 1.0f);
  cv::Mat vf = rescale_to_8u(mf, 0.0, 1.0);
  DD_CHECK_EQ((int)vf.at<uchar>(0, 0), 0);
  DD_CHECK_EQ((int)vf.at<uchar>(0, 1), 128);  // 0.5*255 = 127.5 → 128
  DD_CHECK_EQ((int)vf.at<uchar>(0, 2), 255);

  // --- stretch_contrast ---
  cv::Mat s = (cv::Mat_<uchar>(1, 3) << 40, 90, 240);
  cv::Mat before = s.clone();
  cv::Mat st = stretch_contrast(s);
  DD_CHECK_EQ(st.type(), CV_8U);
  DD_CHECK_EQ((int)st.at<uchar>(0, 0), 0);    // min → 0
  DD_CHECK_EQ((int)st.at<uchar>(0, 1), 64);   // (90-40)*255/200 = 63.75 → 64
  DD_CHECK_EQ((int)st.at<uchar>(0, 2), 255);  // max → 255
  // 入力は変更しない。
  DD_CHECK_EQ(cv::countNonZero(s != before), 0);

  // 一様な画像はそのまま（クラッシュしない・値も不変）。
  cv::Mat flat(4, 4, CV_8U, cv::Scalar(77));
  cv::Mat stf = stretch_contrast(flat);
  DD_CHECK_EQ((int)stf.at<uchar>(0, 0), 77);
  // かつ独立したコピーであること。
  stf.at<uchar>(0, 0) = 1;
  DD_CHECK_EQ((int)flat.at<uchar>(0, 0), 77);

  return dd::testing::summary("section03.histogram_bitdepth");
}
