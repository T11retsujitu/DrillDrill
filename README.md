# DrillDrill — C++17 + OpenCV 古典的画像処理ドリル

CD-SEM（半導体計測用 走査電子顕微鏡）画像のノイズ低減を最終目標に据えた、
**手を動かして学ぶ**C++17 + OpenCV の画像処理ドリルです。

> 目的は「OpenCV の関数を呼べるようになること」ではありません。
> **処理の原理を理解し、自分で実装し、OpenCV 実装と比較し、なぜその処理を選ぶのかを説明できる**
> ようになることです。最終的には CD-SEM 画像の寸法計測を壊さない前処理を設計できることを目指します。

---

## この教材の特徴

- **ドリル形式**: 各セクションは「学習目標 → 最小理論 → 予測 → 実装課題 → 実行 → 検証 → 考察」で構成されます。完成コードを写す形式ではありません。
- **自作 → OpenCV 比較**: 主要な処理はまず自分で実装し、その後 OpenCV 実装と速度・境界処理・精度・保守性を比較します。
- **合成データのみ**: 教材で使う画像はすべて合成（`generate_dataset` で生成）です。機密画像・実機画像は一切含みません。
- **評価重視**: PSNR / SSIM だけに頼らず、CD（線幅）計測誤差・エッジ位置・ラインプロファイルで「解析目的に対して良い前処理か」を評価します。
- **Claude Code をメンターに**: 詰まったら Claude Code に相談します。メンターは答えを最初から出さず、段階的にヒントを出します（[CLAUDE.md](CLAUDE.md) 参照）。

## 対象学習者

- 他言語（Python 等）のプログラミング経験があり、C/C++ はほぼ未経験
- 画像処理を実装・検証するために必要な C++ を、演習の中で身につけたい

## 前提環境

| 項目 | 前提 |
|---|---|
| OS | Windows + WSL2 (Ubuntu) |
| 言語 | C++17 |
| ビルド | CMake (>= 3.16) |
| コンパイラ | GCC (g++) |
| 画像処理 | OpenCV 4.x |
| エディタ | VS Code + WSL 連携（任意） |

環境構築は **[docs/setup.md](docs/setup.md)** を参照してください（`cv::imshow` が動かない場合の対処も記載）。

## クイックスタート

```bash
# 1) ビルド
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

# 2) 共通ライブラリのテスト（CI と同じ。緑になるはず）
ctest --test-dir build -L ci --output-on-failure

# 3) 合成 CD-SEM データを生成（data/generated/ に出力）
./build/tools/generate_dataset data/generated

# 4) 自分の課題（ドリル）のテストを走らせる（最初は赤。実装すると緑になる）
ctest --test-dir build -L drill --output-on-failure
```

## 学習の進め方

1. `sections/NN_.../README.md` を読み、学習目標と最小理論を確認する。
2. 「動作を予測する問題」に自分の予想を書く。
3. `sections/NN_.../src/` のスタブ（`// TODO(you):`）を実装する。
4. `cmake --build build` でビルドし、`sections/NN_.../` のデモを実行して結果画像を目で見る。
5. `ctest --test-dir build -R drill.sectionNN` で検証する（緑になれば合格）。
6. README 末尾の考察問題・振り返りに答える。
7. 詰まったら Claude Code に相談する（答えではなく、ヒントを求める）。

各ドリルテストは、実装前は**わざと赤**です（それが出発点です）。CI（`-L ci`）には
ドリルテストは含めず、共通ライブラリの検証とビルド確認だけを行うため、常に緑を保ちます。

## リポジトリ構成

```
DrillDrill/
├─ README.md              … このファイル
├─ CLAUDE.md              … Claude Code メンター規約
├─ docs/                  … 環境構築 / カリキュラム全体像 / 評価手法
├─ common/                … 共通ライブラリ（画像I/O・評価指標・合成データ・テスト補助）
├─ tools/                 … generate_dataset（合成データ生成 CLI）
├─ sections/              … セクション別ドリル（NN_名前/README.md + src + tests）
├─ final_project/         … 最終課題（CD-SEM 計測とノイズ低減の評価）
└─ progress/              … 学習進捗テンプレート
```

## カリキュラム概要（約 50 時間）

全体像は **[docs/curriculum.md](docs/curriculum.md)** を参照。ノイズ低減結果の評価方法は
**[docs/evaluation.md](docs/evaluation.md)** にまとめています。

## 解答例について

解答例は原則このブランチには含めません。以下の 2 経路で段階的に参照します。

- **Claude Code 経由**（推奨）: 十分に自力で試したうえで、ヒント → コード断片 → 解答例 の順に開示。
- **`solutions` ブランチ**: 自力で答え合わせしたいときのフォールバック。

## ライセンス

[MIT](LICENSE)。教材画像はすべて合成データで、機密画像は含みません。
