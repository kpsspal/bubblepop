#ifndef GLHELPER_H
#define GLHELPER_H
#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL/glut.h>

class MouseEvent
{
public:
	int m_button,m_state,m_x,m_y;
	MouseEvent(int button,int state,int x, int y)
		:m_button(button)
		,m_state(state)
		,m_x(x)
		,m_y(y)
	{
	}
};

class MotionEvent
{
public:
	int m_x,m_y;
	MotionEvent(int x, int y)
		:m_x(x)
		,m_y(y)
	{
	}
};


class MouseEventListener
{
public:
	virtual void MouseEventHandle(const MouseEvent &e)=0;
};
class MotionEventListener
{
public:
	virtual void MotionEventHandle(const MotionEvent &e)=0;
};
#include <vector>
class GlHelper
{
	static GlHelper m_inst;
	GlHelper(){}
	std::vector< MouseEventListener *> m_listmouselisteners;
	std::vector< MotionEventListener *> m_listmotionlisteners;
public:
	void DrawTextAt(float x,float y,const char *text);
	void SetColor(BYTE color);
	void DrawSinglebubbleAnim(float xpos,float ypos,BYTE color,float bubbleradius,float percent);
	void DrawSinglebubble(float xpos,float ypos,BYTE color,float bubbleradius);
	void HandleMouseEvent(const MouseEvent &ev);
	void HandleMotionEvent(const MotionEvent &ev);
	void AddMouseEventListener(MouseEventListener *p){m_listmouselisteners.push_back(p);}
	void AddMotionEventListener(MotionEventListener *p){m_listmotionlisteners.push_back(p);}
	static GlHelper &get(){return m_inst;}
	bool Init(int argc, char **argv,int winsizex,int winsizey);
};
#endif