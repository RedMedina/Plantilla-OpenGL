#include "Load3dmodel.h"
#include "Shader.h"
#include "LoadTexture.h"

class Modelo3D
{
public:
	Modelo3D(const char* vertexShader, const char* fragmentShader, const char* modelpath, const char* texturePath)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		shaders = new Shader;
		programID = shaders->LoadShaders(vertexShader, fragmentShader);
		MatrixID = glGetUniformLocation(programID, "MVP");
		texture = new LoadTexture();
		Texture = texture->LoadDDS(texturePath);
		TextureID = glGetUniformLocation(programID, "myTextureSampler");
		modelLoad = new Load3dModel();
		bool res = modelLoad->loadOBJ(modelpath, vertices, uvs, normals);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}

	void Draw(glm::mat4 MVP)
	{
		// Use our shader
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	~Modelo3D()
	{
		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteProgram(programID);
		glDeleteTextures(1, &TextureID);
		glDeleteVertexArrays(1, &VertexArrayID);
	}

private:
	GLuint programID;
	GLuint MatrixID;
	GLuint Texture;
	GLuint TextureID;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	Shader* shaders;
	LoadTexture* texture;
	Load3dModel* modelLoad;
	GLuint VertexArrayID;
};