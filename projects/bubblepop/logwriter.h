#ifndef LOGWRITER_H
#define LOGWRITER_H
#include <string>
class LogWriter
{
	std::string m_logslocation;
	bool m_blogsInitialised;
	LogWriter(){m_blogsInitialised =false;}

	static LogWriter m_inst;
public:
	void Init();
	void Logwrite(const char *funcname,int lineno,const char *fmt,...);
	static LogWriter &get(){return m_inst;}
};
#define InitLogWriter(...) LogWriter::get().Init()
	
#define Logwrite(...) LogWriter::get().Logwrite(__FUNCTION__,__LINE__,__VA_ARGS__)
#endif