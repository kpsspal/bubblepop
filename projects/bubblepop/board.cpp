#include "common.h"
#include "board.h"
#include "myapp.h"

static BYTE *g_cells;
int g_maxcells_x,g_maxcells_y;
int g_firstrowturn;

#define INDEX_CELL(_x,_y)  ( ((_y)*g_maxcells_x) + (_x))
#define CELLX_FROMINDEX(_index)  ((_index)%g_maxcells_x)
#define CELLY_FROMINDEX(_index)  ((_index)/g_maxcells_x)
#define CELL_SIZEX (MyApp::get().GetBubbleRadius()*2)
#define CELL_SIZEY (MyApp::get().GetBubbleRadius()*2)
#define BUBBLERADIUS (MyApp::get().GetBubbleRadius())
bool Board::Init()
{
	g_firstrowturn=1;
	//(g_maxcells_x*2+1)*BUBBLERADIUS = MyApp::get().m_settings.m_boardsizex
	g_maxcells_x = ((MyApp::get().m_settings.m_boardsizex/BUBBLERADIUS)-1)/2;
	//g_maxcells_y*2*BUBBLERADIUS = MyApp::get().m_settings.m_boardsizey
	g_maxcells_y = MyApp::get().m_settings.m_boardsizey/(2*BUBBLERADIUS);


	MyApp::get().m_settings.m_boardsizex = BUBBLERADIUS*(1+g_maxcells_x*2);
	MyApp::get().m_settings.m_boardsizey = (2*BUBBLERADIUS)*g_maxcells_y ;


	g_cells = new BYTE[g_maxcells_x*g_maxcells_y];
	memset(g_cells,0,g_maxcells_x*g_maxcells_y);

	int initbubblescount = MyApp::get().m_settings.m_initbubblescount;

	for(int y=0;y<g_maxcells_y;++y)
	{
		for(int x=0;x<g_maxcells_x;++x)
		{
			g_cells[INDEX_CELL(x,y)] = 1 + (rand() % MAX_BUBBLECOLORS);
			if(--initbubblescount == 0 )
			{
				m_lastrowy = y;
				CheckGameOver();

				return true;
			}
		}
	}
	return true;
}


#include <math.h>

void Board::Render()
{
	glLoadIdentity();

	GlHelper::get().SetColor(2);
	glBegin(GL_LINES);
	glVertex3f(MyApp::get().m_settings.m_boardsizex, 0, 0.0f);
	glVertex3f(MyApp::get().m_settings.m_boardsizex,MyApp::get().m_winsizey , 0.0f);

	glVertex3f(0, MyApp::get().m_settings.m_boardsizey, 0.0f);
	glVertex3f(MyApp::get().m_winsizex, MyApp::get().m_settings.m_boardsizey, 0.0f);
    glEnd();

	for(int y=0;y<g_maxcells_y;++y)
	{
		float ypos = (y+0.5f)*CELL_SIZEY;
		float xoffset = 0;

		if(y%2 == g_firstrowturn)
		{
			xoffset  = BUBBLERADIUS;
		}
		for(int x=0;x<g_maxcells_x;++x)
		{
			auto col1 = g_cells[INDEX_CELL(x,y)];
			if(  col1  )
			{
				float xpos = (x+0.5f)*CELL_SIZEX + xoffset;
				GlHelper::get().DrawSinglebubble(xpos,ypos,col1,BUBBLERADIUS);
			}
		}
	}

	///render removed bubbles animation
	if( m_removedbubblesanim.m_color)
	{
		for(auto &a:m_removedbubblesanim.m_removedcells)
		{
			int x = CELLX_FROMINDEX(a);
			int y = CELLY_FROMINDEX(a);

			float xpos,ypos;
			GetCellCenterpos(x,y,xpos,ypos);
			GlHelper::get().DrawSinglebubbleAnim(xpos,ypos,m_removedbubblesanim.m_color,BUBBLERADIUS,m_removedbubblesanim.m_percentanim);
		}
	}

	///render new bubble animation
	if(m_newbubbleanim.m_color)
	{
		float xpos,ypos;
		GetCellCenterpos(m_newbubbleanim.m_cellx,m_newbubbleanim.m_celly,xpos,ypos);

		float angle = m_newbubbleanim.m_percentanim*M_PI*2;

		if(angle)
		{
			float disp = (sinf(angle)*BUBBLERADIUS*0.5f)/(1 + m_newbubbleanim.m_percentanim*10);
			xpos += m_newbubbleanim.m_speedx*disp;
			ypos += m_newbubbleanim.m_speedy*disp;
		}
		GlHelper::get().DrawSinglebubble(xpos,ypos,m_newbubbleanim.m_color,BUBBLERADIUS);

	}
}

void Board::GetCellCenterpos(int x,int y,float &xpos,float &ypos)
{
	ypos = (y+0.5f)*CELL_SIZEY;
	xpos = (x+0.5f)*CELL_SIZEX;
	if(y%2 == g_firstrowturn)
		xpos += BUBBLERADIUS;
}

void Board::Update(int timeelapsed)
{
	m_newbubbleanim.Update(timeelapsed);
	m_removedbubblesanim.Update(timeelapsed);
}
bool Board::GetCellsCoord(float posx,float posy,int &cellx,int &celly)
{
	celly = posy/CELL_SIZEY;

	float xoffset=0;
	if(celly%2 == g_firstrowturn)
	{
		xoffset  = BUBBLERADIUS;
	}
	cellx = (posx-xoffset)/CELL_SIZEY;

    if(cellx <0 )return false;
    else if(cellx >= g_maxcells_x )return false;
    return true;
}
bool Board::CheckCollision(float &newposx,float &newposy)
{
	int cellx,celly;
	if(false == GetCellsCoord(newposx,newposy,cellx,celly) )
        return false;

	class CallBk:public TraverseNearbyCellsCallBk
	{
		float newposx,newposy;
	public:
		virtual bool DoEachCell(int x,int y)override
		{
			auto col1 = g_cells[INDEX_CELL(x,y)];
			if(col1)
			{
				float ypos = (y+0.5f)*CELL_SIZEY;
				float diffy = ypos-newposy;
				diffy *=diffy;

				float xoffset = 0;
				if(y%2 == g_firstrowturn)
				{
					xoffset  = BUBBLERADIUS;
				}

				float xpos = (x+0.5f)*CELL_SIZEX + xoffset;
				float diffx = xpos-newposx;
				diffx *= diffx;
				float distsqr = diffx + diffy;

				if(distsqr < 4*BUBBLERADIUS*BUBBLERADIUS)return true;
			}
			return false;
		}
		CallBk(float newposx1,float newposy1)
			:newposx(newposx1)
			,newposy(newposy1)
		{
		}
	}callbk1(newposx,newposy);

	if( callbk1.DoEachCell(cellx,celly) )
		return true;
	return TraverseNearbyCells(cellx,celly,&callbk1);
}

BYTE Board::GetCellColor(int x,int y)
{
	return g_cells[ INDEX_CELL(x,y)];
}

bool Board::TraverseNearbyCells(int cellx,int celly,TraverseNearbyCellsCallBk *pcallbk)
{
#define DO_CELL(_y,_x) if( pcallbk->DoEachCell(_x,_y) )return true;
	if(celly%2 != g_firstrowturn)
	{
		if( celly < (g_maxcells_y-1))
		{
			DO_CELL( celly+1,cellx);
			if(cellx >0 )
			{
				DO_CELL( celly+1,cellx-1);
			}
		}

		if( cellx > 0 )
		{
			DO_CELL( celly,cellx-1);
		}
		if(cellx < (g_maxcells_x-1))
		{
			DO_CELL( celly,cellx+1);
		}

		if( celly > 0)
		{
			DO_CELL( celly-1,cellx);
			if(cellx >0)
			{
				DO_CELL( celly-1,cellx-1);
			}
		}
	}
	else
	{
		if( celly < (g_maxcells_y-1))
		{
			DO_CELL( celly+1,cellx);
			if(cellx < (g_maxcells_x-1) )
			{
				DO_CELL( celly+1,cellx+1);
			}
		}

		if( cellx > 0 )
		{
			DO_CELL( celly,cellx-1);
		}
		if(cellx < (g_maxcells_x-1))
		{
			DO_CELL( celly,cellx+1);
		}

		if( celly > 0)
		{
			DO_CELL( celly-1,cellx);
			if(cellx < (g_maxcells_x-1))
			{
				DO_CELL( celly-1,cellx+1);
			}
		}
	}
	return false;
}

void Board::PinBubble(int cellx,int celly,BYTE color,Bubble *pbubble)
{
	auto &col1 = g_cells[INDEX_CELL(cellx,celly)];
	_ASSERT( col1 == 0 );
	col1 = color;
	if( Check4Pairs(cellx,celly,color) )
	{
		col1 = 0;

		m_newbubbleanim.m_cellx=cellx;
		m_newbubbleanim.m_celly=celly;
		m_newbubbleanim.m_color=color;
		m_newbubbleanim.m_speedx=pbubble->m_speedx;
		m_newbubbleanim.m_speedy=pbubble->m_speedy;
		m_newbubbleanim.m_percentanim=0;

		if( celly > m_lastrowy )
		{
			m_lastrowy=celly ;
			CheckGameOver();
		}
	}
}

bool Board::CheckGameOver()
{
	if (m_lastrowy >= (g_maxcells_y - 1))
	{
		MyApp::get().GameOver();
		return true;
	}
	return false;
}

bool Board::Check4Pairs(int cellx,int celly,BYTE color)
{
	std::vector<DWORD> listcellsprocessed;
	std::vector<DWORD> resultcells;
	class CallBk:public TraverseNearbyCellsCallBk
	{
		virtual bool DoEachCell(int x,int y)override
		{
			DWORD cellindex = INDEX_CELL(x,y);
			for(auto &a:m_listcellsprocessed)
			{
				if(a == cellindex)
				{
					return false;
				}
			}
			m_listcellsprocessed.push_back(cellindex);

			auto col1 = g_cells[cellindex];
			if(col1 == m_color)
			{
				m_resultcells.push_back(cellindex);
				CallBk callbk1(m_pboard,m_listcellsprocessed,m_resultcells,m_color);
				m_pboard->TraverseNearbyCells(x,y,&callbk1);
			}
			return false;
		}
		std::vector<DWORD> &m_listcellsprocessed;
		std::vector<DWORD> &m_resultcells;
		BYTE m_color;
		Board *m_pboard;
	public:
		CallBk(Board *pboard,std::vector<DWORD> &listcellsprocessed,std::vector<DWORD> &resultcells,BYTE color)
			:m_pboard(pboard)
			,m_listcellsprocessed(listcellsprocessed)
			,m_resultcells(resultcells)
			,m_color(color)
		{
		}
	}callbk1(this,listcellsprocessed,resultcells,color);

	int indexcell1 = INDEX_CELL(cellx, celly);
	resultcells.push_back(indexcell1);
	listcellsprocessed.push_back(indexcell1);
	TraverseNearbyCells(cellx,celly,&callbk1);

	if( resultcells .size() < 3  )
	{
		MyApp::get().EnableCannon();
		return true;
	}

	RemoveCells(resultcells,color);
	return false;
}
#include "inttostring.h"
#include "logwriter.h"
void Board::RemoveCells(std::vector<DWORD> &resultcells,BYTE color)
{
	std::string cellsremoved;
	for(auto &a:resultcells)
	{
		g_cells[a]  =0;
		int x = CELLX_FROMINDEX(a);
		int y = CELLY_FROMINDEX(a);
		cellsremoved += "(" + INTTOSTRING(x)+ "," + INTTOSTRING(y) + ")";
	}
	Logwrite("Bubble Drop Event At %s",cellsremoved.c_str());

	static const char *colornames[(MAX_BUBBLECOLORS+1)*3]	=
	{
		"BLACK",//0.0f, 0.0f, 0.0f,
		"RED",//1.0f, 0.0f, 0.0f,
		"GREEN",//0.0f, 1.0f, 0.0f,
		"BLUE",//0.0f, 0.0f, 1.0f,
		"CYAN",//0.0f, 1.0f, 1.0f,
		"PINK",//1.0f, 0.0f, 1.0f,
		"YELLOW",//1.0f, 1.0f, 0.0f,
	};
	
	Logwrite("Bubbles Dropped Color=%s, Arow angle=%.2f",colornames[color],(MyApp::get().m_cannon.m_angle*180)/M_PI);

	m_removedbubblesanim.m_color=color;
	m_removedbubblesanim.m_removedcells= std::move(resultcells);
	m_removedbubblesanim.m_percentanim=0;
}

bool Board::BubbleAnim::Update(int timeelapsed)
{
	if( m_color )
	{
#define ANIMTIME 250
		m_percentanim += timeelapsed/(float)ANIMTIME;
		if(m_percentanim > 1)
		{
			m_percentanim=1;
			m_color=0;
			MyApp::get().EnableCannon();
			return true;
		}
	}
	return false;
}

bool Board::NewBallAnim::Update(int timeelapsed)
{
	BYTE color1 = m_color;
	if( BubbleAnim::Update(timeelapsed))
	{
		auto &col1 = g_cells[INDEX_CELL(m_cellx,m_celly)];
		_ASSERT( col1 == 0 );
		col1 = color1;
		return true;
	}
	return false;
}

void Board::RowsDown()
{
	if( CheckGameOver() )return ;

	g_firstrowturn = (g_firstrowturn?0:1);
	int scoreadd = MyApp::get().m_settings.m_scoreeachrowdown;
	MyApp::get().AddScore(scoreadd);
	for (int y = m_lastrowy; y >= 0; --y)
	{
		for (int x = 0; x < g_maxcells_x; ++x)
		{
			g_cells[INDEX_CELL(x, y + 1)] = g_cells[INDEX_CELL(x, y )];
		}
	}
	++m_lastrowy;
	if( CheckGameOver() )return ;

	for (int x = 0; x < g_maxcells_x; ++x)
	{
		g_cells[INDEX_CELL(x, 0)] = 1 + (rand() % MAX_BUBBLECOLORS);
	}
}

bool Board::RemovedBubblesAnim::Update(int timeelapsed)
{
    if( BubbleAnim::Update(timeelapsed))
    {
		int scoreadd = MyApp::get().m_settings.m_scoreeachbubble*m_removedcells.size();
		MyApp::get().AddScore(scoreadd);
        m_removedcells.clear();

        return true;
    }
    return false;
}
