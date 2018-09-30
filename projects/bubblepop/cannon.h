#ifndef CANNON_H
#define CANNON_H
#include "glhelper.h"
#define CANNONPANELSIZEY 50 
#define CANNONPANELSIZEX 100 
class Cannon:public MouseEventListener,public MotionEventListener
{
	virtual void MotionEventHandle(const MotionEvent &e)override;
	virtual void MouseEventHandle(const MouseEvent &e)override;
	///void UpdateBubblePos();
	int m_bubblesremaining;
	float m_maxanglerange;
public:
	void Enable(bool bEnable){m_Enabled=bEnable;}
	void NewBubble();
	float m_posx,m_posy;////,m_posbubblex,m_posbubbley;
	float m_angle;
	BYTE m_color;
	bool m_Enabled;
	bool Init();
	void Render();
	void Render2D();
};
#endif