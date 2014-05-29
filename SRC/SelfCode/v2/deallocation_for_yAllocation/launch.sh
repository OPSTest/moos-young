#----------------------------------------------------------------
#name:    launch.sh 
#project: moos-young
#brief:   启动各个community，包含三个航行器和一个shoreside. 
#date:    14.5.13
#author:  younghz
#----------------------------------------------------------------

#! /bin/bash

TIME_WARP=1
JUST_MAKE="no"
HAZARD_FILE="hazards.txt"
#----------------------------------------------------------------
#命令行参数处理
#----------------------------------------------------------------
ARGS=`getopt -o hyt:H: --long help-long,younghz-long,timewarp-long:,hazardsfile-long: -- "$@"`

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
	printf "这里提供了四个参数，分别是：    \n"
	printf "'-y'或'--younghz'          \n"
        printf "'-h'或'--help'             \n"
        printf "'-t'或'--timewarp'         \n"
	printf "'--hazardsfile'            \n" 
        shift
        ;;
    -y|--younghz-long)
	printf "my profile:github.com/younghz\n"
	shift
	;;
    -t|--timewarp-long)
	TIME_WARP=$2
	printf "Set the timewarp :%d\n" $2
	shift 2
	;;
    -H|--hazardsfile-long)
	if [ -f $2 ]; then
	    HAZARD_FILE=$2
	    printf "Set hazardsfile:%s\n" $HAZARD_FILE
	else
	    printf "%s not found. exiting...\n" $2
	    exit 0
	fi
        shift 2
        ;;
    --)
	shift
	break
	;;
    *)
	printf "错误参数,检查\n"
	exit 1
	;;
    esac
done

#---------------------------------------------------------
#创建航行器（community）名字
#发起进程
#---------------------------------------------------------
VEHICLE1="archie"
VEHICLE2="betty"
VEHICLE3="yang"
SHORESIDE="shoreside"

printf "发起MOOS进程(community)--$VEHICLE1... (WARP=%s)\n"  $TIME_WARP
pAntler $VEHICLE1.moos >& /dev/null &
sleep 0.25
printf "发起MOOS进程(community)--$VEHICLE2... (WARP=%s)\n"   $TIME_WARP
pAntler $VEHICLE2.moos >& /dev/null &
sleep 0.25
printf "发起MOOS进程(community)--$VEHICLE3... (WARP=%s)\n"    $TIME_WARP
pAntler $VEHICLE3.moos >& /dev/null &
sleep 0.25
printf "发起MOOS进程(community)--$SHORESIDE... (WARP=%s)\n"  $TIME_WARP
pAntler $SHORESIDE.moos >& /dev/null &
printf "完事。\n"

uMAC shoreside.moos >& /dev/null &
#-----------------------------------------------------------
#退出，结束仿真
#-----------------------------------------------------------
INPUT="0"
while [ "$INPUT" != "2" -a "$INPUT" != "q" ];
do
    printf "点击'q'退出结束仿真\n"
    printf "> "
    read INPUT
done

if [ "$INPUT" = "q" -o "$INPUT" = "2" ];then
    printf "杀掉所有进程...\n"
    kill %1 %2 %3 %4
    printf "本次仿真就此结束，生成的LOG及相关信息都在任务文件夹下。完事。\n"
fi
