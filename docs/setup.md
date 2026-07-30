# 環境構築（Windows + WSL2 + GCC + CMake + OpenCV）

このリポジトリは **WSL2 (Ubuntu) 上でビルド・実行**することを前提にします。
Windows 側に直接インストールする必要はありません。

## 1. WSL2 と Ubuntu

Windows PowerShell（管理者）で:

```powershell
wsl --install -d Ubuntu
```

インストール後、Ubuntu を起動してユーザーを作成します。以降のコマンドはすべて
**Ubuntu（WSL）のシェル**で実行します。

## 2. ビルドツールと OpenCV

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git libopencv-dev pkg-config
```

確認:

```bash
g++ --version          # GCC が入っていること
cmake --version        # 3.16 以上
pkg-config --modversion opencv4   # OpenCV 4.x のバージョンが表示されること
```

> 本教材は Ubuntu の apt 版 OpenCV（4.x）で動作します。特定バージョンへの依存はありません。

## 3. VS Code + WSL（任意）

- Windows に VS Code をインストールし、拡張機能 **"WSL"** を入れます。
- WSL のシェルでプロジェクトフォルダに移動し `code .` で開くと、WSL 側のファイルを
  Windows の VS Code から編集できます。
- 推奨拡張: C/C++、CMake Tools。

## 4. ビルドと実行

```bash
git clone <このリポジトリ>
cd DrillDrill
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

# 共通ライブラリのテスト（緑になるはず）
ctest --test-dir build -L ci --output-on-failure

# 合成データ生成
./build/tools/generate_dataset data/generated
```

## 5. 画像の確認方法（重要）

本教材は **結果を PNG に書き出して目で見る**ことを基本にします。理由は、WSL では
`cv::imshow` の GUI 表示が環境に依存して不安定なためです。

- 出力した PNG は、WSL のパス（例 `/home/you/DrillDrill/...`）を Windows のエクスプローラや
  VS Code で開けば確認できます。エクスプローラのアドレス欄に `\\wsl$\Ubuntu\home\you\...` と
  入力してもアクセスできます。
- `imshow` を使いたい場合は Windows 11 + WSLg なら追加設定なしで表示できることが多いですが、
  **必須ではありません**。動かない場合は無理に対応せず、PNG 書き出しで進めてください。

### よくあるトラブル

| 症状 | 対処 |
|---|---|
| `cmake: OpenCV not found` | `libopencv-dev` を入れる。`pkg-config --modversion opencv4` で確認。 |
| `imshow` で `cannot open display` | GUI は必須ではない。`dd::save` / `dd::save_view` で PNG 出力に切替。 |
| 生成画像が真っ黒/真っ白 | float 画像を直接保存していないか確認。表示用は `dd::save_view`（正規化して 8bit 化）。 |
| リンクエラー `undefined reference to cv::...` | OpenCV を `target_link_libraries` に入れているか確認（`dd_common` にリンクすれば伝播する）。 |
