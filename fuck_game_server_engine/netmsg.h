#pragma once

template <typename _buffer>
class netmsg
{
public:
	force_inline netmsg()
	{
	}
	force_inline ~netmsg()
	{
		for (size_t i = 0; i < m_tmp_str.size(); i++)
		{
			SAFE_DELETE(std::string, m_tmp_str[i]);
		}
	}
public:
	force_inline void write_int8(int8_t c, size_t num = 1)
	{
		for (size_t i = 0; i < num; i++)
		{
			write((const int8_t *)&c, 1);
		}
	}
	force_inline void write_int16(int16_t i)
	{
		write((const int8_t *)&i, sizeof(i));
	}
	force_inline void write_int32(int32_t i)
	{
		write((const int8_t *)&i, sizeof(i));
	}
	force_inline void write_int64(int64_t i)
	{
		write((const int8_t *)&i, sizeof(i));
	}
	force_inline void write_str(const char * str, size_t size = 0)
	{
		// 自动方式
		if (size == 0)
		{
			uint16_t len = (uint16_t)strlen(str);
			write((const int8_t *)&len, sizeof(len));
			write((const int8_t *)str, len);
		}
		// 补齐方式
		else
		{
			size_t len = strlen(str);

			if (size <= len)
			{
				write((const int8_t *)str, size);
			}
			else
			{
				write((const int8_t *)str, len);
				write_int8(0, size - len);
			}
		}
	}
	force_inline int8_t read_int8() const
	{
		int8_t i = 0;
		read((int8_t *)&i, sizeof(i));
		return i;
	}
	force_inline int16_t read_int16() const
	{
		int16_t i = 0;
		read((int8_t *)&i, sizeof(i));
		return i;
	}
	force_inline int32_t read_int32() const
	{
		int32_t i = 0;
		read((int8_t *)&i, sizeof(i));
		return i;
	}
	force_inline int64_t read_int64() const
	{
		int64_t i = 0;
		read((int8_t *)&i, sizeof(i));
		return i;
	}
	force_inline const char * read_str(size_t size = 0) const
	{
		std::string * p = fnew<std::string>();
		std::string & i = *p;
		// 自动方式
		if (size == 0)
		{
			uint16_t len = 0;
			read((int8_t *)&len, sizeof(len));
			i.resize(len, 0);
			read((int8_t *)i.c_str(), len);
		}
		// 补齐方式
		else
		{
			i.resize(size, 0);
			read((int8_t *)i.c_str(), size);
			i[i.size() - 1] = 0;
		}
		m_tmp_str.push_back(p);
		return i.c_str();
	}
	force_inline size_t size() const
	{
		return m_buffer.size();
	}
	force_inline void write_buffer(const int8_t * p, size_t size)
	{
		write(p, size);
	}
	force_inline void read_buffer(int8_t * p, size_t size) const
	{
		read(p, size);
	}
public:
	template <typename T>
	force_inline bool serialize(T & t) const
	{
		const uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		// 写入包头
		if (!t.write((const int8_t*)&c, c_msg_header_size))
		{
			return false;
		}

		// 写入buffer
		if (!m_buffer.serialize(t))
		{
			return false;
		}
		
		return true;
	}
	template <typename T>
	force_inline bool deserialize(T & t)
	{
		// 读出包头
		uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		if (!t.read((int8_t*)&c, c_msg_header_size))
		{
			return false;
		}
		if (c != c_msg_header)
		{
			return false;
		}

		// 读出buffer
		if (!m_buffer.deserialize(t))
		{
			return false;
		}

		return true;
	}
private:
	force_inline bool write(const int8_t * p, size_t size)
	{
		return m_buffer.write(p, size);
	}
	force_inline bool read(int8_t * out, size_t size) const
	{
		return m_buffer.read(out, size);
	}
private:
	mutable _buffer m_buffer;
	mutable std::vector<std::string *> m_tmp_str;
};

