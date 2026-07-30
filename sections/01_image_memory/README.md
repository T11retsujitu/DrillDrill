# セクション 01 — 画像データとメモリ

## 1. 学習目標
- `cv::Mat` の型・チャンネル・画素アクセス（`.at` / `.ptr`）を使える。
- 画素ごとの演算（ゲイン・バイアス）を自作できる。
- `const` 参照で入力を「壊さない」関数を書ける。飽和（saturate）を理解する。

## 2. 前提知識
- セクション 00。

## 3. 推定所要時間
- 約 4 時間。

## 4. 最小限の理論
- グレースケール 8bit 画像は 1 画素 = 0..255 の `uchar`。`cv::Mat` は連続した
  メモリブロックとして画素を保持する（行ごとに `.ptr<uchar>(y)` で先頭を取れる）。
- `dst = gain * src + bias` は明るさ・コントラストの基本操作。
- **飽和 (saturation)**: 計算結果が 0..255 を超えたら丸め込む。`cv::saturate_cast<uchar>` が
  これを行う。飽和させないと `uchar` はラップアラウンド（256→0）して**壊れる**。
- **const 参照** `const cv::Mat&`: 入力を読み取り専用で受け取り、誤って書き換えないための約束。

## 5. 動作を予測する問題
- `uchar` に `300` を代入すると値はいくつになる？ `saturate_cast<uchar>(300)` は？
- `gain=2, bias=50` を画素値 `200` に適用すると、飽和ありでいくつ？

## 6. 実装課題
- `src/brightness.cpp` の `apply_gain_bias(const cv::Mat& src, double gain, double bias)`。
- 要件: 各画素 `dst = saturate(gain*src + bias)`、出力は CV_8U、**src は変更しない**
  （返すのは別 Mat）。二重ループ + `saturate_cast` で実装する。

## 7. コンパイル・実行
```bash
cmake --build build -j
./build/sections/01_image_memory/sec01_demo .
```
`sec01_src.png` / `sec01_brighter.png` / `sec01_contrast.png` が作られる。

## 8. テスト・確認
```bash
ctest --test-dir build -R drill.sec01 --output-on-failure
```
飽和（200→255）と、src が変わらないことが検証される。

## 9. 結果の観察
- brighter と contrast の見た目の違いを確認する。コントラスト強調で**白飛び/黒つぶれ**が
  起きていないか（＝情報が失われていないか）を見る。

## 10. OpenCV 実装との比較
- 同じ処理は `src.convertTo(dst, CV_8U, gain, bias)` でも書ける。自作結果と一致するか、
  境界（飽和）で差が出ないかを確認する。

## 11. 考察問題
- `saturate_cast` を使わず `(uchar)(gain*s + bias)` とすると、どんな画素で何が起きる？
- ゲインを上げると失われる情報は何か（CD-SEM のエッジ計測にどう影響するか）？

## 12. 発展課題
- `.at<uchar>` 版と `.ptr<uchar>` 版の両方で実装し、大きい画像で実行時間を比べる。

## 13. 振り返り
- 「入力を壊さない」を保証する仕組み（const・別 Mat 確保）を説明できるか？
- 飽和とラップアラウンドの違いを説明できるか？
