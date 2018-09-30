#include "common.h"
//#pragma comment(lib,"Release\\Win32\\glew32.lib")
//#pragma comment(lib,"freeglut.lib")
#include <vector>
#include "logwriter.h"
#include "glhelper.h"
GlHelper GlHelper::m_inst;


void RenderSceneCB();
void reshape(GLsizei width, GLsizei height) ;
void Timer();

void GlHelper::HandleMouseEvent(const MouseEvent &ev)
{
	for(auto &a:m_listmouselisteners)
	{
		a->MouseEventHandle(ev);
	}
}
void GlHelper::HandleMotionEvent(const MotionEvent &ev)
{
	for(auto &a:m_listmotionlisteners)
	{
		a->MotionEventHandle(ev);
	}
}
void mouse(int button,int state,int x, int y)
{
	GlHelper::get().HandleMouseEvent(MouseEvent(button,state,x, y));
}
void passivemotion(int x, int y)
{
	GlHelper::get().HandleMotionEvent(MotionEvent(x, y));
}
void IdleFunc();

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
#include "text2D.h"

void resize(int width, int height);
bool GlHelper::Init(int argc, char **argv,int winsizex,int winsizey)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(winsizex, winsizey);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sample");

	long dwStyle;
	HWND hwndGlut;
	hwndGlut = FindWindow(NULL, "Sample");
	dwStyle = GetWindowLong(hwndGlut, GWL_STYLE);
	dwStyle ^= WS_MAXIMIZEBOX;
	dwStyle ^= WS_SIZEBOX;
	SetWindowLong(hwndGlut, GWL_STYLE, dwStyle);

	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(resize);
	//glutTimerFunc(1000/60, Timer, 0);
	glutIdleFunc(IdleFunc);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passivemotion);
	{
	GLenum out = glewInit();
	if (GLEW_OK != out)
	{
		Logwrite("glewInit failed:%d",out);
		return false;
	}
	}

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0,winsizex ,winsizey,0);
	//gluOrtho2D(0,winsizex ,0,winsizey);

	glLineWidth(2);

// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
   //glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();

       glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}
	static GLfloat colors[(MAX_BUBBLECOLORS+1)*3]	=
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
	};
void GlHelper::SetColor(BYTE color)
{
	glColor3f(colors[color*3], colors[color*3+1], colors[color*3+2]);
}
#include <math.h>
void GlHelper::DrawSinglebubble(float xpos,float ypos,BYTE color,float bubbleradius)
{
	glColor3f(colors[color*3], colors[color*3+1], colors[color*3+2]);
	glBegin( GL_TRIANGLE_FAN );
        glVertex2f(xpos, ypos);
        for( int n = 0; n <= 20; ++n ) {
            float const t = 2 * M_PI * (float)n / (float)20;
			glVertex2f(xpos + sinf(t) * bubbleradius, ypos + cosf(t) * bubbleradius);
        }
    glEnd();
}

void GlHelper::DrawSinglebubbleAnim(float xpos,float ypos,BYTE color,float bubbleradius,float percent)
{
	glColor3f(colors[color*3], colors[color*3+1], colors[color*3+2]);
	glBegin( GL_TRIANGLE_FAN );
        glVertex2f(xpos, ypos);
		float rad = bubbleradius*(1+percent*0.2f);
        for( int n = 0; n <= 20; ++n )
		{
            float const t = 2 * M_PI * (float)n / (float)20;
			glVertex2f(xpos + sinf(t) * rad, ypos + cosf(t) * rad);
        }
    glEnd();
}
void GlHelper:: DrawTextAt(float x,float y,const char *text)
{
}





