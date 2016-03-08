#include <iostream>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <pthread.h>
#include <deque>

template <class T>
class MyContainer
{
private:
	struct Element
	{
		Element(const T& data):data_(data),prev_(NULL),next_(NULL)
		{


		}
		T data_;				// create copy of the data
		Element * prev_;
		Element * next_;
	};

public:

	MyContainer() : headPtr_(NULL), tailPtr_(NULL), element_count_(0)
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
			tailPtr_ = pLastElement.next_;
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



	Element * headPtr_;
	Element * tailPtr_;
	int element_count_;


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
		return container_.RemoveHead();
	}

	void Add(const T& data)
	{
		container_.AddTail(data);
	}

private:

	MyContainer<T> container_;

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
		container_.AddHead(T)
	}
	T Pop()
	{
		return container_.RemoveHead(T);
	}
};

class MyLocker()
{
public:

	MyLocker()
	{

	}

}
template <class T>
class ThreadSafeQueue
{
public:

	ThreadSafeQueue() : headPtr_(NULL), tailPtr_(NULL), element_count_(0)
	{

	}

	T * PopIfNotEmpty()
	{
		T * retVal = NULL;
		boost::lock_guard<boost::mutex> guard(mutex_);
		if (headPtr_)
		{
			Element * pElement = headPtr_;
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

private:

	struct Element
		{
			Element():data_(NULL),prev_(NULL),next_(NULL)
			{


			}
			T * data_;
			Element * prev_;
			Element * next_;
		};

public:
	void Add(T*data)
	{
		boost::lock_guard<boost::mutex> guard(mutex_);
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

private:

	Element * headPtr_;
	Element * tailPtr_;
	int element_count_;
	boost::mutex mutex_;
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
		std::string val = boost::lexical_cast<std::string>(nRandom);
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
	void * 	  tRes[2];
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

