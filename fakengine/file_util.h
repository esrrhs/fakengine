#pragma once

static force_inline bool fis_file_exist(const stringc & file)
{
#if defined(WIN32)
	return ::PathFileExists((const char *)file.c_str()) == TRUE;
#else
	return access((const char *)file.c_str(), F_OK) == 0;
#endif
}

static force_inline bool fdelete_file(const stringc & file)
{
#if defined(WIN32)
	return ::DeleteFile((const char *)file.c_str()) == TRUE;
#else
	return unlink((const char *)file.c_str()) == 0;
#endif
}
