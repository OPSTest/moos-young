####brief:
这个任务是根据m_10所改写，改写了uSimMarine,hazards.txt,pMarineViewer,...
接下来要做的是增加vehicle数量并能做到更改路径点。

####开发进程：
***
14.2.25 night 10:50
完成了三个航行器，并通过重写BHV_Waypoint使航行器能够遍历搜索区域。

**问题**：
不能狗识别bngine,能识别hazards.??
***
2.28
经过调整sensor_config和max_turn_rate,仍存在识别不到的情况，但已较好。

**备注**：
通过ctrl+c不能杀掉新增加的yang的所有进程，而没终止进程再次发起进程的话就会出现
问题。这时直接找到pAntler进程，kill掉即可。

***
14.2.27
调整航行器的速度的方法：
这里涉及到三个方面：
1/uSimMarine的thrust_map的配置。
2/pHelmIvp中的domin = speed:0:4:21的设置。
3/BHV_WayPoint中speed参数的设置。对于thrust_map是指推力和速度之间的映射，默认是0：0，20：1，...，100：5。而你通过设置BHV_WayPoint中的speed设置的速度（注意这个速度应该是在Ivp中设置的domin参数中设置的速度最大值之间）就相当于设置了一个于上方对应关系一致的推力值，所以设置速度的方法是：
如果想让速度快由原来的2变为4，首先BHV_WapPoint中speed=2,thrust_map = 0:0,20:2,40:4,...,100:10,就是：左侧不变，右侧增加2倍，相当于原来同样的推力得到的速度对应值增大一倍。而domin的最大速度值的设置只要大于BHV_WayPoint中speed的值即可。

***
14.2.28
重分配的操作：
在中间的航行器betty执行完成第四个路径点后点击SIM_FAULT按钮，然后每个航行器重新执行自己的行为。

***
工作：
1 每个航行器添加way_reallocation行为（BHV_Waypoint）。
2 为了在同一时刻只有一个行为在执行，要在每个行为之间加上互锁条件
3 其他。

#####总结：
大致的demo都已经实现，对于copy_m10_targ_2没有重分配这一步，copy_m10_targ_3将两种功能集成，通过SIM_FAULT按钮实现切换。

***
14.5.10
修改了launch.sh，修复了上述**备注**中所属的问题。
>上午始终GUI上无法显示航行器，重启动电脑后也没有解决，但是下午重启后就好了，问题分析：可能之前的某一个mission的launch后会对这个m10相关的任务产生影响。   

***
14.5.21
+ 将moos和bhv文件名都重新定义，并在涉及到文件名的内容处进行了相应的修改。   
+ **这次修改**最多的部分是重写了launch.sh和clean.sh文件。特别是launch.sh中的命令行处理部分，使用了getopt进行了重写。
+ 将搜索区域（在hazards.txt文件中定义）region增大范围，使其更合理。
