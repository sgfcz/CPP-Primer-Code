#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

struct CacheData
{
	string image;
};

queue<CacheData> Q;
const int MAX_CACHEDATA_LENGTH = 10;
mutex mutex_lock;

condition_variable condConsumer;
condition_variable condProducer;

int Count = 1;

//消费者动作 
void ConsumerActor()
{
	unique_lock<mutex> lockerConsumer(mutex_lock);
	while (Q.empty())
	{
		condConsumer.wait(lockerConsumer);
	}
	cout << "[" << this_thread::get_id() << "] "; 
	CacheData temp = Q.front();
	cout << " Data: output " << temp.image.c_str() << "    ";
	cout << Count++ << endl;
	Q.pop(); 
	condProducer.notify_one();
}

//生产者动作 
void ProducerActor()
{
	unique_lock<mutex> lockerProducer(mutex_lock);
	while (Q.size() > MAX_CACHEDATA_LENGTH)
	{
		//对列慢，生产者停止，等待消费者唤醒 
		condProducer.wait(lockerProducer);
	}
	CacheData temp;
	temp.image = "a picture";
	cout << "[" << this_thread::get_id() << "] "; 
	cout <<  " Data: input " << temp.image.c_str() << "    "; 
	cout << Count++ << endl;
	Q.push(temp);
	condConsumer.notify_one();
}

//消费者 
void ConsumerTask()
{
	while(1)
	{
		ConsumerActor();
	}	
}

//生产者 
void ProducerTask()
{
	while(1)
	{
		ProducerActor();
		std::chrono::milliseconds timespan(100);
		this_thread::sleep_for(timespan);
	}	
}

//管理线程的函数 
void Dispatch(int ConsumerNum, int ProducerNum)
{
	vector<thread> thsC;
	for (int i = 0; i < ConsumerNum; ++i)
	{
		thsC.push_back(thread(ConsumerTask));
	}
	
	vector<thread> thsP;
	for (int j = 0; j < ProducerNum; ++j)
	{
		thsP.push_back(thread(ProducerTask));
	}
	
	for (int i = 0; i < ConsumerNum; ++i)
	{
		if (thsC[i].joinable())
		{
			thsC[i].join();
		}
	}
	
	for (int j = 0; j < ProducerNum; ++j)
	{
		if (thsP[j].joinable())
		{
			thsP[j].join();
		}
	}
}

int main()
{
	Dispatch(1,5);
	return 0; 
}