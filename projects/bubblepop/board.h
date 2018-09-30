#ifndef BOARD_H
#define BOARD_H
class TraverseNearbyCellsCallBk
{
public:
	virtual bool DoEachCell(int cellx,int celly)=0;
};
#include <vector>
class Bubble;
class Board
{
	bool CheckGameOver();
	void GetCellCenterpos(int x,int y,float &xpos,float &ypos);
	void RemoveCells(std::vector<DWORD> &resultcells,BYTE color);
	bool Check4Pairs(int cellx,int celly,BYTE color);

	struct BubbleAnim
	{
		float m_percentanim;
		BYTE m_color;
		BubbleAnim(){m_color=0;}

		virtual bool Update(int timeelapsed);
	};
	struct NewBallAnim:public BubbleAnim
	{
		int m_cellx,m_celly;
		float m_speedx,m_speedy;
		virtual bool Update(int timeelapsed)override;
	}m_newbubbleanim;
	struct RemovedBubblesAnim:public BubbleAnim
	{
		std::vector<DWORD> m_removedcells;
		virtual bool Update(int timeelapsed)override;
	}m_removedbubblesanim;
	int m_lastrowy;
public:
	bool TraverseNearbyCells(int cellx,int celly,TraverseNearbyCellsCallBk *pcallbk);
	void Update(int timeelapsed);
	BYTE GetCellColor(int x,int y);
	void PinBubble(int cellx,int celly,BYTE m_color,Bubble *pbubble);
	bool GetCellsCoord(float posx,float posy,int &cellx,int &celly);
	bool CheckCollision(float &newposx,float &newposy);
	bool Init();
	void Render();
	void RowsDown();
};
#endif