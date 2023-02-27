#include "Text2D.h"

class UI
{
public:
	void InitTextUI(const char* texturePath, const char* vertexShader, const char* FragmentShader)
	{
		Texto = new Text2D;
		Texto->initText2D(texturePath, vertexShader, FragmentShader);
	}

	void DrawTextUI()
	{
		char text[256];
		sprintf(text, "%.2f sec", glfwGetTime());
		Texto->printText2D(text, 10, 500, 60);
	}

	~UI()
	{
		Texto->cleanupText2D();
	}

private:
	Text2D* Texto;
	GLuint VertexArrayID;
};
