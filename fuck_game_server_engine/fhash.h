#pragma once

template <class T>
bool fcmp(const T & t1, const T & t2)
{
	return t1 == t2;
}

template <class T>
size_t fhash(const T & t)
{
	return size_t(t);
}
