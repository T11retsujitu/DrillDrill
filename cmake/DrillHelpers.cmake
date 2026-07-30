# DrillDrill 用の CMake ヘルパ。
# 各セクションの CMakeLists.txt を短く保つためのユーティリティ。

# dd_add_drill(<id> <impl_source> <demo_source> <test_source>)
#
#   <id>          : セクション識別子（例: sec01）
#   <impl_source> : 学習者が実装するソース（main を含まない）
#   <demo_source> : 実行して結果を目で見るデモ（main を含む）
#   <test_source> : 検証テスト（main は testing.hpp 経由）
#
# 生成物:
#   <id>_lib   … 実装ライブラリ
#   <id>_demo  … デモ実行ファイル
#   <id>_test  … テスト（CTest に "drill" ラベルで登録。CI からは除外される）
function(dd_add_drill id impl_source demo_source test_source)
  add_library(${id}_lib ${impl_source})
  target_include_directories(${id}_lib PUBLIC include)
  target_link_libraries(${id}_lib PUBLIC dd_common)

  add_executable(${id}_demo ${demo_source})
  target_link_libraries(${id}_demo PRIVATE ${id}_lib)

  add_executable(${id}_test ${test_source})
  target_link_libraries(${id}_test PRIVATE ${id}_lib)

  add_test(NAME drill.${id} COMMAND ${id}_test)
  # "drill" ラベルは学習者が自分の実装を検証するためのもの。
  # 実装前は失敗するのが正常なので、CI(-L ci) からは除外する。
  set_tests_properties(drill.${id} PROPERTIES LABELS "drill")
endfunction()
