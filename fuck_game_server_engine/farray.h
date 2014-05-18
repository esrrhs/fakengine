#pragma once

//! template farray (like stl vector) with additional features.
/** Some features are: Heap sorting, binary search methods, easier debugging.
*/
template <class T, uint32_t N>
class farray
{

public:

	//! Default constructor for empty farray.
	farray()
		: used(0), is_sorted(true)
	{
	}


	//! Copy constructor
	farray(const farray<T, N>& other)
	{
		*this = other;
	}


	//! Destructor.
	~farray()
	{
		clear();
	}


	//! Adds an element at back of farray.
	/** If the farray is too small to add this new element it is made bigger.
	\param element: Element to add at the back of the farray. */
	void push_back(const T& element)
	{
		insert(element, used);
	}


	//! Adds an element at the front of the farray.
	/** If the farray is to small to add this new element, the farray is
	made bigger. Please note that this is slow, because the whole farray
	needs to be copied for this.
	\param element Element to add at the back of the farray. */
	void push_front(const T& element)
	{
		insert(element);
	}


	//! Insert item into farray at specified position.
	/** Please use this only if you know what you are doing (possible
	performance loss). The preferred method of adding elements should be
	push_back().
	\param element: Element to be inserted
	\param index: Where position to insert the new element. */
	void insert(const T& element, uint32_t index=0)
	{
		if(index>used)
		{
			return;
		}

		// element inserted not at end
		if ( used > index )
		{
			// create one new element at the end
			data[used] = data[used-1];

			// move the rest of the farray content
			for (uint32_t i=used-1; i>index; --i)
			{
				data[i] = data[i-1];
			}
			// insert the new element
			data[index] = element;
		}
		else
		{
			// insert the new element to the end
			data[index] = element;
		}

		// set to false as we don't know if we have the comparison operators
		is_sorted = false;
		++used;
	}


	//! Clears the farray.
	void clear()
	{
		used = 0;
		is_sorted = true;
	}
	

	//! Assignment operator
	const farray<T, N>& operator=(const farray<T, N>& other)
	{
		if (this == &other)
			return *this;

		if (data)
			clear();

		used = other.used;
		is_sorted = other.is_sorted;

		for (uint32_t i=0; i<other.used; ++i)
			data[i] = other.data[i]; // data[i] = other.data[i];

		return *this;
	}


	//! Equality operator
	bool operator == (const farray<T, N>& other) const
	{
		if (used != other.used)
			return false;

		for (uint32_t i=0; i<other.used; ++i)
			if (data[i] != other[i])
				return false;
		return true;
	}


	//! Inequality operator
	bool operator != (const farray<T, N>& other) const
	{
		return !(*this==other);
	}


	//! Direct access operator
	T& operator [](uint32_t index)
	{
		if (index>=used)
		{
			FASSERT(index>=used);
			return data[0];
		}

		return data[index];
	}


	//! Direct const access operator
	const T& operator [](uint32_t index) const
	{
		if (index>=used)
		{
			FASSERT(index>=used);
			return data[used];
		}

		return data[index];
	}


	//! Gets last element.
	T& getLast()
	{
		if(!used)
		{
			FASSERT(!used);
			return data[used];
		}

		return data[used-1];
	}


	//! Gets last element
	const T& getLast() const
	{
		if(!used)
		{
			FASSERT(!used);
			return data[used];
		}

		return data[used-1];
	}


	//! Gets a pointer to the farray.
	/** \return Pointer to the farray. */
	T* pointer()
	{
		return data;
	}


	//! Gets a const pointer to the farray.
	/** \return Pointer to the farray. */
	const T* const_pointer() const
	{
		return data;
	}


	//! Get number of occupied elements of the farray.
	/** \return Size of elements in the farray which are actually occupied. */
	uint32_t size() const
	{
		return used;
	}


	//! Check if farray is empty.
	/** \return True if the farray is empty false if not. */
	bool empty() const
	{
		return used == 0;
	}


	//! Sorts the farray using heapsort.
	/** There is no additional memory waste and the algorithm performs
	O(n*log n) in worst case. */
	void sort()
	{
		if (!is_sorted && used>1)
			heapsort(data, used);
		is_sorted = true;
	}


	//! Performs a binary search for an element, returns -1 if not found.
	/** The farray will be sorted before the binary search if it is not
	already sorted. Caution is advised! Be careful not to call this on
	unsorted const arrays, or the slower method will be used.
	\param element Element to search for.
	\return Position of the searched element if it was found,
	otherwise -1 is returned. */
	int32_t binary_search(const T& element)
	{
		sort();
		return binary_search(element, 0, used-1);
	}


	//! Performs a binary search for an element if possible, returns -1 if not found.
	/** This method is for const arrays and so cannot call sort(), if the farray is
	not sorted then linear_search will be used instead. Potentially very slow!
	\param element Element to search for.
	\return Position of the searched element if it was found,
	otherwise -1 is returned. */
	int32_t binary_search(const T& element) const
	{
		if (is_sorted)
			return binary_search(element, 0, used-1);
		else
			return linear_search(element);
	}


	//! Performs a binary search for an element, returns -1 if not found.
	/** \param element: Element to search for.
	\param left First left index
	\param right Last right index.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	int32_t binary_search(const T& element, int32_t left, int32_t right) const
	{
		if (!used)
			return -1;

		int32_t m;

		do
		{
			m = (left+right)>>1;

			if (element < data[m])
				right = m - 1;
			else
				left = m + 1;

		} while((element < data[m] || data[m] < element) && left<=right);
		// this last line equals to:
		// " while((element != farray[m]) && left<=right);"
		// but we only want to use the '<' operator.
		// the same in next line, it is "(element == farray[m])"


		if (!(element < data[m]) && !(data[m] < element))
			return m;

		return -1;
	}


	//! Performs a binary search for an element, returns -1 if not found.
	//! it is used for searching a multiset
	/** The farray will be sorted before the binary search if it is not
	already sorted.
	\param element	Element to search for.
	\param &last	return lastIndex of equal elements
	\return Position of the first searched element if it was found,
	otherwise -1 is returned. */
	int32_t binary_search_multi(const T& element, int32_t &last)
	{
		sort();
		int32_t index = binary_search(element, 0, used-1);
		if ( index < 0 )
			return index;

		// The search can be somewhere in the middle of the set
		// look linear previous and past the index
		last = index;

		while ( index > 0 && !(element < data[index - 1]) && !(data[index - 1] < element) )
		{
			index -= 1;
		}
		// look linear up
		while ( last < (int32_t) used - 1 && !(element < data[last + 1]) && !(data[last + 1] < element) )
		{
			last += 1;
		}

		return index;
	}


	//! Finds an element in linear time, which is very slow.
	/** Use binary_search for faster finding. Only works if ==operator is
	implemented.
	\param element Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	int32_t linear_search(const T& element) const
	{
		for (uint32_t i=0; i<used; ++i)
			if (element == data[i])
				return (int32_t)i;

		return -1;
	}


	//! Finds an element in linear time, which is very slow.
	/** Use binary_search for faster finding. Only works if ==operator is
	implemented.
	\param element: Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	int32_t linear_reverse_search(const T& element) const
	{
		for (int32_t i=used-1; i>=0; --i)
			if (data[i] == element)
				return i;

		return -1;
	}


	//! Erases an element from the farray.
	/** May be slow, because all elements following after the erased
	element have to be copied.
	\param index: Index of element to be erased. */
	void erase(uint32_t index)
	{
		if(index>=used)
		{
			return;
		}

		for (uint32_t i=index+1; i<used; ++i)
		{
			data[i-1] = data[i]; // data[i-1] = data[i];
		}

		--used;
	}


	//! Erases some elements from the farray.
	/** May be slow, because all elements following after the erased
	element have to be copied.
	\param index: Index of the first element to be erased.
	\param count: Amount of elements to be erased. */
	void erase(uint32_t index, int32_t count)
	{
		if (index>=used || count<1)
			return;
		if (index+count>used)
			count = used-index;

		uint32_t i;

		for (i=index+count; i<used; ++i)
		{
			data[i-count] = data[i]; // data[i-count] = data[i];
		}

		used-= count;
	}


	//! Sets if the farray is sorted
	void set_sorted(bool _is_sorted)
	{
		is_sorted = _is_sorted;
	}
	
private:
	T data[N];
	uint32_t used;
	bool is_sorted:1;
};

