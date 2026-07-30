dd_add_drill(sec08
  src/binarize.cpp
  demo.cpp
  tests/test_binarize.cpp
)
# セクション 03 の compute_histogram を大津法で再利用する（前提: セクション 03 完了）。
target_link_libraries(sec08_lib PUBLIC sec03_lib)
