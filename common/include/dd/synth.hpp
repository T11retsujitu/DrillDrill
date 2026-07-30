#pragma once
// dd/synth.hpp — 合成 CD-SEM 風データとノイズモデル。
//
// なぜ合成データか:
//   - 機密画像・実機画像はリポジトリに置けない。
//   - 合成なら「真の線幅・エッジ位置」が既知なので、CD 計測誤差を定量評価できる。
//
// CD-SEM の性質を単純化して再現する:
//   - 縦方向のライン&スペース（半導体パターンの基本形）。
//   - ラインとスペースは 2 値だが、エッジは少しぼける（SE エッジ応答の近似）。
//   - 主要ノイズは信号依存の Poisson（ショット）＋読み出し Gaussian。
//     加えて走査線状（周期的）ノイズ、インパルスノイズも生成できる。
//
// 画素値はすべて CV_32F、公称レンジ 0..255 で扱う（正規化・型変換の学習のため）。

#include <opencv2/core.hpp>
#include <cstdint>
#include <vector>

namespace dd::synth {

// 合成ライン&スペースパターンと、その正解（ground truth）。
struct Pattern {
  cv::Mat clean;               // CV_32F, 0..255。ノイズを載せる前の理想画像。
  double pitch = 0.0;          // 周期 [px]（ライン+スペース）。
  double linewidth = 0.0;      // ライン幅 = CD の正解値 [px]。
  std::vector<double> edges;   // 立ち上がり/立ち下がりエッジの x 座標（サブピクセル）。
};

// 縦ライン&スペースを生成する。
//   width, height : 画像サイズ [px]
//   pitch         : 周期 [px]
//   duty          : ライン幅 / pitch（0<duty<1）。linewidth = duty*pitch。
//   edge_blur     : エッジをぼかすガウス sigma [px]（0 なら鋭いエッジ）。
//   low, high     : スペース／ラインの画素値。
Pattern make_line_space(int width, int height, double pitch, double duty,
                        double edge_blur = 1.0, double low = 40.0,
                        double high = 200.0);

// --- ノイズモデル（すべて seed で再現可能） ---

// 加法性ガウスノイズ（読み出しノイズの近似）。sigma は標準偏差。
cv::Mat add_gaussian(const cv::Mat& img, double sigma, uint64_t seed);

// ショット（Poisson）ノイズ。信号依存: 明るい画素ほど分散が大きい。
//   gain = 1 画素値あたりの電子数の比。小さいほど強いノイズ（低ドーズ）。
cv::Mat add_poisson(const cv::Mat& img, double gain, uint64_t seed);

// インパルス（salt & pepper）ノイズ。prob は各画素が壊れる確率。
cv::Mat add_impulse(const cv::Mat& img, double prob, uint64_t seed);

// 走査線状の周期ノイズ（横縞）。amplitude は振幅、period は周期 [行]。
cv::Mat add_scanline(const cv::Mat& img, double amplitude, double period,
                     uint64_t seed);

}  // namespace dd::synth
