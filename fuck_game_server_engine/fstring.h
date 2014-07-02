#pragma once

static force_inline void fstrcopy(char * des, const char * src, size_t dest_size)
{
	if (dest_size > 0)
	{
		strncpy((char*)des, (char*)src, dest_size - 1);
		des[dest_size - 1] = 0;
	}
}

static force_inline void fstrcopy(int8_t * des, const int8_t * src, size_t dest_size)
{
	if (dest_size > 0)
	{
		strncpy((char*)des, (char*)src, dest_size - 1);
		des[dest_size - 1] = 0;
	}
}

template <typename T, size_t N>  
force_inline void fstrcopy(T (&des)[N], const T * src)  
{ 
	fstrcopy((int8_t *)des, (const int8_t *)src, N);
}

//! Returns a character converted to lower case
static force_inline uint32_t locale_lower ( uint32_t x )
{
	// ansi
	return x >= 'A' && x <= 'Z' ? x + 0x20 : x;
}

//! Returns a character converted to upper case
static force_inline uint32_t locale_upper ( uint32_t x )
{
	// ansi
	return x >= 'a' && x <= 'z' ? x + ( 'A' - 'a' ) : x;
}

template <typename T, uint32_t N>
class fstring
{
public:

	typedef T char_type;

	//! Default constructor
	force_inline fstring()
		: used(0), tmpdata(T())
	{
		array[0] = 0;
	}


	//! Constructor
	force_inline fstring(const fstring<T, N>& other)
		: used(0), tmpdata(T())
	{
		*this = other;
	}

	//! Constructor from other fstring types
	template <class B, uint32_t M>
	force_inline fstring(const fstring<B, M>& other)
		: used(0), tmpdata(T())
	{
		*this = other;
	}


	//! Constructs a fstring from a float
	force_inline explicit fstring(const double number)
		: used(0), tmpdata(T())
	{
		uint8_t tmpbuf[255];
		tsnprintf((char*)tmpbuf, 255, "%0.6f", number);
		*this = tmpbuf;
	}


	//! Constructs a fstring from an int32_t
	force_inline explicit fstring(int32_t number)
		: used(0), tmpdata(T())
	{
		// store if negative and make positive

		bool negative = false;
		if (number < 0)
		{
			number *= -1;
			negative = true;
		}

		// temporary buffer for 16 numbers

		uint8_t tmpbuf[16]={0};
		uint32_t idx = 15;

		// special case '0'

		if (!number)
		{
			tmpbuf[14] = '0';
			*this = &tmpbuf[14];
			return;
		}

		// add numbers

		while(number && idx)
		{
			--idx;
			tmpbuf[idx] = (uint8_t)('0' + (number % 10));
			number /= 10;
		}

		// add sign

		if (negative)
		{
			--idx;
			tmpbuf[idx] = '-';
		}

		*this = &tmpbuf[idx];
	}


	//! Constructs a fstring from an uint32_t
	force_inline explicit fstring(uint32_t number)
		: used(0), tmpdata(T())
	{
		// temporary buffer for 16 numbers

		uint8_t tmpbuf[16]={0};
		uint32_t idx = 15;

		// special case '0'

		if (!number)
		{
			tmpbuf[14] = '0';
			*this = &tmpbuf[14];
			return;
		}

		// add numbers

		while(number && idx)
		{
			--idx;
			tmpbuf[idx] = (uint8_t)('0' + (number % 10));
			number /= 10;
		}

		*this = &tmpbuf[idx];
	}

	// hex
	force_inline explicit fstring(uint32_t number, uint32_t radix)
		: used(0), tmpdata(T())
	{
		if (radix == 10)
		{
			*this = number;
		}
		else if (radix == 16)
		{
			// temporary buffer for 16 numbers

			uint8_t tmpbuf[16]={0};
			uint32_t idx = 15;

			// special case '0'

			if (!number)
			{
				tmpbuf[14] = '0';
				*this = &tmpbuf[14];
				return;
			}

			// add numbers
			const uint8_t chars[]="0123456789ABCDEF";
			while(number && idx)
			{
				--idx;
				tmpbuf[idx] = chars[(number % 16)];
				number /= 16;
			}

			*this = &tmpbuf[idx];
		}
		else
		{
			*this = "fstring not support ";
			*this += radix;
		}
	}

	//! Constructs a fstring from a long
	force_inline explicit fstring(int64_t number)
		: used(0), tmpdata(T())
	{
		// store if negative and make positive

		bool negative = false;
		if (number < 0)
		{
			number *= -1;
			negative = true;
		}

		// temporary buffer for 16 numbers

		uint8_t tmpbuf[66]={0};
		uint32_t idx = 65;

		// special case '0'

		if (!number)
		{
			tmpbuf[64] = '0';
			*this = &tmpbuf[64];
			return;
		}

		// add numbers

		while(number && idx)
		{
			--idx;
			tmpbuf[idx] = (uint8_t)('0' + (number % 10));
			number /= 10;
		}

		// add sign

		if (negative)
		{
			--idx;
			tmpbuf[idx] = '-';
		}

		*this = &tmpbuf[idx];
	}


	//! Constructs a fstring from an unsigned long
	force_inline explicit fstring(uint64_t number)
		: used(0), tmpdata(T())
	{
		// temporary buffer for 16 numbers

		uint8_t tmpbuf[66]={0};
		uint32_t idx = 65;

		// special case '0'

		if (!number)
		{
			tmpbuf[64] = '0';
			*this = &tmpbuf[64];
			return;
		}

		// add numbers

		while(number && idx)
		{
			--idx;
			tmpbuf[idx] = (uint8_t)('0' + (number % 10));
			number /= 10;
		}

		*this = &tmpbuf[idx];
	}


	//! Constructor for copying a fstring from a pointer with a given length
	template <class B>
	force_inline fstring(const B* const c, uint32_t length)
		: used(0), tmpdata(T())
	{
		if (!c)
		{
			// correctly init the fstring to an empty one
			*this="";
			return;
		}

		used = length;

		if (used >= N)
		{
			used = N - 1;
		}

		for (uint32_t l = 0; l<used; ++l)
			array[l] = (T)c[l];

		array[used] = 0;
	}


	//! Constructor for unicode and ascii strings
	template <class B>
	force_inline fstring(const B* const c)
		: used(0), tmpdata(T())
	{
		*this = c;
	}


	//! Destructor
	force_inline ~fstring()
	{
		used = 0;
		array[0] = 0;
	}

	// data
	force_inline fstring<T,N>& operator=(const int32_t & data)
	{
		*this = fstring<T,N>(data);
		return *this;
	}
	force_inline fstring<T,N>& operator=(const uint32_t & data)
	{
		*this = fstring<T,N>(data);
		return *this;
	}
	force_inline fstring<T,N>& operator=(const int64_t & data)
	{
		*this = fstring<T,N>(data);
		return *this;
	}
	force_inline fstring<T,N>& operator=(const uint64_t & data)
	{
		*this = fstring<T,N>(data);
		return *this;
	}
	force_inline fstring<T,N>& operator=(const double & data)
	{
		*this = fstring<T,N>(data);
		return *this;
	}

	//! Assignment operator
	force_inline fstring<T,N>& operator=(const fstring<T,N>& other)
	{
		if (this == &other)
			return *this;

		used = other.size();
		if (used >= N)
		{
			used = N - 1;
		}

		const T* p = other.c_str();
		for (uint32_t i=0; i<used; ++i, ++p)
			array[i] = *p;

		array[used] = 0;

		return *this;
	}

	//! Assignment operator for other fstring types
	template <class B, uint32_t M>
	force_inline fstring<T,N>& operator=(const fstring<B,M>& other)
	{
		*this = other.c_str();
		return *this;
	}


	//! Assignment operator for strings, ascii and unicode
	template <class B>
	force_inline fstring<T,N>& operator=(const B* const c)
	{
		if (!c)
		{
			used = 0;
			array[0] = 0x0;
			return *this;
		}

		if ((void*)c == (void*)array)
			return *this;

		uint32_t len = 0;
		const B* p = c;
		do
		{
			++len;
		} while(*p++);

		len--;

		used = len;
		if (used >= N)
		{
			len = used = N - 1;
		}

		for (uint32_t l = 0; l<len; ++l)
			array[l] = (T)c[l];

		array[used] = 0;

		return *this;
	}


	//! Append operator for other strings
	force_inline fstring<T,N> operator+(const fstring<T,N>& other) const
	{
		fstring<T,N> str(*this);
		str.append(other);

		return str;
	}


	//! Append operator for strings, ascii and unicode
	template <class B>
	force_inline fstring<T,N> operator+(const B* const c) const
	{
		fstring<T,N> str(*this);
		str.append(c);

		return str;
	}


	//! Direct access operator
	force_inline T& operator [](const uint32_t index)
	{
		if(index>used)
		{
			FASSERT(index>used);
			return tmpdata;
		}
		return array[index];
	}


	//! Direct access operator
	force_inline const T& operator [](const uint32_t index) const
	{
		if(index>used)
		{
			FASSERT(index>used);
			return tmpdata;
		}
		return array[index];
	}


	//! Equality operator
	force_inline bool operator==(const T* const str) const
	{
		if (!str)
			return false;

		uint32_t i;
		for (i=0; array[i] && str[i]; ++i)
			if (array[i] != str[i])
				return false;

		return (!array[i] && !str[i]);
	}


	//! Equality operator
	force_inline bool operator==(const fstring<T,N>& other) const
	{
	    if (used != other.used)
	        return false;
	
		for (uint32_t i=0; array[i] && other.array[i]; ++i)
			if (array[i] != other.array[i])
				return false;

		return used == other.used;
	}


	//! Is smaller comparator
	force_inline bool operator<(const fstring<T,N>& other) const
	{
		for (uint32_t i=0; array[i] && other.array[i]; ++i)
		{
			const int32_t diff = array[i] - other.array[i];
			if (diff)
				return (diff < 0);
		}

		return (used < other.used);
	}


	//! Inequality operator
	force_inline bool operator!=(const T* const str) const
	{
		return !(*this == str);
	}


	//! Inequality operator
	force_inline bool operator!=(const fstring<T,N>& other) const
	{
		return !(*this == other);
	}


	//! Returns length of the fstring's content
	/** \return Length of the fstring's content in characters, excluding
	the trailing NUL. */
	force_inline uint32_t size() const
	{
		return used;
	}

	//! Informs if the fstring is empty or not.
	//! \return True if the fstring is empty, false if not.
	force_inline bool empty() const
	{
		return (size() == 0);
	}

	//! Returns character fstring
	/** \return pointer to C-style NUL terminated fstring. */
	force_inline const T* c_str() const
	{
		return array;
	}

	force_inline void clear()
	{
		array[0] = 0;
		used = 0;
	}


	//! Makes the fstring lower case.
	force_inline fstring<T,N>& make_lower()
	{
		for (uint32_t i=0; array[i]; ++i)
			array[i] = locale_lower ( array[i] );
		return *this;
	}


	//! Makes the fstring upper case.
	force_inline fstring<T,N>& make_upper()
	{
		for (uint32_t i=0; array[i]; ++i)
			array[i] = locale_upper ( array[i] );
		return *this;
	}


	//! Compares the strings ignoring case.
	/** \param other: Other fstring to compare.
	\return True if the strings are equal ignoring case. */
	force_inline bool equals_ignore_case(const fstring<T,N>& other) const
	{
		for(uint32_t i=0; array[i] && other[i]; ++i)
			if (locale_lower( array[i]) != locale_lower(other[i]))
				return false;

		return used == other.used;
	}

	//! Compares the strings ignoring case.
	/** \param other: Other fstring to compare.
	\param sourcePos: where to start to compare in the fstring
	\return True if the strings are equal ignoring case. */
	force_inline bool equals_substring_ignore_case(const fstring<T,N>&other, const int32_t sourcePos = 0 ) const
	{
		if ( (uint32_t) sourcePos >= used )
			return false;

		uint32_t i;
		for( i=0; array[sourcePos + i] && other[i]; ++i)
			if (locale_lower( array[sourcePos + i]) != locale_lower(other[i]))
				return false;

		return array[sourcePos + i] == 0 && other[i] == 0;
	}


	//! Compares the strings ignoring case.
	/** \param other: Other fstring to compare.
	\return True if this fstring is smaller ignoring case. */
	force_inline bool lower_ignore_case(const fstring<T,N>& other) const
	{
		for(uint32_t i=0; array[i] && other.array[i]; ++i)
		{
			int32_t diff = (int32_t) locale_lower ( array[i] ) - (int32_t) locale_lower ( other.array[i] );
			if ( diff )
				return diff < 0;
		}

		return used < other.used;
	}


	//! compares the first n characters of the strings
	/** \param other Other fstring to compare.
	\param n Number of characters to compare
	\return True if the n first characters of both strings are equal. */
	force_inline bool equalsn(const fstring<T,N>& other, uint32_t n) const
	{
		uint32_t i;
		for(i=0; array[i] && other[i] && i < n; ++i)
			if (array[i] != other[i])
				return false;

		// if one (or both) of the strings was smaller then they
		// are only equal if they have the same length
		return (i == n) || (used == other.used);
	}


	//! compares the first n characters of the strings
	/** \param str Other fstring to compare.
	\param n Number of characters to compare
	\return True if the n first characters of both strings are equal. */
	force_inline bool equalsn(const T* const str, uint32_t n) const
	{
		if (!str)
			return false;
		uint32_t i;
		for(i=0; array[i] && str[i] && i < n; ++i)
			if (array[i] != str[i])
				return false;

		// if one (or both) of the strings was smaller then they
		// are only equal if they have the same length
		return (i == n) || (array[i] == 0 && str[i] == 0);
	}


	//! Appends a character to this fstring
	/** \param character: Character to append. */
	force_inline fstring<T,N>& append(T character)
	{
		++used;

		if (used >= N)
		{
			used = N - 1;
			return *this;
		}

		array[used - 1] = character;
		array[used] = 0;

		return *this;
	}


	//! Appends a char fstring to this fstring
	/** \param other: Char fstring to append. */
	/** \param length: The length of the fstring to append. */
	force_inline fstring<T,N>& append(const T* const other, uint32_t length=0xffffffff)
	{
		if (!other)
			return *this;

		uint32_t len = 0;
		const T* p = other;
		while(*p)
		{
			++len;
			++p;
		}

		if (len > length)
			len = length;

		if (used + len >= N)
		{
			len = N - used - 1;
		}

		for (uint32_t l=0; l<len; ++l)
			array[l+used] = *(other+l);

		used += len;
		array[used] = 0;

		return *this;
	}


	//! Appends a fstring to this fstring
	/** \param other: String to append. */
	force_inline fstring<T,N>& append(const fstring<T,N>& other)
	{
		if (other.size() == 0)
			return *this;

		uint32_t len = other.size();

		if (used + len >= N)
		{
			len = N - used - 1;
		}

		for (uint32_t l=0; l<len; ++l)
			array[used+l] = other[l];

		used += len;
		array[used] = 0;

		return *this;
	}


	//! Appends a fstring of the length l to this fstring.
	/** \param other: other String to append to this fstring.
	\param length: How much characters of the other fstring to add to this one. */
	force_inline fstring<T,N>& append(const fstring<T,N>& other, uint32_t length)
	{
		if (other.size() == 0)
			return *this;

		if (other.size() < length)
		{
			append(other);
			return *this;
		}

		if (used + length >= N)
		{
			length = N - used - 1;
		}

		for (uint32_t l=0; l<length; ++l)
			array[l+used] = other[l];

		used += length;
		array[used]=0;

		return *this;
	}

	force_inline int32_t find(T c) const
	{
	    return findFirst(c);
	}
	
	//! finds first occurrence of character in fstring
	/** \param c: Character to search for.
	\return Position where the character has been found,
	or -1 if not found. */
	force_inline int32_t findFirst(T c) const
	{
		for (uint32_t i=0; i<used; ++i)
			if (array[i] == c)
				return i;

		return -1;
	}

	//! finds first occurrence of a character of a list in fstring
	/** \param c: List of characters to find. For example if the method
	should find the first occurrence of 'a' or 'b', this parameter should be "ab".
	\param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	\return Position where one of the characters has been found,
	or -1 if not found. */
	force_inline int32_t findFirstChar(const T* const c, uint32_t count=1) const
	{
		if (!c || !count)
			return -1;

		for (uint32_t i=0; i<used; ++i)
			for (uint32_t j=0; j<count; ++j)
				if (array[i] == c[j])
					return i;

		return -1;
	}


	//! Finds first position of a character not in a given list.
	/** \param c: List of characters not to find. For example if the method
	should find the first occurrence of a character not 'a' or 'b', this parameter should be "ab".
	\param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	\return Position where the character has been found,
	or -1 if not found. */
	template <class B>
	force_inline int32_t findFirstCharNotInList(const B* const c, uint32_t count=1) const
	{
		if (!c || !count)
			return -1;

		for (uint32_t i=0; i<used; ++i)
		{
			uint32_t j;
			for (j=0; j<count; ++j)
				if (array[i] == c[j])
					break;

			if (j==count)
				return i;
		}

		return -1;
	}

	//! Finds last position of a character not in a given list.
	/** \param c: List of characters not to find. For example if the method
	should find the first occurrence of a character not 'a' or 'b', this parameter should be "ab".
	\param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	\return Position where the character has been found,
	or -1 if not found. */
	template <class B>
	force_inline int32_t findLastCharNotInList(const B* const c, uint32_t count=1) const
	{
		if (!c || !count)
			return -1;

		for (int32_t i=(int32_t)(used-1); i>=0; --i)
		{
			uint32_t j;
			for (j=0; j<count; ++j)
				if (array[i] == c[j])
					break;

			if (j==count)
				return i;
		}

		return -1;
	}

	//! finds next occurrence of character in fstring
	/** \param c: Character to search for.
	\param startPos: Position in fstring to start searching.
	\return Position where the character has been found,
	or -1 if not found. */
	force_inline int32_t findNext(T c, uint32_t startPos) const
	{
		for (uint32_t i=startPos; i<used; ++i)
			if (array[i] == c)
				return i;

		return -1;
	}


	//! finds last occurrence of character in fstring
	/** \param c: Character to search for.
	\param start: start to search reverse ( default = -1, on end )
	\return Position where the character has been found,
	or -1 if not found. */
	force_inline int32_t findLast(T c, int32_t start = -1) const
	{
		start = ( start < 0 ? (int32_t)(used) - 1 : start );
		if (start >= (int32_t)N)
		{
			start = N - 1;
		}
		for (int32_t i=start; i>=0; --i)
			if (array[i] == c)
				return i;

		return -1;
	}

	//! finds last occurrence of a character of a list in fstring
	/** \param c: List of strings to find. For example if the method
	should find the last occurrence of 'a' or 'b', this parameter should be "ab".
	\param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	\return Position where one of the characters has been found,
	or -1 if not found. */
	force_inline int32_t findLastChar(const T* const c, uint32_t count=1) const
	{
		if (!c || !count)
			return -1;

		for (int32_t i=(int32_t)used-1; i>=0; --i)
			for (uint32_t j=0; j<count; ++j)
				if (array[i] == c[j])
					return i;

		return -1;
	}


	//! finds another fstring in this fstring
	/** \param str: Another fstring
	\param start: Start position of the search
	\return Positions where the fstring has been found,
	or -1 if not found. */
	template <class B>
	force_inline int32_t find(const B* const str, const uint32_t start = 0) const
	{
		if (str && *str)
		{
			uint32_t len = 0;

			while (str[len])
				++len;

			if (len > used)
				return -1;

			for (uint32_t i=start; i<=used-len; ++i)
			{
				uint32_t j=0;

				while(str[j] && array[i+j] == str[j])
					++j;

				if (!str[j])
					return i;
			}
		}

		return -1;
	}

    force_inline fstring<T, N> substr(uint32_t begin, bool make_lower = false ) const
	{
	    return subString(begin, make_lower);
	}
	
    force_inline fstring<T, N> subString(uint32_t begin, bool make_lower = false ) const
	{
	    return subString(begin, size() - begin, make_lower);
	}

    force_inline fstring<T, N> substr(uint32_t begin, int32_t length, bool make_lower = false ) const
	{
	    return subString(begin, length, make_lower);
	}
	
	//! Returns a substring
	/** \param begin Start of substring.
	\param length Length of substring.
	\param make_lower copy only lower case */
	force_inline fstring<T, N> subString(uint32_t begin, int32_t length, bool make_lower = false ) const
	{
		// if start after fstring
		// or no proper substring length
		if ((length <= 0) || (begin>=size()))
			return fstring<T, N>("");
		// clamp length to maximal value
		if ((length+begin) > size())
			length = size()-begin;

		fstring<T, N> o;

		int32_t i;
		if ( !make_lower )
		{
			for (i=0; i<length; ++i)
				o.array[i] = array[i+begin];
		}
		else
		{
			for (i=0; i<length; ++i)
				o.array[i] = locale_lower ( array[i+begin] );
		}

		o.array[length] = 0;
		o.used = length;

		return o;
	}


	//! Appends a character to this fstring
	/** \param c Character to append. */
	force_inline fstring<T,N>& operator += (T c)
	{
		append(c);
		return *this;
	}


	//! Appends a char fstring to this fstring
	/** \param c Char fstring to append. */
	force_inline fstring<T,N>& operator += (const T* const c)
	{
		append(c);
		return *this;
	}


	//! Appends a fstring to this fstring
	/** \param other String to append. */
	force_inline fstring<T,N>& operator += (const fstring<T,N>& other)
	{
		append(other);
		return *this;
	}

	//! Appends a fstring representation of a number to this fstring
	/** \param i Number to append. */
	force_inline fstring<T,N>& operator += (const int32_t i)
	{
		append(fstring<T,N>(i));
		return *this;
	}


	//! Appends a fstring representation of a number to this fstring
	/** \param i Number to append. */
	force_inline fstring<T,N>& operator += (const uint32_t i)
	{
		append(fstring<T,N>(i));
		return *this;
	}


	//! Appends a fstring representation of a number to this fstring
	/** \param i Number to append. */
	force_inline fstring<T,N>& operator += (const int64_t i)
	{
		append(fstring<T,N>(i));
		return *this;
	}


	//! Appends a fstring representation of a number to this fstring
	/** \param i Number to append. */
	force_inline fstring<T,N>& operator += (const uint64_t i)
	{
		append(fstring<T,N>(i));
		return *this;
	}


	//! Appends a fstring representation of a number to this fstring
	/** \param i Number to append. */
	force_inline fstring<T,N>& operator += (const double i)
	{
		append(fstring<T,N>(i));
		return *this;
	}


	//! Appends a fstring representation of a number to this fstring
	/** \param i Number to append. */
	force_inline fstring<T,N>& operator += (const float i)
	{
		append(fstring<T,N>(i));
		return *this;
	}


	//! Replaces all characters of a special type with another one
	/** \param toReplace Character to replace.
	\param replaceWith Character replacing the old one. */
	force_inline fstring<T,N>& replace(T toReplace, T replaceWith)
	{
		for (uint32_t i=0; i<used; ++i)
			if (array[i] == toReplace)
				array[i] = replaceWith;
		return *this;
	}


	//! Replaces all instances of a fstring with another one.
	/** \param toReplace The fstring to replace.
	\param replaceWith The fstring replacing the old one. */
	force_inline fstring<T,N>& replace(const fstring<T,N>& toReplace, const fstring<T,N>& replaceWith)
	{
		if (toReplace.size() == 0)
			return *this;

		const T* other = toReplace.c_str();
		const T* replace = replaceWith.c_str();
		const uint32_t other_size = toReplace.size();
		const uint32_t replace_size = replaceWith.size();

		// Determine the delta.  The algorithm will change depending on the delta.
		int32_t delta = replace_size - other_size;

		// A character for character replace.  The fstring will not shrink or grow.
		if (delta == 0)
		{
			int32_t pos = 0;
			while ((pos = find(other, pos)) != -1)
			{
				for (uint32_t i = 0; i < replace_size; ++i)
					array[pos + i] = replace[i];
				++pos;
			}
			return *this;
		}

		// We are going to be removing some characters.  The fstring will shrink.
		if (delta < 0)
		{
			uint32_t i = 0;
			for (uint32_t pos = 0; pos < used; ++i, ++pos)
			{
				// Is this potentially a match?
				if (array[pos] == *other)
				{
					// Check to see if we have a match.
					uint32_t j;
					for (j = 0; j < other_size; ++j)
					{
						if (array[pos + j] != other[j])
							break;
					}

					// If we have a match, replace characters.
					if (j == other_size)
					{
						for (j = 0; j < replace_size; ++j)
							array[i + j] = replace[j];
						i += replace_size;
						pos += other_size;
						continue;
					}
				}

				// No match found, just copy characters.
				array[i] = array[pos];
			}
			array[i-1] = 0;
			used = i - 1;

			return *this;
		}

		// We are going to be adding characters, so the fstring size will increase.
		// Count the number of times toReplace exists in the fstring so we can allocate the new size.
		uint32_t find_count = 0;
		int32_t pos = 0;
		while ((pos = find(other, pos)) != -1)
		{
			++find_count;
			++pos;
		}

		// Start replacing.
		pos = 0;
		while ((pos = find(other, pos)) != -1)
		{
			T* start = array + pos + other_size;
			T* ptr   = array + used;
			T* end   = array + delta + used;

			// Shift characters to make room for the fstring.
			while (ptr != start)
			{
				if (end < array + N)
				{
					*end = *ptr;
				}
				--ptr;
				--end;
			}

			// Add the new fstring now.
			for (uint32_t i = 0; i < replace_size; ++i)
				array[pos + i] = replace[i];

			pos += replace_size;
			used += delta;

			if (used >= N)
			{
				break;
			}
		}

		if (used >= N)
		{
			used = N - 1;
			array[used]=0;
		}
		return *this;
	}


	//! Removes characters from a fstring.
	/** \param c: Character to remove. */
	force_inline fstring<T,N>& remove(T c)
	{
		uint32_t pos = 0;
		uint32_t found = 0;
		for (uint32_t i=0; i<used; ++i)
		{
			if (array[i] == c)
			{
				++found;
				continue;
			}

			array[pos++] = array[i];
		}
		used -= found;
		array[used] = 0;
		return *this;
	}


	//! Removes a fstring from the fstring.
	/** \param toRemove: String to remove. */
	force_inline fstring<T,N>& remove(const fstring<T,N>& toRemove)
	{
		uint32_t size = toRemove.size();
		if ( size == 0 )
			return *this;
		uint32_t pos = 0;
		uint32_t found = 0;
		for (uint32_t i=0; i<used; ++i)
		{
			uint32_t j = 0;
			while (j < size)
			{
				if (array[i + j] != toRemove[j])
					break;
				++j;
			}
			if (j == size)
			{
				found += size;
				i += size - 1;
				continue;
			}

			array[pos++] = array[i];
		}
		used -= found;
		array[used] = 0;
		return *this;
	}


	//! Removes characters from a fstring.
	/** \param characters: Characters to remove. */
	force_inline fstring<T,N>& removeChars(const fstring<T,N> & characters)
	{
		if (characters.size() == 0)
			return *this;

		uint32_t pos = 0;
		uint32_t found = 0;
		for (uint32_t i=0; i<used; ++i)
		{
			// Don't use characters.findFirst as it finds the \0,
			// causing used to become incorrect.
			bool docontinue = false;
			for (uint32_t j=0; j<characters.size(); ++j)
			{
				if (characters[j] == array[i])
				{
					++found;
					docontinue = true;
					break;
				}
			}
			if (docontinue)
				continue;

			array[pos++] = array[i];
		}
		used -= found;
		array[used] = 0;

		return *this;
	}


	//! Trims the fstring.
	/** Removes the specified characters (by default, Latin-1 whitespace)
	from the begining and the end of the fstring. */
	force_inline fstring<T,N>& trim(const fstring<T,N> & whitespace = " \t\n\r")
	{
		// find start and end of the substring without the specified characters
		const int32_t begin = findFirstCharNotInList(whitespace.c_str(), whitespace.used);
		if (begin == -1)
			return (*this="");

		const int32_t end = findLastCharNotInList(whitespace.c_str(), whitespace.used);

		return (*this = subString(begin, (end +1) - begin));
	}


	//! Erases a character from the fstring.
	/** May be slow, because all elements
	following after the erased element have to be copied.
	\param index: Index of element to be erased. */
	force_inline fstring<T,N>& erase(uint32_t index)
	{
		if(index>=used)
		{
			return *this;
		}

		for (uint32_t i=index+1; i<=used; ++i)
			array[i-1] = array[i];

		--used;
		return *this;
	}

	//! verify the existing fstring.
	force_inline fstring<T,N>& validate()
	{
		// terminate on existing null
		for (uint32_t i=0; i<N; ++i)
		{
			if (array[i] == 0)
			{
				used = i;
				return *this;
			}
		}

		if (used >= N)
		{
			used = N - 1;
		}

		// terminate
		if ( used > 0 )
		{
			array[used] = 0;
		}
		else
		{
			used = 0;
		}

		return *this;
	}

	//! gets the last char of a fstring or null
	force_inline T lastChar() const
	{
		return used > 0 ? array[used-1] : 0;
	}

	//! split fstring into parts.
	/** This method will split a fstring at certain delimiter characters
	into the container passed in as reference. The type of the container
	has to be given as template parameter. It must provide a push_back and
	a size method.
	\param ret The result container
	\param c C-style fstring of delimiter characters
	\param count Number of delimiter characters
	\param ignoreEmptyTokens Flag to avoid empty substrings in the result
	container. If two delimiters occur without a character in between, an
	empty substring would be placed in the result. If this flag is set,
	only non-empty strings are stored.
	\param keepSeparators Flag which allows to add the separator to the
	result fstring. If this flag is true, the concatenation of the
	substrings results in the original fstring. Otherwise, only the
	characters between the delimiters are returned.
	\return The number of resulting substrings
	*/
	template<class container>
	force_inline uint32_t split(container& ret, const T* const c, uint32_t count=1, bool ignoreEmptyTokens=true, bool keepSeparators=false) const
	{
		if (!c)
			return 0;

		const uint32_t oldSize=ret.size();
		uint32_t lastpos = 0;
		bool lastWasSeparator = false;
		for (uint32_t i=0; i<used; ++i)
		{
			bool foundSeparator = false;
			for (uint32_t j=0; j<count; ++j)
			{
				if (array[i] == c[j])
				{
					if ((!ignoreEmptyTokens || i - lastpos != 0) &&
						!lastWasSeparator)
						ret.push_back(fstring<T,N>(&array[lastpos], i - lastpos));
					foundSeparator = true;
					lastpos = (keepSeparators ? i : i + 1);
					break;
				}
			}
			lastWasSeparator = foundSeparator;
		}
		if (used > lastpos)
			ret.push_back(fstring<T,N>(&array[lastpos], used - lastpos));
		return ret.size()-oldSize;
	}

private:

	T array[N];		// array="abc"
	uint32_t used;	// used=3
	T tmpdata;
};

//! Typedef for character strings
typedef fstring<uint8_t, c_DefaultStringBuffer> stringc;

//! Typedef for wide character strings
typedef fstring<uint16_t, c_DefaultStringBuffer> stringw;

