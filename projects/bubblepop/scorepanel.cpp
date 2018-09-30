#include "common.h"
#include "scorepanel.h"

#include "text2d.h"
#include "myapp.h"
#include "inttostring.h"

void ScorePanel::Render()
{
	int score = MyApp::get().m_score;
	int x = MyApp::get().m_settings.m_boardsizex + 10;
	printText2D("Score", x, 30, 0.5f);
	printText2D((INTTOSTRING(score)).c_str(), x, 60, 0.5f);
}
void ScorePanel::Update(int timeElapsed)
{
}
