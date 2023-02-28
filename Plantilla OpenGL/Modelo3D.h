#ifndef __Modelo3D
#define __Modelo3D
#include "quaternion.h"
#include "Load3dmodel.h"
#include "Shader.h"
#include "LoadTexture.h"
#include "tangentspace.h"
#include "vboindexer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

class Modelo3D
{
public:
	Modelo3D(const char* vertexShader, const char* fragmentShader, const char* modelpath, const char* difuse, const char* specular, const char* normal)
	{
		quaternion = new Quaternion();
		vboIndex = new vboindexer();
		tangent = new tangentspace();
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		shaders = new Shader;
		programID = shaders->LoadShaders(vertexShader, fragmentShader);

		// Get a handle for our "MVP" uniform
		MatrixID = glGetUniformLocation(programID, "MVP");
		ViewMatrixID = glGetUniformLocation(programID, "V");
		ModelMatrixID = glGetUniformLocation(programID, "M");
		ModelView3x3MatrixID = glGetUniformLocation(programID, "MV3x3");

		TiempoID = glGetUniformLocation(programID, "time");

		texture = new LoadTexture();
		//Texture = texture->LoadDDS(texturePath);
		//TextureID = glGetUniformLocation(programID, "myTextureSampler");

		// Load the texture
		 DiffuseTexture = texture->LoadAnyTexture(difuse);
		 NormalTexture = texture->LoadAnyTexture(normal);
		 SpecularTexture = texture->LoadAnyTexture(specular);

		 // Get a handle for our "myTextureSampler" uniform
		  DiffuseTextureID = glGetUniformLocation(programID, "DiffuseTextureSampler");
		  NormalTextureID = glGetUniformLocation(programID, "NormalTextureSampler");
		  SpecularTextureID = glGetUniformLocation(programID, "SpecularTextureSampler");


		modelLoad = new Load3dModel();
		bool res = modelLoad->loadOBJ(modelpath, vertices, uvs, normals);

		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		tangent->computeTangentBasis(
			vertices, uvs, normals, // input
			tangents, bitangents    // output
		);
		
		vboIndex->indexVBO_TBN(
			vertices, uvs, normals, tangents, bitangents,
			indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
		);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

		
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

		
		glGenBuffers(1, &tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

		
		glGenBuffers(1, &bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

		// Generate a buffer for the indices as well
		
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	
		// Get a handle for our "LightPosition" uniform
		glUseProgram(programID);
		LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	}

	void Draw(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 ProjectionMatrix, glm::vec3 position, glm::vec3 scala, float rotation, float DayTransicionDuration)
	{
		// Use our shader
		glUseProgram(programID);

		glm::vec3 lightPos = glm::vec3(0, 0, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Bind our diffuse texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
		// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
		glUniform1i(DiffuseTextureID, 0);
		

		// Bind our normal texture in Texture Unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NormalTexture);
		// Set our "Normal	TextureSampler" sampler to user Texture Unit 0
		glUniform1i(NormalTextureID, 1);
		
		// Bind our normal texture in Texture Unit 2
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, SpecularTexture);
		// Set our "Normal	TextureSampler" sampler to user Texture Unit 0
		glUniform1i(SpecularTextureID, 2);
		glDisable(GL_BLEND);
		//Lights
		if (SkyB) {
			if (Sky > 0.8f && Sky <= 1)
			{
				Sky += DayTransicionDuration * 3.0f;
			}
			else if(Sky > 0.2 && Sky < 0.8)
			{
				Sky += DayTransicionDuration * 5.0f;
			}
			else if (Sky < 0.2 && Sky >= 0)
			{
				Sky += DayTransicionDuration;
			}
		}
		else {
			if (Sky > 0.8f && Sky <= 1)
			{
				Sky -= DayTransicionDuration * 3.0f;
			}
			else if(Sky>0.2 && Sky < 0.8)
			{
				Sky -= DayTransicionDuration * 5.0f;
			}
			else if(Sky<0.2 && Sky >= 0)
			{
				Sky -= DayTransicionDuration;
			}
		}

		if (Sky < 0.0) {
			Sky = 0.0f;
			SkyB = true;
		}
		else if (Sky > 1.0f) {

			Sky = 1.0f;
			SkyB = false;
		}
		glUniform1f(TiempoID, Sky);

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

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 4th attribute buffer : tangents
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glVertexAttribPointer(
			3,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 5th attribute buffer : bitangents
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glVertexAttribPointer(
			4,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// in the "MVP" uniform
		glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), vec3(1, 1, 1));
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), position); // A bit to the left
		glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), scala);
		glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

		glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);

		// Draw the triangle !
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		
		

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((const GLfloat*)&ProjectionMatrix[0]);
		glMatrixMode(GL_MODELVIEW);
		glm::mat4 MV = ViewMatrix * ModelMatrix;
		glLoadMatrixf((const GLfloat*)&MV[0]);

		glUseProgram(0);

		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		for (unsigned int i = 0; i < indices.size(); i++) {
			glm::vec3 p = indexed_vertices[indices[i]];
			glVertex3fv(&p.x);
			glm::vec3 o = glm::normalize(indexed_normals[indices[i]]);
			p += o * 0.1f;
			glVertex3fv(&p.x);
		}
		glEnd();
		// tangents
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for (unsigned int i = 0; i < indices.size(); i++) {
			glm::vec3 p = indexed_vertices[indices[i]];
			glVertex3fv(&p.x);
			glm::vec3 o = glm::normalize(indexed_tangents[indices[i]]);
			p += o * 0.1f;
			glVertex3fv(&p.x);
		}
		glEnd();
		// bitangents
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
		for (unsigned int i = 0; i < indices.size(); i++) {
			glm::vec3 p = indexed_vertices[indices[i]];
			glVertex3fv(&p.x);
			glm::vec3 o = glm::normalize(indexed_bitangents[indices[i]]);
			p += o * 0.1f;
			glVertex3fv(&p.x);
		}
		glEnd();
		// light pos
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		glVertex3fv(&lightPos.x);
		lightPos += glm::vec3(1, 0, 0) * 0.1f;
		glVertex3fv(&lightPos.x);
		lightPos -= glm::vec3(1, 0, 0) * 0.1f;
		glVertex3fv(&lightPos.x);
		lightPos += glm::vec3(0, 1, 0) * 0.1f;
		glVertex3fv(&lightPos.x);
		glEnd();
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
	GLuint LightID;
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
	vboindexer* vboIndex;
	tangentspace* tangent;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ModelView3x3MatrixID;
	GLuint DiffuseTexture;
	GLuint NormalTexture;
	GLuint SpecularTexture;
	GLuint normalbuffer;
	GLuint tangentbuffer;
	GLuint bitangentbuffer;
	GLuint elementbuffer;
	GLuint DiffuseTextureID;
	GLuint NormalTextureID;
	GLuint SpecularTextureID;
	GLuint TiempoID;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
	Quaternion* quaternion;
	bool gLookAtOther = true;
	float Sky = 0.0f;
	bool SkyB = true;
};
#endif