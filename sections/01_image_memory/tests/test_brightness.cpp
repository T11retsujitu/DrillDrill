#include "brightness.hpp"
#include "dd/testing.hpp"

#include <opencv2/core.hpp>

int main() {
  cv::Mat src = (cv::Mat_<uchar>(1, 4) << 0, 100, 200, 255);
  cv::Mat before = src.clone();

  // gain=2, bias=50: 0->50, 100->250, 200->450(飽和で255), 255->560(255)
  cv::Mat dst = apply_gain_bias(src, 2.0, 50.0);

  DD_CHECK_EQ(dst.type(), CV_8U);
  DD_CHECK_EQ((int)dst.at<uchar>(0, 0), 50);
  DD_CHECK_EQ((int)dst.at<uchar>(0, 1), 250);
  DD_CHECK_EQ((int)dst.at<uchar>(0, 2), 255);   // 飽和
  DD_CHECK_EQ((int)dst.at<uchar>(0, 3), 255);   // 飽和

  // src を書き換えていないこと（const 正しさ）。
  DD_CHECK_EQ(cv::countNonZero(src != before), 0);

  // 出力を書き換えても src に影響しないこと（別メモリであること）。
  dst.at<uchar>(0, 0) = 123;
  DD_CHECK_EQ((int)src.at<uchar>(0, 0), 0);

  return dd::testing::summary("section01.apply_gain_bias");
}
