#include <iostream>
#include <pthread.h>
#include <deque>
#include <mutex>
#include <thread>
#include <unistd.h>

template <class T>
class MyContainer
{
private:
	struct Element
	{
		Element(const T&& data):data_(data),prev_(nullptr),next_(nullptr)
		{


		}
		T data_;				// create copy of the data
		Element * prev_ = nullptr;
		Element * next_ = nullptr;
	};

public:

	MyContainer() : headPtr_(nullptr), tailPtr_(nullptr), element_count_(0)
	{

	}

	bool isEmpty()
	{
		return element_count_ == 0;
	}

	void AddTail(const T& data)
	{
		Element * pNewElement = new Element(data);
		// first element
		if (element_count_ == 0)
		{
			headPtr_ = pNewElement;
			tailPtr_ = pNewElement;
		}
		else
		{
			pNewElement->prev_ = tailPtr_;
			tailPtr_->next_ = pNewElement;
			tailPtr_ = pNewElement;
		}
		element_count_++;
	}

	T RemoveHead()
	{
		T retVal;
		if (headPtr_)
		{
			Element * pElement = headPtr_;
			Element * pNextElement = headPtr_;
			retVal = headPtr_->data_;

			// delete object pointed to by the head ptr;
			delete pElement;
			// move the head pointer to point to the new beginning of the list
			headPtr_ = pNextElement;
			element_count_--;
			if(element_count_==0)
			{
				tailPtr_ = NULL;

			}
		}
		return retVal;
	}
	void AddHead(const T& data)
	{
		Element * pNewElement = new Element(data);
		// first element
		if (element_count_ == 0)
		{
			headPtr_ = pNewElement;
			tailPtr_ = pNewElement;
		}
		else
		{
			headPtr_->next_ = pNewElement;
			pNewElement->prev_ = headPtr_;
			headPtr_ = pNewElement;
		}
		element_count_++;
	}
	T RemoveTail(const T& data)
	{
		T retVal;
		if (tailPtr_)
		{
			retVal = tailPtr_->data_;
			Element * pLastElement = tailPtr_;
			tailPtr_ = pLastElement->next_;
			delete pLastElement;
			element_count_--;
			if (element_count_==0)
			{
				headPtr_ = NULL;
			}

		}
		return retVal;
	}

private:



	Element * headPtr_ = nullptr;
	Element * tailPtr_ = nullptr;
	int element_count_ = 0;


};
template <class T>
class MyQueue : public MyContainer<T>
{
public:

	MyQueue()
	{

	}
	T Pop()
	{
		return MyContainer<T>::RemoveHead();
	}

	void Add(const T& data)
	{
		MyContainer<T>::AddTail(data);
	}



};

template <class T>
class MyStack : public MyContainer<T>
{

public:

	MyStack()
	{

	}

	void Push(const T& data)
	{
		MyContainer<T>::AddHead(data);
	}
	T Pop()
	{
		return MyContainer<T>::RemoveHead();
	}


};


template <class T>
class ThreadSafeQueue
{
private:

	int element_count_ = 0;
	std::mutex mutex_;

	struct Element
		{
			Element():data_(NULL),prev_(NULL),next_(NULL)
			{


			}
			T * data_;
			Element * prev_;
			Element * next_;
		};

	Element * headPtr_ = nullptr;
	Element * tailPtr_ = nullptr;

public:

	ThreadSafeQueue() :element_count_(0), headPtr_(NULL), tailPtr_(NULL)
	{

	}

	T * PopIfNotEmpty()
	{
		T * retVal = NULL;
		std::lock_guard<std::mutex> guard(mutex_);
		if (headPtr_)
		{
			retVal = headPtr_->data_;
			headPtr_ = headPtr_->next_;
			element_count_--;
			if(element_count_==0)
			{
				tailPtr_ = NULL;
			}
		}
		return retVal;
	}



public:
	void Add(T*data)
	{
		std::lock_guard<std::mutex> lck (mutex_);
		Element * pNewElement = new Element;
		pNewElement->data_ = data;
		// first element
		if (element_count_ == 0)
		{
			headPtr_ = pNewElement;
			tailPtr_ = pNewElement;
		}
		else
		{
			pNewElement->prev_ = tailPtr_;
			tailPtr_->next_ = pNewElement;
			tailPtr_ = pNewElement;
		}
		element_count_++;
	}


};

struct Thread_data
{
	pthread_mutex_t mutex_;
	std::deque<std::string> queue_;

	pthread_cond_t signal_condition_;
	pthread_cond_t die_condition_;
};
void * threadFunc1(void*thread_data)
{
	Thread_data * pThreadData = (Thread_data*)thread_data;
	std::cout << "Hello Thread" << std::endl;

	while(1)
	{
		int nRandom = rand()%15;
		std::string val = std::to_string(nRandom);
		pthread_mutex_lock(&pThreadData->mutex_);
		try
		{
			std::cout<<"< Adding " << val << std::endl;
			pThreadData->queue_.push_back(val);
		}
		catch(...)
		{

		}

		// now sleep

		pthread_mutex_unlock(&pThreadData->mutex_);
		sleep(nRandom);
	}

	std::cout << "Hello thread -gone!" << std::endl;

	return nullptr;
}


void * threadFunc2(void*thread_data)
{
	Thread_data * pThreadData = (Thread_data*)thread_data;
	std::cout << "goodbye thread " << std::endl;

	while(1)
	{
		pthread_mutex_lock(&pThreadData->mutex_);


		try
		{

			std::string val = pThreadData->queue_.front();
			pThreadData->queue_.pop_front();

			std::cout<<"> Removing " << val << std::endl;

		}
		catch(...)
		{

		}

		pthread_mutex_unlock(&pThreadData->mutex_);

	}



	std::cout << "goodbye thread -gone!" << std::endl;
}

int main(int argc, char* argv[])
{

	ThreadSafeQueue<int> intQueue;

	intQueue.Add(new int(1));
	intQueue.Add(new int(2));
	intQueue.Add(new int(3));

	int * x = intQueue.PopIfNotEmpty();
	int * x1 = intQueue.PopIfNotEmpty();
	int * x2 = intQueue.PopIfNotEmpty();
	int * x3 = intQueue.PopIfNotEmpty();

	//

	Thread_data thread_data;
	pthread_mutex_init(&thread_data.mutex_,NULL);
	pthread_cond_init(&thread_data.die_condition_, NULL);
	pthread_cond_init(&thread_data.signal_condition_, NULL);

	pthread_t t[2];

	pthread_create(&t[0],NULL,threadFunc1,(void*)&thread_data);
	pthread_create(&t[1],NULL,threadFunc2,(void*)&thread_data);


	// wait for threads to terminate....
	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);

	// all done
	pthread_mutex_destroy(&thread_data.mutex_);
	pthread_cond_destroy(&thread_data.die_condition_);
	pthread_cond_destroy(&thread_data.signal_condition_);

	std::cout << "All thread -gone!" << std::endl;
}

