/*********************************************************************************************
	> File Name:      Main.cpp
	> Author:         common
	> Mail:	          yp_abc2015@163.com 
	> Created Time:   2016-08-17 Ср. 10:09:01
	> Explanation :    
 ********************************************************************************************/

#include <iostream>
#include "threadpool.h"
using namespace std;

void *TestMyProcess (void *arg)
{
#ifdef __THREADPOOL__
	cout<<"threadid 0x"<<pthread_self()<<" is process task@@@@@@@"<<endl;
#endif
    usleep (1);          // @ 休息一秒，延长任务的执行时间，如果不休眠一段时间，由于作业太少，可能都由一个线程执行完了

    return NULL;
}

int main()
{
#ifdef __THREADPOOL__
	cout<<"main thread start,  threadid : "<<pthread_self()<<endl<<endl;
#endif

	Threadpool pool;

	int i = 0;
	pool.AddWorkToWorkQueue(TestMyProcess, (void *)i++);
	pool.AddWorkToWorkQueue(TestMyProcess, (void *)i++);
	pool.AddWorkToWorkQueue(TestMyProcess, (void *)i++);
	pool.AddWorkToWorkQueue(TestMyProcess, (void *)i++);
	pool.AddWorkToWorkQueue(TestMyProcess, (void *)i++);
	pool.AddWorkToWorkQueue(TestMyProcess, (void *)i++);

	sleep(10);

	return 0;
}
