#include "brightness.hpp"

#include <stdexcept>

// TODO(you): 各画素 dst = saturate(gain*src + bias) を実装してください。
//   要件: 出力は CV_8U。src は変更しない（返すのは別 Mat）。0..255 に飽和させる。
//   ヒント: 出力用に別 Mat を確保し、二重ループ + cv::saturate_cast<uchar>。
cv::Mat apply_gain_bias(const cv::Mat& src, double gain, double bias) {
  (void)src;
  (void)gain;
  (void)bias;
  throw std::logic_error(
      "TODO(you): apply_gain_bias を実装してください（sections/01_image_memory/README.md 参照）");
}
