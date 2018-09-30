#include "common.h"
#include "bubble.h"
#include "glhelper.h"
#include "myapp.h"

void Bubble::Update(int timeElapsed)
{
	if(m_state == eStateNone)return ;
#define SPEED (0.5f)

	float newposx = m_posx + m_speedx *SPEED*timeElapsed;
	float newposy = m_posy + m_speedy *SPEED*timeElapsed;

	if (newposx < 0)
	{
		m_speedx *= -1;
		return ;
	}
	else if (newposx > MyApp::get(). m_settings.m_boardsizex)
	{
		m_speedx *= -1;
		return ;
	}
	if (newposy >= MyApp::get().m_settings.m_boardsizey)
	{
		m_posx = newposx;
		m_posy = newposy;
		return;
	}

	bool bcollision =false;
	int newcellx,newcelly;
	if( MyApp::get().CheckCollision(newposx,newposy,this) )
	{
		bcollision =true;
	}
	else
	{
		if( false == MyApp::get().GetBoard().GetCellsCoord(newposx,newposy,newcellx,newcelly) )
		{
			bcollision =true;
		}
		else
		{
			bcollision =false;
		}
	}

	if(bcollision)
	{
		if (m_cellx == -1)
		{
			MyApp::get().GameOver();
		}
		else
			MyApp::get().GetBoard().PinBubble(m_cellx,m_celly,m_color,this);
		m_state = eStateNone;
	}
	else
	{
		m_posx = newposx;
		m_posy = newposy;
		
		m_cellx=newcellx;
		m_celly=newcelly;
		BYTE col1 = MyApp::get().GetBoard().GetCellColor(m_cellx,m_celly);
		_ASSERT(col1 == 0 );
		if(m_posy < -MyApp::get().GetBubbleRadius() )
		{
			m_state = eStateNone;
		}
	}
}
void Bubble::Render()
{
	if(m_state == eStateMoving)
		GlHelper::get().DrawSinglebubble(m_posx,m_posy,m_color,MyApp::get().GetBubbleRadius());
}
