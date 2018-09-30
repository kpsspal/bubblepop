#ifndef MYAPP_H
#define MYAPP_H
#include "glhelper.h"
#include "board.h"
#include "cannon.h"
#include "bubble.h"
#include <stdio.h>
#include "scorepanel.h"
#include <string>
#include "settings.h"


class MyApp
{
	ScorePanel m_scorepanel;
	Bubble m_bubble;
	Board m_board;
	Cannon m_cannon;
	int m_prevtick;
	bool m_gameover;
	//GLuint vertexbuffer;
	//GLuint programID;
	bool Init();
	MyApp(){}
	Bubble *GetAvailableBubble();
public:
	int m_score;
	int m_winsizex,m_winsizey;

	Settings m_settings;
	inline int GetBubbleRadius(){return m_settings.m_bubbleradius;}
	Board &GetBoard(){return m_board;}
	void AddBubbleFromCannon();
	void Run(HINSTANCE hInstance,int       nCmdShow);
	void Render();
	void Update();
	static MyApp g_App;
	static MyApp &get(){return g_App;}
	bool CheckCollision(float &newposx,float &newposy,Bubble *pbubble);
	void GameOver();
	void EnableCannon(bool bEnable=true);
	void AddScore(int scoreadd);
};
#endif
