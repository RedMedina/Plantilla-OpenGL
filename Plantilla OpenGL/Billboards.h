

class Billboards
{
public:
	Billboards(const char* vertexShader, const char* fragmentShader, const char* Textura)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		shader = new Shader();
		programID = shader->LoadShaders(vertexShader, fragmentShader);
		TextureLoad = new LoadTexture;
		// Vertex shader
		CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
		CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
		ViewProjMatrixID = glGetUniformLocation(programID, "VP");
		BillboardPosID = glGetUniformLocation(programID, "BillboardPos");
		BillboardSizeID = glGetUniformLocation(programID, "BillboardSize");
		LifeLevelID = glGetUniformLocation(programID, "LifeLevel");
		/*Valores para el pasto con viento*/
		IDtime1 = glGetUniformLocation(programID, "time1");
		IDtime2 = glGetUniformLocation(programID, "time2");
		idWindDir = glGetUniformLocation(programID, "WindDirection");
		//TextureID2 = glGetUniformLocation(programID, "blendMap");
		//Texture2 = TextureLoad->LoadAnyTexture(Blendmap);
		/*----------------------------------------------------------*/

		TextureID = glGetUniformLocation(programID, "myTextureSampler");
		Texture = TextureLoad->LoadAnyTexture(Textura);

		TiempoID = glGetUniformLocation(programID, "time");

		// The VBO containing the 4 vertices of the particles.
		static const GLfloat g_vertex_buffer_data[] = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
		};

		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	}

	void Draw(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::mat4 ModelMatrix, glm::vec3 position, glm::vec2 scala, float DayTransicionDuration)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Use our shader
		glUseProgram(programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		glUniform3f(BillboardPosID, -position.x, -position.y, -position.z); // The billboard will be just above the cube
		glUniform2f(BillboardSizeID, scala.x, scala.y);     // and 1m*12cm, because it matches its 256*32 resolution =)
		
														// Generate some fake life level and send it to glsl
		float LifeLevel =  0.1f + 0.7f;
		glUniform1f(LifeLevelID, LifeLevel);

		/*Valores para el pasto con viento*/
		TiempoValor += 1.0f;
		glUniform1f(IDtime1, TiempoValor);
		glUniform1f(IDtime2, TiempoValor);
		glm::vec2 DireccionViento = glm::vec2(1, 1);
		glUniform2f(idWindDir, DireccionViento.x, DireccionViento.y);
		// Bind our texture in Texture Unit 0
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, Texture2);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		//glUniform1i(TextureID, 1);
		/*----------------------------------------------------------*/


		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

		glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

		//Lights
		if (SkyB) {
			if (Sky > 0.8f && Sky <= 1)
			{
				Sky += DayTransicionDuration * 3.0f;
			}
			else if (Sky > 0.2 && Sky < 0.8)
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
			else if (Sky > 0.2 && Sky < 0.8)
			{
				Sky -= DayTransicionDuration * 5.0f;
			}
			else if (Sky < 0.2 && Sky >= 0)
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
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the billboard !
		// This draws a triangle_strip which looks like a quad.
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(0);
		glDisable(GL_BLEND);

	}

	~Billboards()
	{
		// Cleanup VBO and shader
		glDeleteBuffers(1, &billboard_vertex_buffer);
		glDeleteProgram(programID);
		glDeleteTextures(1, &TextureID);
		glDeleteVertexArrays(1, &VertexArrayID);
	}

private:
	GLuint programID;
	Shader* shader;
	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;
	GLuint BillboardPosID;
	GLuint BillboardSizeID;
	GLuint LifeLevelID;
	GLuint TextureID, TextureID2;
	GLuint Texture, Texture2;
	LoadTexture* TextureLoad;
	GLuint billboard_vertex_buffer;
	GLuint VertexArrayID;
	GLuint IDtime, idWindDir, ModelID, ViewID, PryectID;
	float TiempoValor = 1;
	GLuint TiempoID;
	GLuint IDtime1;
	GLuint IDtime2;
	float Sky = 0.0f;
	bool SkyB = true;
};
