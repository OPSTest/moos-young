    这个例子主要讲述从.moos文件中读取配置信息，通常所有的进程都共享同一个moos文件。每个进程都在moos文件中自己的配置快中读取信息。
    解析配置块：
    MOOSGenLib库包含许多帮助解析任务文件的类和函数，CMOOSApp中有一个叫m_MissionReader的对象，当调用OnStartUp时，这个对象就已经准备就绪使用。



其他：
    每个继承自CMOOSApp的进程都会发送名为XXX_STATUS的mail，包含进程的uptime以及订阅和发布的信息。



***
14.5.27
+ 可以接收到SIM-FAULT发射的FAULT信息。
+ 中间遇到一种情况是：点击SIM-FAULT按钮后我修改的航行器yang处于停止状态，原因是我所设置的updates所在的行为必须是在FAULT变量是fault时才运行，所以这时yang找不到自己应该运行的行为。
+ 现阶段航行器可以响应updates,也就是能够在按下按钮后根据航行器发送的ALLOTION_UPDATES信息更新自身的状态。

***
14.5.28
建立application并应用的原理与流程：
>+ 根目录和src目录下的CMakeLists.txt的编写。主要是为了使用库以及生成所需的app（也就是可执行文件）。
>+ 按照MOOS框架编写app程序，对于5.27所述功能，需要的是注册变量FAULT，在iterate中去接收变量的发送并做相应的处理（这里是发送notify相应的updates对应信息）
>+ 编写完成后在build文件夹下`cmake ..`,然后`make`，然后在build下的bin文件夹下找到生成的可执行文件放到moos-ivp/bin下。在.moos配置文件中将工具加入即可运行，当然.moos中还要进行updates等一系列的配置。
