# 合成データ生成 CLI。

add_executable(generate_dataset generate_dataset.cpp)
target_link_libraries(generate_dataset PRIVATE dd_common)
