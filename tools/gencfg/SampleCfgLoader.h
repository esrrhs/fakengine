#pragma once

class CSampleCfgLoader
{
public:
    
    // ����1
    struct STSample1Cfg
    {
        STSample1Cfg()
        {
            memset(this, 0, sizeof(*this));
        }
        
        // �ֶ�1
        int32_t m_iSetting1;
        
        // �ֶ�2
        int32_t m_iSetting2;
        
        // �ֶ�3
        int32_t m_iSetting3;
        
    };

    const STSample1Cfg & GetSample1Cfg()
    {
        return m_Sample1Cfg;
    }
    
    // ����2
    struct STSample2Cfg
    {
        STSample2Cfg()
        {
            memset(this, 0, sizeof(*this));
        }
        
        // �ֶ�a
        int32_t m_iSettinga;
        
        // �ֶ�b
        int32_t m_iSettingb;
        
        // �ֶ�c
        int32_t m_iSettingc;
        
    };

    const STSample2Cfg & GetSample2Cfg()
    {
        return m_Sample2Cfg;
    }
    
        
	bool LoadCfg(const stringc & file)
	{
		inifile<16, 16> stConfigFile;
		if (!stConfigFile.load(file))
		{
			LOG_ERROR("LoadCfg(%s) failed\n", file.c_str());
			return false;
		}

		
		// -------------Begin Load m_Sample1Cfg ����1---------------
		
		
		// �ֶ�1
		stConfigFile.get("Sample1",
								  "Setting1",
								  m_Sample1Cfg.m_iSetting1,
								  1);
		
		// �ֶ�2
		stConfigFile.get("Sample1",
								  "Setting2",
								  m_Sample1Cfg.m_iSetting2,
								  2);
		
		// �ֶ�3
		stConfigFile.get("Sample1",
								  "Setting3",
								  m_Sample1Cfg.m_iSetting3,
								  3);
		
		// -------------End Load m_Sample1Cfg ����1---------------
		
		// -------------Begin Load m_Sample2Cfg ����2---------------
		
		
		// �ֶ�a
		stConfigFile.get("Sample2",
								  "Settinga",
								  m_Sample2Cfg.m_iSettinga,
								  1);
		
		// �ֶ�b
		stConfigFile.get("Sample2",
								  "Settingb",
								  m_Sample2Cfg.m_iSettingb,
								  2);
		
		// �ֶ�c
		stConfigFile.get("Sample2",
								  "Settingc",
								  m_Sample2Cfg.m_iSettingc,
								  3);
		
		// -------------End Load m_Sample2Cfg ����2---------------
		
		
		LOG_DEBUG("after load Sample cfg get ARGS:");
		
		LOG_DEBUG("Setting1[%d], "
			"Setting2[%d], "
			"Setting3[%d], "
			"Settinga[%d], "
			"Settingb[%d], "
			"Settingc[%d], "
			, m_Sample1Cfg.m_iSetting1
			, m_Sample1Cfg.m_iSetting2
			, m_Sample1Cfg.m_iSetting3
			, m_Sample2Cfg.m_iSettinga
			, m_Sample2Cfg.m_iSettingb
			, m_Sample2Cfg.m_iSettingc
			);

		return true;
	}
    
protected:
    
    // ����1
    STSample1Cfg m_Sample1Cfg;
    
    // ����2
    STSample2Cfg m_Sample2Cfg;
    
};
