#include "common.h"
#include <stdio.h>
#include "logwriter.h"
#include <string>
#include <time.h>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#pragma comment(lib,"Shlwapi.lib")

LogWriter LogWriter::m_inst;
void LogWriter::Init()
{
	CHAR g_loglocation[MAX_PATH];
	//get %appdata% folder location on user machine
	if(SUCCEEDED(SHGetFolderPathA(NULL,CSIDL_FLAG_CREATE|CSIDL_APPDATA,NULL, 0, g_loglocation)))
	{
		//getting app specific log folder
		if(g_loglocation [strlen(g_loglocation)-1] == '\\')
			g_loglocation [strlen(g_loglocation)-1]=0;
		PathAppendA(g_loglocation, (std::string("\\") + STRINGIZE(APPNAME)).c_str());

#ifdef _MSC_VER
		///trying to create logs folder ,if not created already

		struct stat info;

		if( stat( g_loglocation, &info ) != 0 )
		{
			CreateDirectoryA(g_loglocation,NULL);
		}

		if( stat( g_loglocation, &info ) != 0 )
		{
			//cannot access
		}
		else if( info.st_mode & S_IFDIR )
		{
			m_logslocation = g_loglocation;
		}
		else
		{
			// is not a directory
		}
	#else
	CreateDirectoryA(g_loglocation,NULL);
	m_logslocation = g_loglocation;
#endif
	}

	///if logs folder location cant be determined or not created
	//use current directory for saving log file
	if(m_logslocation.empty())
	{
		DWORD res1 = GetCurrentDirectoryA(MAX_PATH,g_loglocation);
		if( ( res1 != 0 ) && (res1 <=MAX_PATH ) )
		{
			m_logslocation = g_loglocation;
		}
		else
		{
			return ;
		}
	}

	{
		std::string::iterator it = m_logslocation.end() - 1;
		if (*it == '\"')
		{
			m_logslocation.erase(it);
		}
	}
	{
		std::string::iterator it = m_logslocation.begin();
		if (*it == '\"')
		{
			m_logslocation.erase(it);
		}
	}
	{
		std::string::iterator it = m_logslocation.end() - 1;
		if(  (*it != '/') && (*it != '\\') )
		{
			m_logslocation.append("\\");
		}
	}


	//at this point , log folder created and location saved in m_logslocation
	//now need to decide log filename

	char timestr[256];
	time_t t = time(0);
	struct tm * now = localtime( & t );
	sprintf(timestr,"%04d-%02d-%02d_" STRINGIZE(APPNAME) ".log",(now->tm_year + 1900) ,(now->tm_mon + 1),now->tm_mday);
	m_logslocation += timestr;

	///log writer intialised success
	m_blogsInitialised = true;
}
#undef Logwrite
void LogWriter::Logwrite(const char *funcname,int lineno,const char *fmt,...)
{
	char timestr[256];
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		sprintf(timestr,"%02d:%02d.%02d'%03d,%d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,GetTickCount());
	}

	va_list args;
	va_start (args, fmt);

	FILE *fp = fopen(m_logslocation.c_str(),"at");
	if(fp )
	{
#define FORMAT_LOGLINE "%s\t%05d\t%s\t\t\t"
		fprintf(fp,FORMAT_LOGLINE,timestr,lineno,funcname);
		vfprintf (fp, fmt, args);
		fprintf(fp,"\r\n");
		fclose(fp);
	}

	va_end (args);
}
