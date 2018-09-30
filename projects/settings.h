#ifndef SETTINGS_H
#define SETTINGS_H
#include <string>
#include <stdio.h>
struct Settings
{
	int m_boardsizex;
	int m_boardsizey;
	int m_bubbleradius;
	int m_initbubblescount;
	int m_cannonanglerangeindegrees;
	int m_ballonseachrowdown;
	int m_scoreeachrowdown;
	int m_scoreeachbubble;

	Settings()
	{
		m_boardsizex = 320;
		m_boardsizey = 480;
		m_bubbleradius = 10;
		m_initbubblescount=144;
		m_cannonanglerangeindegrees=90;
		m_ballonseachrowdown=6;
		m_scoreeachrowdown=5;
		m_scoreeachbubble=1;
	}
	void LoadSettings();
	void LoadSettings(FILE *fp);
	void ProcessSingleSetting(const std::string &propname,const std::string &valuename);
	bool SaveSettings();
};
#endif