#include "reference_lab.hpp"
#include "dd/testing.hpp"

#include <opencv2/core.hpp>

int main() {
  cv::Mat a = (cv::Mat_<uchar>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

  // 代入は浅いコピー: データを共有する。
  cv::Mat shallow = a;
  DD_CHECK(shares_data(a, shallow));

  // ROI もデータを共有する。
  cv::Mat roi = a(cv::Rect(0, 0, 2, 2));
  DD_CHECK(shares_data(a, roi));

  // independent_copy は共有しない。
  cv::Mat deep = independent_copy(a);
  DD_CHECK(!shares_data(a, deep));

  // deep を書き換えても a は変わらない。
  deep.at<uchar>(0, 0) = 100;
  DD_CHECK_EQ((int)a.at<uchar>(0, 0), 1);

  // 逆に浅いコピーを書き換えると a も変わる（共有の確認）。
  shallow.at<uchar>(0, 0) = 55;
  DD_CHECK_EQ((int)a.at<uchar>(0, 0), 55);

  return dd::testing::summary("section02.reference_lab");
}
