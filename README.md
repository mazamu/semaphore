# semaphore

- 留意信号量的使用
信号量分为**无名信号量**和**有名信号量**。
<br>
pc1.c
有名信号量才能够在不同进程间通信，使用sem_open()来定义。

pc2.c
如果使用的是无名信号量可以用sem_init()来定义，
这个情况下不能使用fork()，因为fork()是写时复制，修改信号量时，子进程的信号量变，但是父进程的信号量不变，这时实现不了通信，
要想实现通信，则需要使用pthread来创建进程中的线程。
<br>
编译运行：gcc -o pc1 pc1.c -lpthread，没有 -lpthread无法编译。

p1.c和p2.c都是用来模拟生产者和消费者问题的。
