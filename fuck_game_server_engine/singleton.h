#pragma once

template <class T>
class singleton
{
public:
	singleton(){}
	virtual ~singleton(){}
	static T * pData;
	static fore_inline void	release();
	static fore_inline T * ptr();
	static fore_inline T & ref();
};

template <class T>
T* singleton<T>::pData = 0;

template <class T> fore_inline
T* singleton<T>::ptr()
{
	if(!pData)
	{
		pData = fnew<T>();
	}
	return pData;
}

template <class T> fore_inline
T& singleton<T>::ref()
{
	if(!pData)
	{
		pData = fnew<T>();
	}
	return *pData;
}

template <class T> fore_inline
void singleton<T>::release()
{
	SAFE_DELETE(pData);
}


