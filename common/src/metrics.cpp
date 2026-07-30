#include "dd/metrics.hpp"

#include <opencv2/imgproc.hpp>
#include <limits>
#include <stdexcept>

namespace dd {

namespace {
void check_same(const cv::Mat& a, const cv::Mat& b, const char* who) {
  if (a.empty() || b.empty()) {
    throw std::runtime_error(std::string(who) + ": 空の画像です");
  }
  if (a.size() != b.size() || a.channels() != b.channels()) {
    throw std::runtime_error(std::string(who) +
                             ": サイズ/チャンネル数が一致しません");
  }
}
}  // namespace

double mse(const cv::Mat& a, const cv::Mat& b) {
  check_same(a, b, "mse");
  cv::Mat fa, fb;
  a.convertTo(fa, CV_64F);
  b.convertTo(fb, CV_64F);
  cv::Mat diff = fa - fb;
  diff = diff.mul(diff);
  return cv::mean(diff)[0];
}

double psnr(const cv::Mat& a, const cv::Mat& b, double max_val) {
  const double m = mse(a, b);
  if (m <= 0.0) {
    return std::numeric_limits<double>::infinity();
  }
  return 10.0 * std::log10((max_val * max_val) / m);
}

double ssim(const cv::Mat& a, const cv::Mat& b) {
  check_same(a, b, "ssim");
  const double C1 = 6.5025;    // (0.01*255)^2
  const double C2 = 58.5225;   // (0.03*255)^2

  cv::Mat I1, I2;
  a.convertTo(I1, CV_32F);
  b.convertTo(I2, CV_32F);

  cv::Mat I1_2 = I1.mul(I1);
  cv::Mat I2_2 = I2.mul(I2);
  cv::Mat I1_I2 = I1.mul(I2);

  cv::Mat mu1, mu2;
  cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
  cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

  cv::Mat mu1_2 = mu1.mul(mu1);
  cv::Mat mu2_2 = mu2.mul(mu2);
  cv::Mat mu1_mu2 = mu1.mul(mu2);

  cv::Mat sigma1_2, sigma2_2, sigma12;
  cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
  sigma1_2 -= mu1_2;
  cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
  sigma2_2 -= mu2_2;
  cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
  sigma12 -= mu1_mu2;

  cv::Mat t1 = 2 * mu1_mu2 + C1;
  cv::Mat t2 = 2 * sigma12 + C2;
  cv::Mat numerator = t1.mul(t2);

  t1 = mu1_2 + mu2_2 + C1;
  t2 = sigma1_2 + sigma2_2 + C2;
  cv::Mat denominator = t1.mul(t2);

  cv::Mat ssim_map;
  cv::divide(numerator, denominator, ssim_map);
  return cv::mean(ssim_map)[0];
}

std::vector<double> line_profile(const cv::Mat& img, int row) {
  if (img.empty()) throw std::runtime_error("line_profile: 空の画像です");
  if (row < 0 || row >= img.rows) {
    throw std::runtime_error("line_profile: row が範囲外です");
  }
  cv::Mat f;
  img.convertTo(f, CV_64F);
  std::vector<double> out(f.cols);
  const double* p = f.ptr<double>(row);
  for (int x = 0; x < f.cols; ++x) out[x] = p[x];
  return out;
}

}  // namespace dd
