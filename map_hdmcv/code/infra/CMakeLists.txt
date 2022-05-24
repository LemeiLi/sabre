# 查找当前目录下的所有源文件
aux_source_directory(./net net)
aux_source_directory(./stream stream)
aux_source_directory(./link link)
aux_source_directory(./pack pack)
aux_source_directory(./utils utils)

# 生成链接库
add_library(infra SHARED ${net} ${stream} ${link} ${pack} ${utils})
add_library(infra_static STATIC ${net} ${stream} ${link} ${pack} ${utils})
