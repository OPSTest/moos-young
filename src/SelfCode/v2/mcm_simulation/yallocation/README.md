
***
14.5.27
+可以接收到SIM-FAULT发射的FAULT信息。
+中间遇到一种情况是：点击SIM-FAULT按钮后我修改的航行器yang处于停止状态，原因是我所设置的updates所在的行为必须是在FAULT变量是fault时才运行，所以这时yang找不到自己应该运行的行为。
+现阶段航行器可以响应updates,也就是能够在按下按钮后根据航行器发送的ALLOTION_UPDATES信息更新自身的状态。

***
14.5.28
建立application并应用的原理与流程：
>+根目录和src目录下的CMakeLists.txt的编写。主要是为了使用库以及生成所需的app（也就是可执行文件）。
>+按照MOOS框架编写app程序，对于5.27所述功能，需要的是注册变量FAULT，在iterate中去接收变量的发送并做相应的处理（这里是发送notify相应的updates对应信息）
>+编写完成后在build文件夹下`cmake ..`,然后`make`，然后在build下的bin文件夹下找到生成的可执行文件放到moos-ivp/bin下。在.moos配置文件中将工具加入即可运行，当然.moos中还要进行updates等一系列的配置。

***
14.6.5
关于按照文章的reldist和cost公式优化结果的思考：
首先，按照reldist划分区域的结果是合理并正确的。
其次，根据cost的结果确定路径点的访问顺序在路径点全部在一条直线上的情况下是最优的，但是除了上述特殊情况，它最终的效果都不如使用其他方式优化（其实访问路径点的问题就是TSP问题，可以用我之前作业中的模拟退火方式优化），并且还存在一个缺点就是无法固定终点。

附：关于上述“其次”中的论证可参见Distributed Multi-AUV Coordination in Naval Mine Countermeasure Missions(11.21).pdf和另一篇Efficient Bids on Task Allocation for Multi-Robot Exploration。

对于这个问题的分析补充，我在论文的第五章处也做了相关的论述。


***
14.6.6
**代码组织失误**
>关于yallocation和yglobalinformation这两个应用，在代码的组织上有一点小失误。体现在对OnNewMail和Iterate()两个函数的理解上，在上面两个应用中，我吧所有的处理代码都放到了OnNewMail()中，在Iterate()中没有写任何代码。就MOOS架构来说，合理的组中方式是在OnNewMail()中只获取接收到的数据的名字（name）与内容(value)，并对内容做相应存储操作，并根据获得的数据执行一些变量的赋值等操作。也就是说在这里只围绕数据做相关的处理，并不涉及其他的关于应用程序功能的代码。关于功能实现的代码（notify等操作也算）应该是放在Iterate()中。    
>而在我开发的这两个程序中，所有程序都放在了OnNewMail()中。但并不影响程序的实现，在这里，由于时间关系，先不进行改动。

***
14.6.6
TODO:
>针对14.6.5的分析，若想要使结果更好看，现阶段想到的就是使用上面提出的TSP优化，既然方法明确又已经在之前实现过，在这里就将这种思路直接简化为为updates参数赋定值方式达到效果。
