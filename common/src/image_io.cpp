#include "dd/image_io.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

namespace dd {

cv::Mat load_gray(const std::string& path) {
  cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
  if (img.empty()) {
    throw std::runtime_error("load_gray: 画像を読み込めませんでした: " + path);
  }
  return img;
}

void save(const std::string& path, const cv::Mat& img) {
  if (img.empty()) {
    throw std::runtime_error("save: 空の画像は保存できません: " + path);
  }
  if (!cv::imwrite(path, img)) {
    throw std::runtime_error("save: 保存に失敗しました: " + path);
  }
}

void save_view(const std::string& path, const cv::Mat& img) {
  if (img.empty()) {
    throw std::runtime_error("save_view: 空の画像は保存できません: " + path);
  }
  cv::Mat view;
  if (img.depth() == CV_8U) {
    view = img;  // すでに表示可能。
  } else {
    // min/max を 0..255 に線形正規化してから 8bit 化する。
    cv::normalize(img, view, 0, 255, cv::NORM_MINMAX);
    view.convertTo(view, CV_8U);
  }
  if (!cv::imwrite(path, view)) {
    throw std::runtime_error("save_view: 保存に失敗しました: " + path);
  }
}

}  // namespace dd
