#ifndef TEXT2D_HPP
#define TEXT2D_HPP

void initText2D(int winsizex,int winsizey);
void printText2D(const char * text, int x, int y, float size,bool bgameover=false);

void cleanupText2D();

#endif
