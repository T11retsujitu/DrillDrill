# 共通ライブラリ dd_common と、その検証テスト。

add_library(dd_common
  src/image_io.cpp
  src/metrics.cpp
  src/synth.cpp
)
target_include_directories(dd_common PUBLIC
  include
  ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(dd_common PUBLIC ${OpenCV_LIBS})

# --- 共通ライブラリのテスト（CI で走らせる: ラベル "ci"） ---
add_executable(test_metrics tests/test_metrics.cpp)
target_link_libraries(test_metrics PRIVATE dd_common)
add_test(NAME common.metrics COMMAND test_metrics)
set_tests_properties(common.metrics PROPERTIES LABELS "ci")

add_executable(test_synth tests/test_synth.cpp)
target_link_libraries(test_synth PRIVATE dd_common)
add_test(NAME common.synth COMMAND test_synth)
set_tests_properties(common.synth PROPERTIES LABELS "ci")
