

class Grass
{
public:
	Grass(const char* vertexShader, const char* fragmentShader, const char* Textura)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		shaders = new Shader;
		ProgramID = shaders->LoadShaders(vertexShader, fragmentShader);

		MatrixID = glGetUniformLocation(ProgramID, "MVP");
		ProyeccionID = glGetUniformLocation(ProgramID, "viewProjMatrix");

		textura = new LoadTexture();
		Texture = textura->LoadAnyTexture(Textura);
		TextureID = glGetUniformLocation(ProgramID, "myTextureSampler");

		IDMix = glGetUniformLocation(ProgramID, "time");
		IDMix2 = glGetUniformLocation(ProgramID, "time1");
		TiempoID = glGetUniformLocation(ProgramID, "time2");
		PositionID = glGetUniformLocation(ProgramID, "Position");

		Model = new Load3dModel();
		Model->loadOBJ("Assets/Grass/Grass.obj", vertices, uvs, normals);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}

	void Draw(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat4 ProjectionMatrix, float DayTransicionDuration, vec3 Position)
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);

		// Use our shader
		glUseProgram(ProgramID);

		// in the "MVP" uniform
		glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), vec3(1, 1, 1));
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), vec3(Position.x, Position.y, Position.z)); // A bit to the left
		glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), vec3(1, 1, 1));
		glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

		glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glUniformMatrix4fv(ProyeccionID, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		TiempoValor += 1.0f;
		glUniform1f(IDMix, TiempoValor);
		glUniform1f(IDMix2, TiempoValor);

		glUniform3f(PositionID, Position.x, Position.y, Position.z);


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

		glDepthMask(GL_TRUE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}

private:
	GLuint VertexArrayID, ProgramID, MatrixID, ProyeccionID, Texture, TextureID, IDMix, uvbuffer, vertexbuffer, TiempoID, PositionID, IDMix2;
	Shader* shaders;
	LoadTexture* textura;
	Load3dModel* Model;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	float TiempoValor = 0;
	float Sky = 0.0f;
	bool SkyB = true;
};
