#include "common.h"
#include "settings.h"

const char *FirstWhiteSpace(const char *string)
{
	for (; string[0] ; ++string)
	{
		if (string[0] <= 32)return string;
	}
	return NULL;
}

const char *FirstNonWhiteSpace(const char *string)
{
	for (; string[0]; ++string)
	{
		if (string[0] > 32)return string;
	}
	return NULL;
}
#include <algorithm>
#include <cctype>
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Settings::LoadSettings(FILE *fp)
{
#define MAX_LINECHARS 1024
	static char linebuf[MAX_LINECHARS];
	std::string currentservicename;
	for(;;)
	{
		if( NULL == fgets(linebuf,MAX_LINECHARS,fp) )break;
		const char *tokenstart = linebuf;
		const char *firstnonwhite = FirstNonWhiteSpace(tokenstart);
		if(NULL==firstnonwhite)continue;

		const char *eqpos = strchr(firstnonwhite,'=');
		if(NULL == eqpos )continue;

		int nochars = (eqpos  - firstnonwhite);
		if(nochars<=0)continue;

		std::string propname(firstnonwhite ,nochars );
		rtrim(propname);

		const char *startvalue = FirstNonWhiteSpace(eqpos +1);
		const char *lastvalue = FirstWhiteSpace(startvalue);
		if(NULL == lastvalue)continue;

		nochars = lastvalue - startvalue;
		if(nochars<=0)continue;

		std::string valuename (startvalue,nochars);
		ProcessSingleSetting(propname,valuename);
	}
}

void Settings::ProcessSingleSetting(const std::string &propname,const std::string &valuename)
{
#define ASSIGN_PROP_INT(__propname)\
	if(propname == STRINGIZE(__propname))\
	{\
		m_##__propname = atoi(valuename.c_str() );\
		return ;\
	}

	ASSIGN_PROP_INT(boardsizex);
	ASSIGN_PROP_INT(boardsizey);
	ASSIGN_PROP_INT(bubbleradius);
	ASSIGN_PROP_INT(initbubblescount);
	ASSIGN_PROP_INT(cannonanglerangeindegrees);

	ASSIGN_PROP_INT(ballonseachrowdown);
	ASSIGN_PROP_INT(scoreeachrowdown);
	ASSIGN_PROP_INT(scoreeachbubble);




}
void Settings::LoadSettings()
{
	FILE *fp =fopen("settings.txt","rt");
	if(fp)
	{
		LoadSettings(fp);
		fclose(fp);
	}
}
#include "inttostring.h"
bool Settings::SaveSettings()
{
	FILE *fp = fopen("settings.txt","wt");
	if(NULL == fp)
		return false;

#define ASSIGN_PROP_INT(__propname)	fprintf(fp,STRINGIZE(__propname)"=%s\r\n",INTTOSTRING( m_##__propname).c_str())


	ASSIGN_PROP_INT(boardsizex);
	ASSIGN_PROP_INT(boardsizey);
	ASSIGN_PROP_INT(bubbleradius);
	ASSIGN_PROP_INT(initbubblescount);
	ASSIGN_PROP_INT(cannonanglerangeindegrees);

	ASSIGN_PROP_INT(ballonseachrowdown);
	ASSIGN_PROP_INT(scoreeachrowdown);
	ASSIGN_PROP_INT(scoreeachbubble);




	fclose(fp);
	return true;
}
