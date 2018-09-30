#include "common.h"
#include <vector>
#include <cstring>

#include <GL/glew.h>

#include "shader.h"
#include "texture.h"

#include "text2D.h"
#include "logwriter.h"
#include "resourcemgr.h"

unsigned int Text2DShaderID;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#if 0
unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;

unsigned int Text2DUniformID;
void initText2D(const char * dataptr, int datasize)
{
	// Initialize texture
	Text2DTextureID = loadDDS(dataptr, datasize);

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	DWORD ressize1;
	char *resptr1;
	DWORD ressize2;
	char *resptr2;

	resptr1 = ResourceMgr::get().WorkOnResource(502, ressize1);
	if (NULL == resptr1)\
	{\
		Logwrite("loading resource failed"); \
		return; \
	}

	resptr2 = ResourceMgr::get().WorkOnResource(503, ressize2);
	if (NULL == resptr2)\
	{\
		Logwrite("loading resource failed"); \
		return; \
	}

	// Initialize Shader
	Text2DShaderID = LoadShaders(resptr1, ressize1, resptr2, ressize2);

	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation( Text2DShaderID, "myTextureSampler" );

}

void printText2D(const char * text, int x, int y, int size){

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for ( unsigned int i=0 ; i<length ; i++ ){

		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(Text2DShaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void cleanupText2D(){

	// Delete buffers
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

	// Delete shader
	glDeleteProgram(Text2DShaderID);
}

#else
GLuint VAO, VBO;
struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};
#include <map>
std::map<GLchar, Character> Characters;
float g_winsizey;
void printText2D(const char * text1, int x, int y, float size,bool bgameover)
//void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    y = g_winsizey - y;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	auto scale = size;
	//glm::vec3 color(1, 0, 0);
	glm::vec3 color(0.5, 0.8f, 0.2f);
	if(bgameover)
		color = glm::vec3(1, 0, 0);
	std::string text(text1);
	// Activate corresponding render state
	//s.Use();
	glUseProgram(Text2DShaderID);
	glUniform3f(glGetUniformLocation(Text2DShaderID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{

		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
#include "ft2build.h"
#include FT_FREETYPE_H
#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib,"freetype.lib")

void initText2D(int winsizex,int winsizey)
{
    g_winsizey= winsizey;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	DWORD ressize1;
	char *resptr1;
	DWORD ressize2;
	char *resptr2;

	resptr1 = ResourceMgr::get().WorkOnResource(502, ressize1);
	if (NULL == resptr1)\
	{\
		Logwrite("loading resource failed"); \
		return; \
	}

	resptr2 = ResourceMgr::get().WorkOnResource(503, ressize2);
	if (NULL == resptr2)\
	{\
		Logwrite("loading resource failed"); \
		return; \
	}

	// Initialize Shader
	Text2DShaderID = LoadShaders(resptr1, ressize1, resptr2, ressize2);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(winsizex), 0.0f, static_cast<GLfloat>(winsizey));
    //shader.Use();
    glUseProgram(Text2DShaderID);
    glUniformMatrix4fv(glGetUniformLocation(Text2DShaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(0);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		Logwrite("ERROR::FREETYPE: Could not init FreeType Library" );

	CHAR g_location[MAX_PATH];
	//get %appdata% folder location on user machine
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FLAG_CREATE | CSIDL_FONTS, NULL, 0, g_location)))
	{
		//getting app specific log folder
		if (g_location[strlen(g_location) - 1] == '\\')
			g_location[strlen(g_location) - 1] = 0;
		PathAppendA(g_location,"\\arial.ttf");
	}
	else
	{
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, g_location, 0, &face))
		Logwrite("ERROR::FREETYPE: Failed to load font" );

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		auto res = FT_Load_Char(face, c, FT_LOAD_RENDER);
		if (res)
		{
			Logwrite("ERROR::FREETYTPE: Failed to load Glyph" );
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
FT_Done_FreeType(ft);
}
#endif
