#include "dd/synth.hpp"

#include <opencv2/imgproc.hpp>
#include <cmath>
#include <random>
#include <stdexcept>

namespace dd::synth {

Pattern make_line_space(int width, int height, double pitch, double duty,
                        double edge_blur, double low, double high) {
  if (width <= 0 || height <= 0) {
    throw std::runtime_error("make_line_space: サイズは正である必要があります");
  }
  if (pitch <= 1.0 || duty <= 0.0 || duty >= 1.0) {
    throw std::runtime_error("make_line_space: pitch>1, 0<duty<1 が必要です");
  }

  const double linewidth = duty * pitch;
  cv::Mat img(height, width, CV_32F);

  // 1 行分の値を作り、全行にコピーする（縦ラインなので行方向は一定）。
  for (int x = 0; x < width; ++x) {
    const double phase = std::fmod(static_cast<double>(x), pitch);
    const float v = (phase < linewidth) ? static_cast<float>(high)
                                        : static_cast<float>(low);
    for (int y = 0; y < height; ++y) {
      img.at<float>(y, x) = v;
    }
  }

  // エッジの正解座標（ぼかす前の理想エッジ）を記録する。
  std::vector<double> edges;
  for (double base = 0.0; base < width; base += pitch) {
    if (base > 0.0 && base < width) edges.push_back(base);            // 立ち上がり
    const double fall = base + linewidth;
    if (fall > 0.0 && fall < width) edges.push_back(fall);           // 立ち下がり
  }

  // エッジをぼかす（SE エッジ応答の近似）。縦エッジなので実質的に水平方向にぼける。
  if (edge_blur > 0.0) {
    cv::GaussianBlur(img, img, cv::Size(0, 0), edge_blur);
  }

  Pattern p;
  p.clean = img;
  p.pitch = pitch;
  p.linewidth = linewidth;
  p.edges = std::move(edges);
  return p;
}

cv::Mat add_gaussian(const cv::Mat& img, double sigma, uint64_t seed) {
  cv::Mat f;
  img.convertTo(f, CV_32F);
  std::mt19937_64 rng(seed);
  std::normal_distribution<double> nd(0.0, sigma);
  for (int y = 0; y < f.rows; ++y) {
    float* p = f.ptr<float>(y);
    for (int x = 0; x < f.cols; ++x) p[x] += static_cast<float>(nd(rng));
  }
  return f;
}

cv::Mat add_poisson(const cv::Mat& img, double gain, uint64_t seed) {
  if (gain <= 0.0) throw std::runtime_error("add_poisson: gain>0 が必要です");
  cv::Mat f;
  img.convertTo(f, CV_32F);
  std::mt19937_64 rng(seed);
  for (int y = 0; y < f.rows; ++y) {
    float* p = f.ptr<float>(y);
    for (int x = 0; x < f.cols; ++x) {
      const double v = std::max(0.0, static_cast<double>(p[x]));
      // 電子数を lambda = v*gain とみなして Poisson サンプルし、画素値に戻す。
      // 分散は v/gain となり、gain が小さいほど（低ドーズほど）ノイズが強い。
      std::poisson_distribution<long long> pd(v * gain);
      p[x] = static_cast<float>(pd(rng) / gain);
    }
  }
  return f;
}

cv::Mat add_impulse(const cv::Mat& img, double prob, uint64_t seed) {
  if (prob < 0.0 || prob > 1.0) {
    throw std::runtime_error("add_impulse: 0<=prob<=1 が必要です");
  }
  cv::Mat f;
  img.convertTo(f, CV_32F);
  std::mt19937_64 rng(seed);
  std::uniform_real_distribution<double> ud(0.0, 1.0);
  for (int y = 0; y < f.rows; ++y) {
    float* p = f.ptr<float>(y);
    for (int x = 0; x < f.cols; ++x) {
      const double r = ud(rng);
      if (r < prob * 0.5) {
        p[x] = 0.0f;        // pepper
      } else if (r < prob) {
        p[x] = 255.0f;      // salt
      }
    }
  }
  return f;
}

cv::Mat add_scanline(const cv::Mat& img, double amplitude, double period,
                     uint64_t seed) {
  if (period <= 0.0) throw std::runtime_error("add_scanline: period>0 が必要です");
  cv::Mat f;
  img.convertTo(f, CV_32F);
  std::mt19937_64 rng(seed);
  std::uniform_real_distribution<double> ud(0.0, 2.0 * CV_PI);
  const double phase = ud(rng);
  for (int y = 0; y < f.rows; ++y) {
    const double bias =
        amplitude * std::sin(2.0 * CV_PI * y / period + phase);
    float* p = f.ptr<float>(y);
    for (int x = 0; x < f.cols; ++x) p[x] += static_cast<float>(bias);
  }
  return f;
}

}  // namespace dd::synth
