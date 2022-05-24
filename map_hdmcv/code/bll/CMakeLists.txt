# 查找当前目录下的所有源文件
aux_source_directory(. bll_srcs)
# 生成链接库
add_library(bll SHARED ${bll_srcs})
add_library(bll_static STATIC ${bll_srcs})
