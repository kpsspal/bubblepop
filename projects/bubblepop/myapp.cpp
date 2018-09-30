#include "common.h"
#include "myapp.h"
#include "glhelper.h"
#include "resourcemgr.h"
#include "logwriter.h"

MyApp MyApp::g_App;

void RenderSceneCB()
{
	MyApp::get().Render();
}
#include "text2D.h"
void MyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
	glLoadIdentity();

	m_board.Render();
	m_cannon.Render();
	m_bubble.Render();
	m_scorepanel.Render();

	m_cannon.Render2D();
	//printText2D("This is sample text", 25.0f, 25.0f, 1.0f);
    //RenderText(shader, );

	if(m_gameover)
	{
	    printText2D("Game Over",m_settings.m_boardsizex/2,m_settings.m_boardsizey/2,1,true);
	}

#if 0
	glUseProgram(programID);
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
#endif
	glutSwapBuffers();
}

#include "text2D.h"
bool MyApp::Init()
{
    m_gameover=false;
	m_score=0;
	//logwriter initialisation
	InitLogWriter();

	m_settings.LoadSettings();
	////initialisation glut and glew with specified window size
	m_winsizex = MAX(CANNONPANELSIZEX,m_settings.m_boardsizex) + SCOREPANEL_SIZEX;
	m_winsizey  = MAX(m_settings.m_boardsizey+CANNONPANELSIZEY,SCOREPANEL_SIZEY);
	if(false == GlHelper::get().Init(0, NULL,m_winsizex,m_winsizey))
	{
		Logwrite("GlHelper init failed");
		return false;
	}

	{
		DWORD ressize;
		char *resptr;
#define LOAD_RES(__resid)\
		resptr = ResourceMgr::get().WorkOnResource(__resid, ressize);\
		if (NULL == resptr)\
		{\
			Logwrite("loading resource failed");\
			return false;\
		}

		LOAD_RES(501);
		initText2D(resptr, ressize,m_winsizex,m_winsizey);
	}
	//loading shaders
#if 0
	{
		DWORD vertexshaderresid = 501;
		DWORD fragmentshaderresid = 502;

		DWORD vertexshaderfileisize;
		char *vertexshaderptr = ResourceMgr::get().WorkOnResource(vertexshaderresid,vertexshaderfileisize);
		if( NULL == vertexshaderptr )
		{
			Logwrite("vertexshader resource loading failed");
			return false;
		}
		DWORD fragmentshaderfileisize;
		char *fragmentshaderptr = ResourceMgr::get().WorkOnResource(fragmentshaderresid,fragmentshaderfileisize);
		if( NULL == fragmentshaderptr )
		{
			Logwrite("fragmentshader resource loading failed");
			return false;
		}

		programID =  GlHelper::get().LoadShaders(vertexshaderptr,vertexshaderfileisize,fragmentshaderptr,fragmentshaderfileisize);
	}
#endif

	if(false == m_board.Init() )
	{
		Logwrite("Board init failed");
		return false;
	}

	if(false == m_cannon.Init() )
	{
		Logwrite("Cannon init failed");
		return false;
	}

#if 0
	{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};

	// This will identify our vertex buffer

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	}
#endif
	m_prevtick = glutGet(GLUT_ELAPSED_TIME);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	return true;
}


void MyApp::Run(HINSTANCE hInstance,int       nCmdShow)
{
	if(false == Init() )return ;

	glutMainLoop();
}

void IdleFunc()
{
	MyApp::get().Update();
}

Bubble *MyApp::GetAvailableBubble()
{
	{
		if(m_bubble.m_state == Bubble::eStateNone)
		{
			return &m_bubble;
		}
	}
	return NULL;
}

void MyApp::GameOver()
{
    m_gameover = true;
}
#include <math.h>
void MyApp::AddBubbleFromCannon()
{
#if 1
	Bubble *pbubble = GetAvailableBubble();
	if( pbubble == NULL )return ;

	float yspeed = -cosf(m_cannon.m_angle);
	float xspeed = sinf(m_cannon.m_angle);

	pbubble->m_posx = m_cannon.m_posx;
	pbubble->m_posy = m_cannon.m_posy;
	//if( m_board.CheckCollision(pbubble->m_posx,pbubble->m_posy) )
	//{
	//	GameOver();
	//	return ;
	//}
	pbubble->m_state = Bubble::eStateMoving;
	pbubble->m_speedx = xspeed;
	pbubble->m_speedy = yspeed;
	pbubble->m_color = m_cannon.m_color;

	m_cannon.NewBubble();
	pbubble->m_cellx = -1;
	///m_board.GetCellsCoord(pbubble->m_posx,pbubble->m_posy,pbubble->m_cellx,pbubble->m_celly);
	EnableCannon(false);
#endif
}

#define FPS_REQUIRED 50
void MyApp::Update()
{
	int newtick = glutGet(GLUT_ELAPSED_TIME);
	int timeelapsed = newtick - m_prevtick;
	float mintime=(1000/FPS_REQUIRED);

    if(!m_gameover)
    {
		m_prevtick = newtick;
		{
			auto runtimeelapsed=timeelapsed;
			for(;;)
			{
				if(runtimeelapsed < mintime)
				{
					m_bubble.Update(runtimeelapsed);
					break;
				}
				m_bubble.Update(mintime);
				runtimeelapsed-=mintime;
			}
		}
		m_board.Update(timeelapsed);
		//m_cannon.Update();
	}
	m_scorepanel.Update(timeelapsed);
	glutPostRedisplay();
}

bool MyApp::CheckCollision(float &newposx,float &newposy,Bubble *pbubble)
{
	/*check collision with walls and board */
	return m_board.CheckCollision(newposx,newposy);
}
void MyApp::EnableCannon(bool bEnable)
{
	m_cannon.Enable(bEnable);
}

void MyApp::AddScore(int scoreadd)
{
	m_score +=scoreadd;
}

void resize(int width, int height)
{
	glutReshapeWindow(MyApp::get().m_winsizex, MyApp::get().m_winsizey);
}
