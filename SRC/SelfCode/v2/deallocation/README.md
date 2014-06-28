###v2.0 之后注释
14.5.23
>**alogview工具**：这是一个使用pLogger工具产生的记录文件去*复现*运行过程中的状态的一个工具，他可以察看在某时刻任一航行器的IvP Helm信息，IvP函数信息，以及各个变量的信息。
>>14.5.26补充：关于这个工具，还能完成uPalyBack的作用（找到了这个工具，但没有找到用法的相应文献。），即使用菜单栏中的palyback菜单（也存在相应的快捷键），可以进行连续的回放。

这个应该加在论文中。

***
整个程序运行的流程：   
>首先在launch.sh中通过PAntler application发起所有的进程，pAntler（其他的application同样）通过读取.moos中相应的配置块（这部分的代码可能写在相应的cpp文件中）配置自身并发起其他的进程（application），其它的进程发起后，又会去.moos文件中读相应的参数进行相应配置，然后工作。    

关于MOOS周围的**application**的开发新的理解：
>可以借鉴在moos-young项目中的example中的相应案例，或着找到原处在研究下。还可以借鉴其他application的编写。    

最后把可执行文件放到bin文件夹下以便能够直接调用。

***
14.5.24
问题：
>他大爷的，有出现了三个航行器node不能运行的情况了，有病乱投医，想到了缓存的方面，想清理一下缓存，于是茶下相关的命令，执行了sudo apt-get clean(清楚所有软件缓存），没有效果，于是sodu apt-get autoclean(清楚旧版本软件缓存），没有效果，最后sudo apt-get autoremove(清楚不再使用的孤立软件），呀，好了。天知道是不是相关，下次出问题的时候在时一下。   

***
14.5.26   
关于单航行器内部和多航行器之间通信问题整理：
+ 单航行器：   

>对于单航行器，航行器中运行的各个application之间的通信方式正是MOOS的精髓，编码方式可见《Introduction to programming with MOOS》。      
+ 多航行器：   

>对于多航行器，一般是通过uField Toolbox的工具完成的：    

>>+ pHostInfo:这个在shoreside和vehicle端都存在，主要检测涉及到通信的IP，端口等相关信息。
>>+ uFldNodeBroker:只在vehicle上运行，用作与shoreside之间建立连接。
>>+ uFldShoreBroker:只在shoreside上运行，从航行器处获取信息。还有一个作用就是**使航行器能够识别用户在shoreside端所定义的变量**。比如定义的按钮FAULT变量。
>>+ uFldMessageHandler:上面的工具是接受与发送，这个工具是处理信息。
>>+ uFldNodeComms:这个才是各个航行器之间能够通信的工具。它运行在shoreside端。它作为中间人接收vehicle端发送的信息并有条件的将信息发送出去。
