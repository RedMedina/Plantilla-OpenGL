#include "LoadTexture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

class Clouds
{
public:
	Clouds(const char* vertexShader, const char* fragmentShader, const char* NubesTextura)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		shaders = new Shader;
		ProgramID = shaders->LoadShaders(vertexShader, fragmentShader);

		MatrixID = glGetUniformLocation(ProgramID, "MVP");
		Offset = glGetUniformLocation(ProgramID, "textureOffset");
		ProyeccionID = glGetUniformLocation(ProgramID, "viewProjMatrix");

		textura = new LoadTexture();

		Texture = textura->LoadAnyTexture(NubesTextura);
		TextureID = glGetUniformLocation(ProgramID, "cloudTexture");

		Texture2 = textura->LoadAnyTexture("Assets/Skydome/perturb.png");
		TextureID2 = glGetUniformLocation(ProgramID, "perturbTexture");

		Model = new Load3dModel();
		Model->loadOBJ("Assets/Skydome/Sky.obj", vertices, uvs, normals);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}

	void Draw(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat4 ProjectionMatrix)
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Use our shader
		glUseProgram(ProgramID);

		// in the "MVP" uniform
		glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), vec3(1, 1, 1));
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), vec3(0, 500, 0)); // A bit to the left
		glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), vec3(1 * 200, 1, 1 * 200));
		glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

		glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glUniformMatrix4fv(ProyeccionID, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		IncrementOffset -= 0.0005f;
		glUniform2f(Offset, OffsetPos.x, OffsetPos.y + IncrementOffset);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID2, 1);

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
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}

private:
	GLuint VertexArrayID, ProgramID, MatrixID, Offset, ProyeccionID;
	Shader* shaders;
	LoadTexture* textura;
	GLuint Texture, TextureID;
	GLuint Texture2, TextureID2;
	Load3dModel* Model;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	glm::vec2 OffsetPos = glm::vec2(10.1f, 20.2f);
	float IncrementOffset = 0;
};

