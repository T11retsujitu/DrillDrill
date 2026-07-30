dd_add_drill(sec07
  src/edges.cpp
  demo.cpp
  tests/test_edges.cpp
)
# セクション 04 の convolve を土台として再利用する（前提: セクション 04 完了）。
target_link_libraries(sec07_lib PUBLIC sec04_lib)
