###v2.0 之后注释
14.5.23
>**alogview工具**：这是一个使用pLogger工具产生的记录文件去*复现*运行过程中的状态的一个工具，他可以察看在某时刻任一航行器的IvP Helm信息，IvP函数信息，以及各个变量的信息。    

这个应该加在论文中。

***
整个程序运行的流程：   
>首先在launch.sh中通过PAntler application发起所有的进程，pAntler（其他的application同样）通过读取.moos中相应的配置块（这部分的代码可能写在相应的cpp文件中）配置自身并发起其他的进程（application），其它的进程发起后，又会去.moos文件中读相应的参数进行相应配置，然后工作。    

关于MOOS周围的**application**的开发新的理解：
>可以借鉴在moos-young项目中的example中的相应案例，或着找到原处在研究下。还可以借鉴其他application的编写。    

最后把可执行文件放到bin文件夹下以便能够直接调用。
