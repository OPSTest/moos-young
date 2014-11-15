#----------------------------------------------------------------
#name:    clean.sh
#project: moos-young
#brief:   Clean the tmp and log file.
#date:    14.5.13
#author:  younghz
#----------------------------------------------------------------

#! /bin/bash

VERBOSE=""
file=""

#----------------------------------------------------------------
#命令行参数处理
#----------------------------------------------------------------
ARGS=`getopt -o hvy --long help-long,verbose-long,younghz-long\
       -- "$@"`

if [ $? != 0 ]; then
    echo "Terminating..." >&2 
    exit 1
fi

#eval防止参数中存在同名的shell命令被扩展
#set重新排列参数的顺序
eval set -- "$ARGS"

#下面处理具体的选项
while true
do
    case "$1" in
    -h|--help-long)  
	printf "这里提供了两个参数，分别是：    \n"
	printf "'-v'或'--verbose'          \n"
        printf "'-h'或'--help'             \n"
        shift
        ;;
    -v|--verbose-long)  
	VERBOSE="-v"
	shift
	;;
    -y|--younghz-long)
	printf "github.com/younghz\n"
	shift
	;;
    --)
	shift
	break
	;;
    *)
	printf "错误参数\n"
	exit 1
	;;
    esac
done

#---------------------------------------------------------
#开始清理
#---------------------------------------------------------
#printf "全部扫光！！\n"

rm -rf $VERBOSE LOG_*
rm -f  $VERBOSE *~
rm -f  $VERBOSE .LastOpenedMOOSLogDirectory

	    
