#pragma once

template<typename T1, class T2>
class fpair
{
public:
    typedef T1 first_type;
    typedef T2 second_type;

    force_inline fpair() : first(T1()), second(T2())
	{	// construct from defaults
	}

    force_inline fpair(const T1 & _Val1, const T2 & _Val2) : first(_Val1), second(_Val2)
	{	// construct from specified values
	}

    template<class _Other1, class _Other2>
	force_inline fpair(fpair<_Other1, _Other2> & _Right) : first(_Right.first), second(_Right.second)
	{	// construct from compatible fpair
	}

    template<class _Other1, class _Other2>
	force_inline fpair(const fpair<_Other1, _Other2> & _Right) : first(_Right.first), second(_Right.second)
	{	// construct from compatible fpair
	}

    force_inline fpair<T1, T2> & operator=(const fpair<T1, T2> & _Right)
	{	// assign from copied fpair
	    this->first = _Right.first;
	    this->second = _Right.second;
	    return (*this);
	}

public:
	first_type first;
	second_type second;
};

// fpair TEMPLATE FUNCTIONS

template<class _Ty1, class _Ty2> 
force_inline bool operator==(const fpair<_Ty1, _Ty2>& _Left, const fpair<_Ty1, _Ty2>& _Right)
{	// test for fpair equality
    return (_Left.first == _Right.first && _Left.second == _Right.second);
}

template<class _Ty1, class _Ty2>
force_inline bool operator!=(const fpair<_Ty1, _Ty2>& _Left, const fpair<_Ty1, _Ty2>& _Right)
{	// test for fpair inequality
    return (!(_Left == _Right));
}

template<class _Ty1, class _Ty2>
force_inline bool operator<(const fpair<_Ty1, _Ty2>& _Left, const fpair<_Ty1, _Ty2>& _Right)
{	// test if _Left < _Right for pairs
    return (_Left.first < _Right.first || !(_Right.first < _Left.first) && _Left.second < _Right.second);
}

template<class _Ty1, class _Ty2> 
force_inline bool operator>(const fpair<_Ty1, _Ty2>& _Left, const fpair<_Ty1, _Ty2>& _Right)
{	// test if _Left > _Right for pairs
    return (_Right < _Left);
}

template<class _Ty1, class _Ty2> 
force_inline bool operator<=(const fpair<_Ty1, _Ty2>& _Left, const fpair<_Ty1, _Ty2>& _Right)
{	// test if _Left <= _Right for pairs
    return (!(_Right < _Left));
}

template<class _Ty1, class _Ty2> 
force_inline bool operator>=(const fpair<_Ty1, _Ty2>& _Left, const fpair<_Ty1, _Ty2>& _Right)
{	// test if _Left >= _Right for pairs
    return (!(_Left < _Right));
}

