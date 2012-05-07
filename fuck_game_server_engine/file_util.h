#pragma once

static force_inline bool fis_file_exist(const std::string file)
{
#if defined(WIN32)
	return ::PathFileExists(file.c_str()) == TRUE;
#else
	return access(file.c_str(), F_OK) == 0;
#endif
}

static force_inline bool fdelete_file(const std::string file)
{
#if defined(WIN32)
	return ::DeleteFile(file.c_str()) == TRUE;
#else
	return unlink(file.c_str()) == 0;
#endif
}

