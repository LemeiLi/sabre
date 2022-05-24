#########################################################################
# File Name: build.sh
# Author: XXDK
# Email: v.manstein@qq.com
# Created Time: Mon 28 Oct 2019 10:32:40 AM CST
#########################################################################
#!/bin/bash

while [ $# -ge 1 ]
do
	case "$1" in
		-c)
			shift
			cd build
			echo "cleaning..."
			make clean 
			cd ../
			if [ -d build ]; then
				rm -rf build 
			fi
			if [ -d install ]; then
				rm -rf install
			fi
		;;
	esac
done

if [ ! -d build ]; then
	mkdir build
fi

if [ ! -d install ]; then
	mkdir install
fi

cd build

if [ ! -f Makefile ]; then
	cmake ../
fi 

make && make install

cd ../ 
