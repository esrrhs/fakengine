#pragma once

template <typename _fserialize>
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
			m_fserialize.serialize(c);
		}
	}
	force_inline void write_int16(int16_t i)
	{
		m_fserialize.serialize(i);
	}
	force_inline void write_int32(int32_t i)
	{
		m_fserialize.serialize(i);
	}
	force_inline void write_int64(int64_t i)
	{
		m_fserialize.serialize(i);
	}
	force_inline void write_str(const char * str, size_t size = 0)
	{
		// �Զ���ʽ
		if (size == 0)
		{
			m_fserialize.serialize((std::string)str);
		}
		// ���뷽ʽ
		else
		{
			size_t len = strlen(str);

			if (size <= len)
			{
				m_fserialize.serialize((const int8_t *)str, size);
			}
			else
			{
				m_fserialize.serialize((const int8_t *)str, len);
				write_int8(0, size - len);
			}
		}
	}
	force_inline int8_t read_int8() const
	{
		int8_t i = 0;
		m_fserialize.deserialize(i);
		return i;
	}
	force_inline int16_t read_int16() const
	{
		int16_t i = 0;
		m_fserialize.deserialize(i);
		return i;
	}
	force_inline int32_t read_int32() const
	{
		int32_t i = 0;
		m_fserialize.deserialize(i);
		return i;
	}
	force_inline int64_t read_int64() const
	{
		int64_t i = 0;
		m_fserialize.deserialize(i);
		return i;
	}
	force_inline const char * read_str(size_t size = 0) const
	{
		std::string * p = fnew<std::string>();
		std::string & i = *p;
		// �Զ���ʽ
		if (size == 0)
		{
			m_fserialize.deserialize(i);
		}
		// ���뷽ʽ
		else
		{
			i.resize(size, 0);
			m_fserialize.deserialize((int8_t *)i.c_str(), size);
			i[i.size() - 1] = 0;
		}
		m_tmp_str.push_back(p);
		return i.c_str();
	}
	force_inline size_t size() const
	{
		return m_fserialize.size();
	}	
	force_inline void write_buffer(const int8_t * p, size_t size)
	{
		m_fserialize.serialize(p, size);
	}
	force_inline void read_buffer(int8_t * p, size_t size) const
	{
		m_fserialize.deserialize(p, size);
	}
public:
	template <typename F>
	force_inline bool to_buffer(const F & f) const
	{
		const uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		// д���ͷ
		if (!f((const int8_t*)&c, c_msg_header_size))
		{
			return false;
		}

		// д��size
		size_t data_size = size();
		FASSERT(c_msg_size_size <= sizeof(size_t));
		FASSERT(data_size <= (1<<(c_msg_size_size * 8)));
		if (!f((int8_t*)&data_size, c_msg_size_size))
		{
			return false;
		}

		// д��data
		FASSERT(m_fserialize.get_read_line_size() == m_fserialize.size());
		if (!f((int8_t*)m_fserialize.get_read_line_buffer(), m_fserialize.size()))
		{
			return false;
		}
		m_fserialize.skip_read(m_fserialize.size());
		
		return true;
	}
	template <typename F>
	force_inline bool from_buffer(const F & f)
	{
		// ������ͷ
		uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		if (!f((int8_t*)&c, c_msg_header_size))
		{
			return false;
		}
		if (c != c_msg_header)
		{
			return false;
		}

		// ����size
		size_t data_size = 0;
		FASSERT(c_msg_size_size <= sizeof(size_t));
		if (!f((int8_t*)&data_size, c_msg_size_size))
		{
			return false;
		}

		// ����data
		m_fserialize.resize(data_size);
		FASSERT(m_fserialize.get_write_line_size() >= data_size);
		if (data_size > 0 && 
			!f((int8_t*)m_fserialize.get_write_line_buffer(), m_fserialize.get_write_line_size()))
		{
			return false;
		}
		m_fserialize.skip_write(data_size);

		return true;
	}
private:
	mutable _fserialize m_fserialize;
	mutable std::vector<std::string *> m_tmp_str;
};

