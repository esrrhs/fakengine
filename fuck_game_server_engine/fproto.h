#pragma once

class fproto
{
public:
    force_inline fproto()
	{
		clear();
	}

	force_inline ~fproto()
	{

	}

	force_inline void clear()
	{
	    m_lasterr.clear();
	}

	force_inline bool parse(stringc file)
	{
	    clear();
	    SAFE_TEST_RET_VAL(parse_xml(file), false, false);
	    SAFE_TEST_RET_VAL(parse_proto(), false, false);
	    SAFE_TEST_RET_VAL(check_proto(), false, false);
	    return true;
	}

    force_inline stringc err() const
    {   
        return m_lasterr;
    }

private:
    force_inline bool parse_xml(stringc file)
	{
	    
	    return true;
	}
    force_inline bool parse_proto()
	{
	    
	    return true;
	}
    force_inline bool check_proto()
	{
	    
	    return true;
	}

private:
	stringc m_lasterr;
	
};

