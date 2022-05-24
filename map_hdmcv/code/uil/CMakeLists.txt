# 查找当前目录下的所有源文件
aux_source_directory(. uil_srcs)
# 添加依赖的库
link_libraries(bll)
link_libraries(dal)
link_libraries(infra)
link_libraries(pthread)
# 生成链接库
add_library(uil SHARED ${uil_srcs})
add_library(uil_static STATIC ${uil_srcs})
