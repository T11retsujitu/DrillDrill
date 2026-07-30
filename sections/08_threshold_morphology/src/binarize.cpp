#include "binarize.hpp"

#include "histogram.hpp"  // セクション 03 の compute_histogram を再利用

#include <algorithm>

// 参照実装（solutions ブランチ用）。dev ブランチではスタブに差し替える。

cv::Mat threshold_binary(const cv::Mat& src, int thresh) {
  CV_Assert(src.type() == CV_8U);
  cv::Mat dst(src.rows, src.cols, CV_8U);
  for (int y = 0; y < src.rows; ++y) {
    const uchar* s = src.ptr<uchar>(y);
    uchar* d = dst.ptr<uchar>(y);
    for (int x = 0; x < src.cols; ++x) {
      d[x] = (s[x] > thresh) ? 255 : 0;
    }
  }
  return dst;
}

int otsu_threshold(const cv::Mat& src) {
  CV_Assert(src.type() == CV_8U);
  const std::vector<int> hist = compute_histogram(src);
  const double total = static_cast<double>(src.rows) * src.cols;

  // 全体の輝度和（μ_T * total）。
  double sum_all = 0.0;
  for (int v = 0; v < 256; ++v) sum_all += v * static_cast<double>(hist[v]);

  // t を動かしながら、クラス0（<= t）とクラス1（> t）の
  // クラス間分散 σ_B² = ω0·ω1·(μ0−μ1)² を逐次更新で計算する。
  double w0 = 0.0;    // クラス0 の画素数
  double sum0 = 0.0;  // クラス0 の輝度和
  double best_sigma = -1.0;
  int best_t = 0;
  for (int t = 0; t < 256; ++t) {
    w0 += hist[t];
    if (w0 <= 0.0) continue;
    const double w1 = total - w0;
    if (w1 <= 0.0) break;
    sum0 += t * static_cast<double>(hist[t]);
    const double mu0 = sum0 / w0;
    const double mu1 = (sum_all - sum0) / w1;
    const double sigma_b = w0 * w1 * (mu0 - mu1) * (mu0 - mu1);
    if (sigma_b > best_sigma) {  // 同点なら最小の t を保持（strict >）
      best_sigma = sigma_b;
      best_t = t;
    }
  }
  return best_t;
}

namespace {

// erode/dilate は「近傍の min / max」という点だけが違う。共通化しておく。
template <typename Compare>
cv::Mat morph3(const cv::Mat& binary, Compare better) {
  CV_Assert(binary.type() == CV_8U);
  cv::Mat dst(binary.rows, binary.cols, CV_8U);
  for (int y = 0; y < binary.rows; ++y) {
    uchar* d = dst.ptr<uchar>(y);
    for (int x = 0; x < binary.cols; ++x) {
      uchar v = binary.at<uchar>(y, x);
      for (int j = -1; j <= 1; ++j) {
        const int yy = std::clamp(y + j, 0, binary.rows - 1);
        const uchar* srow = binary.ptr<uchar>(yy);
        for (int i = -1; i <= 1; ++i) {
          const int xx = std::clamp(x + i, 0, binary.cols - 1);
          if (better(srow[xx], v)) v = srow[xx];
        }
      }
      d[x] = v;
    }
  }
  return dst;
}

}  // namespace

cv::Mat erode3(const cv::Mat& binary) {
  return morph3(binary, [](uchar a, uchar b) { return a < b; });
}

cv::Mat dilate3(const cv::Mat& binary) {
  return morph3(binary, [](uchar a, uchar b) { return a > b; });
}

cv::Mat open3(const cv::Mat& binary) { return dilate3(erode3(binary)); }

cv::Mat close3(const cv::Mat& binary) { return erode3(dilate3(binary)); }
