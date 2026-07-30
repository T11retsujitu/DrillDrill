dd_add_drill(sec05
  src/smoothing.cpp
  demo.cpp
  tests/test_smoothing.cpp
)
# セクション 04 の convolve を土台として再利用する（前提: セクション 04 完了）。
target_link_libraries(sec05_lib PUBLIC sec04_lib)
