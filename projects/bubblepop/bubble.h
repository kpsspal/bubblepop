#ifndef BUBBLE_H
#define BUBBLE_H
class Bubble
{
public:
	float m_posx,m_posy,m_speedx,m_speedy;
	enum EnumState{eStateNone,eStateMoving};
	EnumState m_state;
	int m_cellx,m_celly;
	BYTE m_color;
	Bubble()
	{
		m_state=eStateNone;
	}
	void Update(int timeElapsed);
	void Render();
};
#endif