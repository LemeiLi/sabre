#########################################################################
# File Name: sample.sh
# Author: XXDK
# Email: v.manstein@qq.com
# Created Time: 2019年05月31日 星期五 20时25分30秒
#########################################################################
#!/bin/bash
# NOTE

#masking all output info!
set +x

g_sspath="/home/$USER/screenshot/"
g_image_path="/media/${USER}/08C9068C115CF85E/data/image/"
g_sdk_path="/home/${USER}/cv22_code/"
g_board_ip="10.0.0.2"
#g_board_ip="192.168.10.2"
g_remote_addr="root@${g_board_ip}"
g_remote_root="${g_remote_addr}:/root/"
g_proj_lib="libmapdata.so"
g_proj_test="test_map_data"
g_sabre_path="/home/${USER}/xxdk/sabre"

# stipulate: return 0 success, 1 error
func_return_value_check()
{
	if [ $1 != 0 ]; then
		echo ">>>>>>>> capture a error! please check your operation!"
		exit 0
	else 
		#echo ">>>>>>>> check ok."
		return	
	fi
}

func_check_board_net_connect()
{
	ping -c 1 -w 3 $1 >> /dev/null 2>&1 
	if [ $? != 0 ]; then
		echo "network not connected! please check board network!"
		return 1
	else 
		echo "check board network connection ok!"
		return 0
	fi
}

func_scp()
{
	if [ -f $1 ]; then
		echo "copy $1 to $2"
		scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null $1 $2 
		if [ $? != 0 ]; then
			echo "do scp error!"
			return 1
		fi
		return 0
	else 
		echo "$1 not exist!"
		return 1
	fi
}
#-----------------------------------------------
# function sample(): start the map collect system
#-----------------------------------------------
func_sample()
{
	func_check_board_net_connect ${g_board_ip}
	func_return_value_check $?

	if [ $# -ge 1 ]; then
		case "$1" in
			-start)
				if [ ! -d ${g_image_path} ]; then
					echo "please insert the SSD!"
					return 1
				fi

				func_clean_image;
				func_return_value_check $?

				gnome-terminal -x bash -c "ssh -o StrictHostKeyChecking=no ${g_remote_addr} \"test_map_data $2\";exec bash;"
				echo "start sample, waiting for camera init 8 seconds......"; sleep 8

				if [ -d /home/${USER}/Software ]; then
					gnome-terminal -x bash -c "cd /home/${USER}/Software;echo 1 | sudo -S make start_nodes;exec bash"
					sleep 4
					firefox ----new-window "localhost:5000" &
				else 
					echo "skip make start_nodes."
				fi

				echo "start image receive process......"; sleep 4
				gnome-terminal -x bash -c "cd ${g_sabre_path}; ./rx_image; exec bash"
				return 0;;

			-stop)
				echo "stop sample"
				if [ -d /home/${USER}/Software ];then
					spwd=`pwd`;
					cd /home/${USER}/Software;
					echo 1 | sudo -S make stop_nodes;
					cd $spwd;
				else 
					echo "skip make stop_nodes."
				fi

				pids=`ssh -o StrictHostKeyChecking=no ${g_remote_addr} "ps -e | grep \"test_map_data *\" | grep -v grep"`; echo "test_map_data pids: "$pids
				pid1=$(echo $pids | awk '{printf("%s\n", $1);}')
				pid2=$(echo $pids | awk '{printf("%s\n", $8);}')
				echo $pid1; echo $pid2

				#pid_arr=($pids) # 'string 2 array': only supported by bash
				#pid1=${pid_arr[0]}; 
				#pid2=${pid_arr[7]}; 

				echo "stop 4 camera application"
				if [ $pid1 ]; then
					ssh -o StrictHostKeyChecking=no ${g_remote_addr} "kill -2 $pid1"			
				fi
				if [ $pid2 ]; then
					ssh -o StrictHostKeyChecking=no ${g_remote_addr} "kill -2 $pid2"			
				fi

				pids=`ps -e | grep "rx_image" | grep -v grep`; echo "rx_iamge pids: "
				pid1=$(echo $pids | awk '{printf("%s\n", $1);}'); echo $pid1;
				if [ $pid1 ]; then
					kill -2 $pid1
				fi
				return 0;;

			-reboot)
				func_remote_reboot; break;;
			*)
				echo "func_sample unknow command!!!"; break;;
		esac
	else 
		echo "usage: sample -start | -stop"
		return 1
	fi

	return 0
}

func_scp_map_data()
{
	if [ $# -lt 1 ]; then
		echo "usage: scp_map_data -s187 | -s106 | -code2b | -repo2b)"
		return 1
	fi

	func_check_board_net_connect ${g_board_ip} 
	func_return_value_check $?

	csvps187="lijianhua"
	csv187="lijianhua@172.22.64.4:"    # code server 

	csvps123="1"
	csv123="lijianhua@172.21.21.123:"    # xxdk dell desktop computer

	ul="/usr/lib/"
	ulb="/usr/local/bin/"

	fakeroot1="/home/${USER}/cv22_code/ambarella/out/cv22_walnut_auto/fakeroot"
	fakeroot2="/home/${USER}/DiskA/cv22_repo/ambarella/out/cv22_walnut_auto/fakeroot"
	local_path="/home/x/map_data"
	while [ $# -ge 1 ] ; do
		case "$1" in
			-path)
				path="/home/${USER}/$2/ambarella/out/cv22_walnut_auto/fakeroot"
				if [ ! -d $path ]; then
					path="/home/${USER}/$2/ambarella/out/cv22_walnut/fakeroot"
				fi
				func_scp ${path}${ul}${g_proj_lib} ${g_remote_addr}:${ul}
				func_return_value_check $?
				func_scp ${path}${ulb}${g_proj_test} ${g_remote_addr}:${ulb}
				func_return_value_check $?
				shift;;

			-s187)
				echo "get map data form $csv187 to $local_path"
				sshpass -p ${csvps187} scp ${csv187}${fakeroot}${ul}${g_proj_lib} ${local_path}
				echo "Get ${g_proj_lib} ok."

				sshpass -p ${csvps187} scp ${csv187}${fakeroot}${ulb}${g_proj_test} ${local_path}
				echo "Get ${g_proj_test} ok."
				shift;;

			-s123)
				echo "get map data form $csv123 to $local_path"
				sshpass -p ${csvps123} scp ${csv123}${fakeroot}${ul}${g_proj_lib} ${local_path}
				echo "Get ${g_proj_lib} ok."

				sshpass -p ${csvps123} scp ${csv123}${fakeroot}${ulb}${g_proj_test} ${local_path}
				echo "Get ${g_proj_test} ok."
				shift;;

			-code2b)
				func_scp ${fakeroot1}${ul}${g_proj_lib} ${g_remote_addr}:${ul}
				func_return_value_check $?
				func_scp ${fakeroot1}${ulb}${g_proj_test} ${g_remote_addr}:${ulb}
				func_return_value_check $?
				shift;;

			-repo2b)
				func_scp ${fakeroot2}${ul}${g_proj_lib} ${g_remote_addr}:${ul}
				func_return_value_check $?
				func_scp ${fakeroot2}${ulb}${g_proj_test} ${g_remote_addr}:${ulb}
				func_return_value_check $?
				shift;;

			-reboot)
				func_remote_reboot;;

			*) echo "unknown parameter $1."; exit 1;;
		esac
	done

	return 0
}

#-----------------------------------------------
# function build_map_data_code()
#-----------------------------------------------
func_build_map_data_code()
{
	if [ $# -lt 1 ]; then
		echo "usage: build_map_data_code -repo2b | -code2b"
		return 1
	fi
	
	if [ $# -eq 1 ]; then
		build_path1=/home/${USER}/cv22_code/ambarella/boards/cv22_walnut_auto
		build_path2=/home/${USER}/cv22_code/ambarella/boards/cv22_walnut
		source /home/$USER/cv22_code/ambarella/build/env/aarch64-linaro-gcc.env
		if [ -d $build_path1 ]; then
			cd $build_path1 
			make ${g_proj_lib} && make ${g_proj_test} && func_scp_map_data $1
		elif [ -d $build_path2 ]; then
			cd $build_path2 
			make ${g_proj_lib} && make ${g_proj_test} && func_scp_map_data $1
		fi

		return 0
	fi 
	
	if [ "$1" == "-path" ]; then
		build_path1=/home/${USER}/$2/ambarella/boards/cv22_walnut_auto
		build_path2=/home/${USER}/$2/ambarella/boards/cv22_walnut
		source /home/$USER/$2/ambarella/build/env/aarch64-linaro-gcc.env
		if [ -d $build_path1 ]; then
			cd $build_path1 
			make ${g_proj_lib} && make ${g_proj_test} && func_scp_map_data $1 $2
		elif [ -d $build_path2 ]; then
			cd $build_path2 
			make ${g_proj_lib} && make ${g_proj_test} && func_scp_map_data $1 $2
		fi
		return 0
	fi
}

#-----------------------------------------------
# function func_deploy()
#-----------------------------------------------
func_deploy()
{
	if [ $# -lt 1 ]
	then
		echo "usage: deploy -flc | -mav"
		return 1
	fi

	func_check_board_net_connect ${g_board_ip}
	func_return_value_check $?

	lpath="config_file"
	llua_flc="390_vin0_1080p_wdr.lua.flc" 
	llua_mav="390_vin0_1_1080p_wdr.lua.mav" 
	lccf_flc="config_camera_file_flc"
	lccf_mav="config_camera_file_mav"
	ls99_flc="S99config.flc"
	ls99_mav="S99config.mav"
	sysconf="system_config.sh"

	rlua_flc="390_vin0_1080p_wdr.lua" 
	rlua_mav="390_vin0_1_1080p_wdr.lua" 
	rccf="config_camera_file"
	rulb="/usr/local/bin"
	reis="/etc/init.d"
	rs99="S99config"

	while [ $# -ge 1 ]
	do
		case "$1" in
			-mav)
				echo "deploying mav configuration."
				func_scp ${g_sabre_path}/$lpath/$llua_mav $g_remote_addr:$rulb/$rlua_mav
				func_return_value_check $?
				func_scp ${g_sabre_path}/$lpath/$lccf_mav $g_remote_addr:$rulb/$rccf 
				func_return_value_check $?
				func_scp ${g_sabre_path}/$lpath/$ls99_mav $g_remote_addr:$reis/$rs99
				func_return_value_check $?
				func_scp ${g_sabre_path}/$lpath/$sysconf $g_remote_addr:$reis/
				func_return_value_check $?

				func_remote_reboot; 
				shift;;
			-flc)
				echo "deploying flc configuration."
				func_scp ${g_sabre_path}/$lpath/$llua_flc $g_remote_addr:$rulb/$rlua0
				func_return_value_check $?
				func_scp ${g_sabre_path}/$lpath/$lccf_flc $g_remote_addr:$rulb/$rccf 
				func_return_value_check $?
				func_scp ${g_sabre_path}/$lpath/$ls99_flc $g_remote_addr:$reis/$rs99
				func_return_value_check $?
				func_scp ${g_sabre_path}/$lpath/$sysconf $g_remote_addr:$reis/
				func_return_value_check $?

				func_remote_reboot; 
				shift;;
			-reboot)
				func_remote_reboot; 
				shift;;
			*) 
				echo "func_deploy unknow parameter $1"; exit 1; break;;
		esac
	done

	return 0
}

#-----------------------------------------------
# function conb()
#-----------------------------------------------
func_conb()
{
	func_check_board_net_connect ${g_board_ip}
	func_return_value_check $?
	echo "connect to ${g_remote_addr}"
	# -q quiet
	ssh -q -o StrictHostKeyChecking=no ${g_remote_addr} 
}

func_remote_reboot()
{
	func_check_board_net_connect ${g_board_ip}
	func_return_value_check $?

	echo "remote rebooting ${g_remote_addr}......"
	# -q quiet
	ssh -q -o StrictHostKeyChecking=no ${g_remote_addr} "reboot";
}

#-----------------------------------------------
# function ()
#-----------------------------------------------
func_cons()
{
	sshpass -p "lijianhua" ssh lijianhua@172.22.64.4
}

func_rx_image()
{
	if [ -d ${g_sabre_path} -a -f ${g_sabre_path}/rx_image ]; then
		cd ${g_sabre_path}; ./rx_image;
		exit 0
	else 
		return 1
	fi
}

func_clean_image()
{
	while [ $# -ge 1 ]
	do
		case "$1" in
			--path)
				shift
				g_image_path=$1
				shift
				continue;;
			*) 
				echo "func_clean_image unknow parameter: $1"; exit 1; break;;
		esac
	done

	if [ -d ${g_image_path} ]; then
		counter=`ls ${g_image_path} -lR | grep "^-" | wc -l""`
		ssize=`du ${g_image_path} -sh`
		size=$(echo ${ssize} | awk '{printf("%s\n", $1);}')
		echo "removing ${counter}(${size}) images file in ${g_image_path}"
		#find ${g_image_path} -name '*.jpg' -type f -print -exec rm -f {} \;
		find ${g_image_path} -name '*.jpg' -type f -exec rm -f {} \;
		return ${counter}
	fi

	return 1
}

func_cycle_clean_image()
{
	local cycle=100

	while [ $# -ge 1 ]
	do
		case "$1" in
			--path)
				shift
				g_image_path=$1
				shift;continue;;
			--cycle)
				shift
				cycle=$1
				shift;continue;;
			*) 
				echo "func_cycle clean image unknow parameter: $1"; exit 1; break;;
		esac
	done

	if [ -d ${g_image_path} ]; then
		while [ 1 ]
		do
			func_clean_image;
			total=$(($total + $?))
			echo "clean ${total} images"
			sleep ${cycle}
		done 
	else 
		return 1
		echo "not find valid image path!"
	fi 
}

func_screenshot()
{
	if [ ! -d "$g_sspath" ]; then
		mkdir $g_sspath
		echo "mkdir $g_sspath"
	fi

	extent=10 #unit: second
	if [ $# -ge 1 ]; then
		extent=$1	
	fi

	while [ 1 ]
	do
		local file="${g_sspath}`date +"%Y%m%d-%H%M%S"`.jpg"
		gnome-screenshot -f $file
		echo "save screenshot: $file"
		sleep $extent	
	done 
	
}

func_usage()
{
	echo "------------------------------------------------------------------------"
	echo ">>  a. -build<bash>   1 --code    2 --repo  3 --sdk  4 --path"
	echo ">>  b. -deploy        1 --mav     2 --flc   "
	echo ">>  c. -sample        1 --start e 2 --stop  "
	echo ">>  d. -scp           1 --s187    2 --s123  3 --repo 4 --code 5 --path"
	echo ">>  e. -reboot        | -cons     | -conb   | -clean | -cc    | -rx   | -ss<s>"
	echo "------------------------------------------------------------------------"
}

#-----------------------------------------------
# function main()
#-----------------------------------------------
main_process() 
{
	if [ $# -lt 1 ]
	then
		func_usage;
		exit 0
	fi

	while [ $# -ge 1 ]
	do
		case "$1" in
			-build)
				shift
				case "$1" in
					--code)
						shift;
						echo "bash build_map_data_code -code2b"
						func_build_map_data_code -code2b
						func_return_value_check $?
						;;
					--repo)
						shift;
						echo "bash build_map_data_code -repo2b"
						func_build_map_data_code -repo2b
						func_return_value_check $?
						;;
					--sdk)
						shift;
						echo "bash ${g_sdk_path}ambarella/build.sh"
						cd ${g_sdk_path}ambarella/
						bash build.sh
						;;
					--path)
						shift;
						echo "bash build_map_data_code <$@>"
						func_build_map_data_code -path $@
						func_return_value_check $?
						;;
					*)
						echo "error command"
						shift;;
				esac; 
				continue;;
			-deploy)
				shift
				case "$1" in
					--flc)
						shift;
						echo "deploy -flc"
						func_deploy -flc $@
						func_return_value_check $?
						;;
					--mav)
						shift;
						echo "deploy -mav"
						func_deploy -mav $@
						func_return_value_check $?
						;;
					*)
						echo "error command"
						shift;;
				esac; 
				continue;;
			-sample)
				shift
				case "$1" in
					--start)
						shift;
						echo "sample -start $@"
						func_sample -start $@
						func_return_value_check $?
						shift;;
					--stop)
						shift;
						echo "sample -stop"
						func_sample -stop
						func_return_value_check $?
						;;
					*)
						echo "error command"
						shift;; 
				esac;
				continue;;
			-scp)
				shift
				case "$1" in
					--s187)
						shift;
						echo "scp_map_data -s187"
						func_scp_map_data -s197
						func_return_value_check $?
						;;
					--s123)
						shift;
						echo "scp_map_data -s123"
						func_scp_map_data -s123
						func_return_value_check $?
						;;
					--repo)
						shift;
						echo "scp_map_data -repo2b"
						func_scp_map_data -repo2b
						func_return_value_check $?
						;;
					--code)
						shift;
						echo "scp_map_data -code2b"
						func_scp_map_data -code2b
						func_return_value_check $?
						;;
					--path)
						shift;
						echo "scp $1 to $g_remote_root"
						func_scp $1 $g_remote_root
						func_return_value_check $?
						;;
					*)
						echo "error command"
						shift;;
				esac; 
				continue;;
			-rx)
				func_rx_image; 
				func_return_value_check $?
				break;;
			-reboot)
				func_remote_reboot; break;;
			-cons)
				func_cons; break;;
			-conb)
				func_conb; break;;
			-clean)
				shift
				func_clean_image $@; 
				break;;
			-cc)
				shift;
				func_cycle_clean_image $@; break;; # block in
			-ss)
				shift;
				func_screenshot $@; break;;        # block in
			*) 
				echo "main_process unknow parameter $@"; exit 1;;
		esac
	done
} #end main

# >entry point<
main_process $@

set -x











