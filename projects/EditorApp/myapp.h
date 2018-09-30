#ifndef MYAPP_H
#define MYAPP_H
#include "settings.h"
class MyApp
{
	static MyApp m_inst;
public:
	Settings m_settings;
	void Run(HINSTANCE hInstance,int       nCmdShow);
	static MyApp &get(){return m_inst;}
	void StartGame();
};
#endif