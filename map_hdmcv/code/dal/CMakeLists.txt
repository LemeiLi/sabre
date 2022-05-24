# 查找当前目录下的所有源文件
aux_source_directory(. dal_srcs)
link_libraries(pthread)
link_libraries(infra)
# 生成链接库
add_library(dal SHARED ${dal_srcs})
add_library(dal_static STATIC ${dal_srcs})
