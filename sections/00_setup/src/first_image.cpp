#include "first_image.hpp"

#include "dd/synth.hpp"
#include <opencv2/imgproc.hpp>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。
cv::Mat make_first_image() {
  auto pat = dd::synth::make_line_space(/*w=*/256, /*h=*/256, /*pitch=*/32.0,
                                        /*duty=*/0.5, /*edge_blur=*/1.5);
  cv::Mat out;
  cv::normalize(pat.clean, out, 0, 255, cv::NORM_MINMAX);
  out.convertTo(out, CV_8U);
  return out;
}
