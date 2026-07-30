#include "first_image.hpp"
#include "dd/testing.hpp"

int main() {
  cv::Mat img = make_first_image();
  DD_CHECK(!img.empty());               // 空でないこと
  DD_CHECK_EQ(img.type(), CV_8U);       // 8bit 1ch であること
  DD_CHECK_EQ(img.cols, 256);
  DD_CHECK_EQ(img.rows, 256);
  return dd::testing::summary("section00.first_image");
}
