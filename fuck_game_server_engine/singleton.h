#pragma once

template <class T>
class singleton
{
public:
	singleton(){}
	virtual ~singleton(){}
	static T * pData;
	static force_inline void	release();
	static force_inline T * ptr();
	static force_inline T & ref();
};

template <class T>
T* singleton<T>::pData = 0;

template <class T> force_inline
T* singleton<T>::ptr()
{
	if(!pData)
	{
		pData = fnew<T>();
	}
	return pData;
}

template <class T> force_inline
T& singleton<T>::ref()
{
	if(!pData)
	{
		pData = fnew<T>();
	}
	return *pData;
}

template <class T> force_inline
void singleton<T>::release()
{
	SAFE_DELETE(T, pData);
}


