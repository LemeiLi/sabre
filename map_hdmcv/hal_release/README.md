
| Version| Description    | Date       | Maintainer             |
| ----:  | :----:         | :----      | :----                  |
| v1.0.0 | First edition. | 2019-12-06 | XXDK,v.manstein@qq.com |

---
<h2 style="color:red"> 1.说明 </h2>
- **bin**    <span style="color:blue">存放编译好的测试文件</span>
- **config** <span style="color:blue">存放配置文件</span>
- **inc**    <span style="color:blue">存放头文件</span>
- **lib**    <span style="color:blue">存放库文件</span>
- **test**   <span style="color:blue">存放测试文件</span>

---
<h2 style="color:red"> 2.编译 </h2>
- <span style="color:blue">make</span>

---
<h2 style="color:red"> 3.运行 </h2>
**进入test目录**
- 切换到root: <span style="color:blue">sudo -s </span>
  + 导入库路径: <span style="color:blue">export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/xxxx/release/lib </span>
  + 拷贝config下的json 和conf 文件到可执行文件同级目录
- 运行(需要root权限): 
  + 图像:  <span style="color:blue">./test_image </span>
  + 惯导:  <span style="color:blue">./test_insgps </span>
  + lidar: <span style="color:blue">./test_lidar </span>

---
<h2 style="color:red"> 4.文档 </h2>
**安装doxygen(已安装则跳过)**
- <span style="color:blue">sudo apt-get install doxygen </span>
- <span style="color:blue">sudo apt-get install graphviz </span>

**生成文档**
- <span style="color:blue">make docs </span>
- <span style="color:blue">firefox ./docs/html/index.html </span>



