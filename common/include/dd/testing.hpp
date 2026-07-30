#pragma once
// dd/testing.hpp — 依存の少ない最小テストヘルパ。
//
// GoogleTest を導入する前の序盤セクションでも使えるよう、ヘッダオンリーにしてある。
// 各テストは自分の main を持ち、失敗が 1 つでもあれば非ゼロ終了する（CTest 連携）。
//
// 使い方:
//   #include "dd/testing.hpp"
//   int main() {
//     DD_CHECK(1 + 1 == 2);
//     DD_CHECK_EQ(gain_bias(100, 1.0, 10.0), 110);
//     DD_CHECK_NEAR(psnr, 30.0, 0.5);
//     return dd::testing::summary("my_test");
//   }

#include <cmath>
#include <cstdio>

namespace dd::testing {

inline int& failures() { static int f = 0; return f; }
inline int& checks()   { static int c = 0; return c; }

inline void report(bool ok, const char* expr, const char* file, int line) {
  ++checks();
  if (!ok) {
    ++failures();
    std::fprintf(stderr, "  [FAIL] %s:%d  %s\n", file, line, expr);
  }
}

// 近似比較（浮動小数点用）。
inline bool near(double a, double b, double eps) {
  return std::fabs(a - b) <= eps;
}

// テスト終了時に呼ぶ。失敗が無ければ 0、あれば 1 を返す。
inline int summary(const char* name) {
  if (failures() == 0) {
    std::fprintf(stderr, "[PASS] %s  (%d checks)\n", name, checks());
    return 0;
  }
  std::fprintf(stderr, "[FAIL] %s  (%d of %d checks failed)\n",
               name, failures(), checks());
  return 1;
}

}  // namespace dd::testing

#define DD_CHECK(cond) \
  ::dd::testing::report((cond), #cond, __FILE__, __LINE__)

#define DD_CHECK_EQ(a, b) \
  ::dd::testing::report((a) == (b), #a " == " #b, __FILE__, __LINE__)

#define DD_CHECK_NEAR(a, b, eps) \
  ::dd::testing::report(::dd::testing::near((double)(a), (double)(b), (eps)), \
                        #a " ~= " #b, __FILE__, __LINE__)
