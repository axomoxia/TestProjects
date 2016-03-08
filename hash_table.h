#ifndef _hash_table_h
#define _hash_table_h
#include <vector>
#include <functional>
#include <algorithm>

template< typename  FUNC_KEY, typename FUNC_ELEMENT>
	struct find_first_in_pair : public std::binary_function<FUNC_ELEMENT , FUNC_KEY, bool>
	{
	public:

		bool operator()(const FUNC_ELEMENT& element, const FUNC_KEY& key) const
		{
			return element.first == key;
		}
	};


template <typename KEY, typename VALUE>
class hash_table
{
public:
	hash_table(const typename std::vector<std::pair<KEY,VALUE> >::size_type size,
			   std::size_t (*a_hash_func)(const KEY& a_key)) :
				   	   	   	   	   hash_func_(a_hash_func),
				   	   	   	   	   hash_table_data_(new HASH_ELEMENT[size]),
				   	   	   	   	   size_(size)
	{

	}

	void add(const KEY& key, const VALUE& value)
	{
		std::size_t index = hash_func_(key) % size_;
		hash_table_data_[index].push_back(std::make_pair(key,value));
	}

	bool get(const KEY& key, VALUE& val)
	{
		std::size_t index = hash_func_(key) % size_;
		typename HASH_ELEMENT::iterator it = std::find_if(hash_table_data_[index].begin(),
												 hash_table_data_[index].end(),
												 std::bind2nd (find_func(), key) );
		if (it!=hash_table_data_[index].end())
		{
			val = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual ~hash_table()
	{
		delete [] hash_table_data_;
	}

private:


	typedef std::pair<KEY, VALUE> ELEMENT;
	typedef std::vector<ELEMENT> HASH_ELEMENT;
	typedef find_first_in_pair<KEY, ELEMENT> find_func;

	std::size_t (*hash_func_)(const KEY& a_key);
	HASH_ELEMENT* hash_table_data_;
	typename HASH_ELEMENT::size_type size_;
};
#endif
