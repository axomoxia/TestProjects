
#include <string>
#include <pthread.h>

// this is a locking class that does nothing
class MyDummyLocker
{
public:

	MyDummyLocker()
	{
		// does nothing
	}

	void lock()
	{
		// does nothing
	}

	void unlock()
	{
		// does nothing
	}

};

// a pthread bassed locking class
class MyLocker
{
public:

	MyLocker()
	{
		pthread_mutex_init(&mutex_, NULL);
	}

	void lock()
	{
		pthread_mutex_lock(&mutex_);
	}

	void unlock()
	{
		pthread_mutex_unlock(&mutex_);
	}

	~MyLocker()
	{
		pthread_mutex_destroy(&mutex_);
	}
private:

	pthread_mutex_t mutex_;

};

// T is the data type, Locker is the locking class. To implement mutex locking
// the container, use MyLocker, to not use it, use MyDummyLocker
// e.g MyContainer<std::string,MyLocker>
// The object T must be copyable.
template <class T, class Locker>
class MyContainer
{
private:

	// forward declaration of the linked list data structure
	struct Element
	{
		Element(const T& data):data_(data),prev_(NULL),next_(NULL)
		{

		}
		T data_;				// copy of the data
		Element * prev_;
		Element * next_;
	};

	// RAII templated class for locking / unlocking the mutex
	template <class LockClass>
	class MyLockerWrapper
	{
	public:

		MyLockerWrapper(LockClass& locker) : locker_(locker)
		{
			locker_.lock();
		}

		~MyLockerWrapper()
		{
			locker_.unlock();
		}

	private:

		MyLockerWrapper();

		LockClass& locker_;
	};

public:

	MyContainer() : headPtr_(NULL), tailPtr_(NULL), element_count_(0)
	{

	}

	bool isEmpty()
	{

		MyLockerWrapper<Locker> lock(locker_);
		return (element_count_ == 0);
	}

	void AddTail(const T& data)
	{
		MyLockerWrapper<Locker> lock(locker_);
		Element * pNewElement = new Element(data);
		// first element
		if (element_count_ == 0)
		{
			headPtr_ = pNewElement;
			tailPtr_ = pNewElement;
		}
		else
		{
			tailPtr_->prev_ = pNewElement;
			pNewElement->next_ = tailPtr_;
			tailPtr_ = pNewElement;
		}
		element_count_++;
	}

	T RemoveHead()
	{
		MyLockerWrapper<Locker> lock(locker_);
		T retVal;
		if (headPtr_)
		{
			Element * pElement = headPtr_;
			Element * pPrevElement = headPtr_->prev_;
			retVal = headPtr_->data_;

			// delete object pointed to by the head ptr;
			delete pElement;
			// move the head pointer to point to the new beginning of the list
			headPtr_ = pPrevElement;
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
		MyLockerWrapper<Locker> lock(locker_);
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
	T RemoveTail()
	{
		MyLockerWrapper<Locker> lock(locker_);
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

	Element * headPtr_;
	Element * tailPtr_;
	int element_count_;
	Locker locker_;


};
template <class T, class Locker>
class MyQueue : public MyContainer<T,Locker>
{
public:

	bool isEmpty()
	{
		return container_.isEmpty();
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

	MyContainer<T,Locker> container_;

};

template <class T,class Locker>
class MyStack : public MyContainer<T,Locker>
{
public:
	bool isEmpty()
	{
		return container_.isEmpty();
	}

	T Pop()
	{
		return container_.RemoveHead();
	}

	void Push(const T& data)
	{
		container_.AddHead(data);
	}


private:

	MyContainer<T,Locker> container_;
};


int main(int argc, char* argv[])
{
	MyContainer<std::string,MyLocker> test;

	test.AddHead("One");
	test.AddHead("Two");
	test.AddTail("Three");
	test.AddTail("Four");

	std::string out = test.RemoveHead();
	out = test.RemoveHead();
	out = test.RemoveTail();
	out = test.RemoveTail();

}

