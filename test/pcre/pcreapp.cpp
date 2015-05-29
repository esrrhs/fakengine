#include "fakengine.h"
#include "pcreapp.h"

#define OVECCOUNT 30 /* should be a multiple of 3 */  
#define EBUFLEN 128  
#define BUFLEN 1024  

bool pcreapp::ini( int argc, char *argv[] )
{
	return true;
}

bool pcreapp::heartbeat()
{
    pcre  * re;  
    const char *error;  
    int  erroffset;  
    int  ovector[OVECCOUNT];  
    int  rc, i;  
    char  src [] = "111 <title>Hello World</title> 222";   // Ҫ������ƥ����ַ���  
    char  pattern [] = "<title>(.*)</(tit)le>";              // ��Ҫ��������ַ�����ʽ��������ʽ  
    LOG_DEBUG("String : %s \n", src);  
    LOG_DEBUG("Pattern: %s \n", pattern);  
    
    re = pcre_compile(pattern,       // pattern, �����������Ҫ��������ַ�����ʽ��������ʽ  
                      0,            // options, �������������ָ������ʱ��һЩѡ��  
                      &error,       // errptr, ����������������������Ϣ  
                      &erroffset,   // erroffset, ���������pattern�г���λ�õ�ƫ����  
                      NULL);        // tableptr, �������������ָ���ַ���һ�������NULL  
                      
    // ����ֵ��������õ�������ʽ��pcre�ڲ���ʾ�ṹ  
    if (re == NULL) 
    {                 
        //�������ʧ�ܣ����ش�����Ϣ  
        LOG_DEBUG("PCRE compilation failed at offset %d: %s \n", erroffset, error);  
        return false;  
    }  
    
    rc = pcre_exec(re,            // code, �����������pcre_compile����õ�������ṹ��ָ��  
                   NULL,          // extra, ���������������pcre_exec��һЩ�����������Ϣ�Ľṹ��ָ��  
                   src,           // subject, ���������Ҫ������ƥ����ַ���  
                   strlen(src),  // length, ��������� Ҫ������ƥ����ַ�����ָ��  
                   0,             // startoffset, �������������ָ��subject��ʲôλ�ÿ�ʼ��ƥ���ƫ����  
                   0,             // options, ��������� ����ָ��ƥ������е�һЩѡ��  
                   ovector,       // ovector, �����������������ƥ��λ��ƫ����������  
                   OVECCOUNT);    // ovecsize, ��������� ��������ƥ��λ��ƫ���������������С  
                   
    // ����ֵ��ƥ��ɹ����طǸ�����û��ƥ�䷵�ظ���  
    if (rc < 0) 
    {                     
        //���û��ƥ�䣬���ش�����Ϣ  
        if (rc == PCRE_ERROR_NOMATCH) 
            LOG_DEBUG("Sorry, no match ... \n");  
        else 
            LOG_DEBUG("Matching error %d \n", rc);  
        pcre_free(re);  
        return false;  
    }  
    
    LOG_DEBUG("\n OK, has matched ... \n\n");   //û�г����Ѿ�ƥ��  
    
    for (i = 0; i < rc; i++) 
    {             
        //�ֱ�ȡ��������� $0��������ʽ $1��һ��()  
        char *substring_start = src + ovector[2*i];  
        int substring_length = ovector[2*i+1] - ovector[2*i];  
        LOG_DEBUG("$%2d: %.*s \n", i, substring_length, substring_start);  
    }  
    
    pcre_free(re);                     // ����������ʽre �ͷ��ڴ�  
    
	return true;
}

bool pcreapp::exit()
{
	return true;
}

pcreapp::pcreapp() : mainapp("pcreapp")
{

}
