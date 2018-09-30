#include "common.h"
#include "cannon.h"
#include "glhelper.h"
#include "myapp.h"
#include <math.h>
bool Cannon::Init()
{
	m_posx = MyApp::get().m_settings.m_boardsizex/2;
	m_posy = MyApp::get().m_settings.m_boardsizey + CANNONPANELSIZEY - MyApp::get().GetBubbleRadius();
	m_angle = 0;
	m_Enabled=true;
	m_bubblesremaining = MyApp::get().m_settings.m_ballonseachrowdown;

	NewBubble();
	GlHelper::get().AddMouseEventListener(this);
	GlHelper::get().AddMotionEventListener(this);

	float boardsize= MyApp::get().m_settings.m_boardsizey;
	float maxanglerangle = atan2(m_posx, CANNONPANELSIZEY - MyApp::get().GetBubbleRadius());
	m_maxanglerange = MyApp::get().m_settings.m_cannonanglerangeindegrees*M_PI / 180;
	if (m_maxanglerange > maxanglerangle)
		m_maxanglerange = maxanglerangle;
	return true;
}

void Cannon::NewBubble()
{
	if(m_bubblesremaining == 0 )
	{
		MyApp::get().GetBoard().RowsDown();
		m_bubblesremaining = MyApp::get().m_settings.m_ballonseachrowdown;
	}

	--m_bubblesremaining;
	m_color= 1 + rand() % MAX_BUBBLECOLORS;
}
#define ARRAWSIZE1 (10*2*3)
#define ARRAWSIZE2 (10*2*2)
#define ARRAWANGLE1 0.2f
//float mouseposx,mouseposy;
#include <string>
#include "inttostring.h"
#include "text2D.h"

void Cannon::Render2D()
{
	//glLoadIdentity();
	//GlHelper::get().SetColor(1);

	printText2D(( INTTOSTRING(m_bubblesremaining)).c_str(), 50, m_posy, 1);
}
void Cannon::Render()
{
	//glLoadIdentity();


	///GlHelper::get().DrawTextAt(,);
	GlHelper::get().DrawSinglebubble(m_posx,m_posy,m_color,MyApp::get().GetBubbleRadius());
	//GlHelper::get().SetColor


	float y1 = m_posy;
	float x1 = m_posx;

	float y2 = y1 - ARRAWSIZE1*cosf(m_angle);
	float x2 = x1 + ARRAWSIZE1*sinf(m_angle);

	float y3 = y1 - ARRAWSIZE2*cosf(m_angle+ARRAWANGLE1);
	float x3 = x1 + ARRAWSIZE2*sinf(m_angle+ARRAWANGLE1);

	float y4 = y1 - ARRAWSIZE2*cosf(m_angle-ARRAWANGLE1);
	float x4 = x1 + ARRAWSIZE2*sinf(m_angle-ARRAWANGLE1);


	//glLineC
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0f);
	glVertex3f(x2, y2, 0.0f);

	glVertex3f(x3, y3, 0.0f);
	glVertex3f(x2, y2, 0.0f);

	glVertex3f(x4, y4, 0.0f);
	glVertex3f(x2, y2, 0.0f);

	glEnd();
}

void Cannon::MotionEventHandle(const MotionEvent &ev)
{
	float mouseposx = ev.m_x;
	float mouseposy = ev.m_y;

	float diffx = mouseposx - m_posx;
	float diffy = -mouseposy + m_posy;
	float diffsqr = diffx*diffx + diffy*diffy;
	if( diffsqr < 1 ) return ;

	m_angle = atan2(diffx,diffy);


	float anglerange = m_maxanglerange;

	if( m_angle < -anglerange )m_angle = -anglerange;
	else if( m_angle > anglerange )m_angle = anglerange;
	//UpdateBubblePos();
}

//void Cannon::UpdateBubblePos()
//{
//	int bubbleradius = MyApp::get().GetBubbleRadius();
//	m_posbubbley = m_posy - bubbleradius*cosf(m_angle);
//	m_posbubblex = m_posx + bubbleradius*sinf(m_angle);
//
//	float maxposy = (m_posy-bubbleradius);
//	if( m_posbubbley >  maxposy)m_posbubbley = maxposy;
//}
void Cannon::MouseEventHandle(const MouseEvent &ev)
{
	if (ev.m_button != GLUT_LEFT_BUTTON )return ;
	if (ev.m_state != GLUT_DOWN) return ;
	if(!m_Enabled)return ;

	//MyApp::get().GetBoard().RowsDown();
	MyApp::get().AddBubbleFromCannon();
}
