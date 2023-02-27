class Terrain
{
public:

	Terrain(const char* vertexShader, const char* fragmentShader, const char* Textura1, const char* Textura2, const char* Textura3, const char* Blendmap, const char* Alturas)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		shader = new Shader;
		programID = shader->LoadShaders(vertexShader, fragmentShader);
		MatrixID = glGetUniformLocation(programID, "MVP");

		Texture = new LoadTexture();
		textura1 = Texture->LoadAnyTexture(Textura1);
		Textura1ID = glGetUniformLocation(programID, "Texture1");

		textura2 = Texture->LoadAnyTexture(Textura2);
		Textura2ID = glGetUniformLocation(programID, "Texture2");

		textura3 = Texture->LoadAnyTexture(Textura3);
		Textura3ID = glGetUniformLocation(programID, "Texture3");

		blendmap = Texture->LoadAnyTexture(Blendmap);
		blendmap3ID = glGetUniformLocation(programID, "Blendmap");

		Altura = Texture->LoadAnyTexture(Alturas);
		AlturaID = glGetUniformLocation(programID, "heightmap");

		IDMix = glGetUniformLocation(programID, "time");

		modelLoad = new Load3dModel();
		modelLoad->loadOBJ("Assets/Terreno/Terreno.obj", vertices, uvs, normals);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}

	void Draw(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 u_ProjectionMatrix, float DayTransicionDuration)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Use our shader
		glUseProgram(programID);

		// in the "MVP" uniform
		glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), vec3(1, 1, 1));
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), vec3(0, 0, 0)); // A bit to the left
		glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), vec3(20, 1, 20));
		glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

		glm::mat4 MVP2 = u_ProjectionMatrix * ViewMatrix * ModelMatrix2;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);


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
			if (Sky > 0.8f && Sky <= 1.0f)
			{
				Sky -= DayTransicionDuration * 3.0f;
			}
			else if (Sky > 0.2f && Sky < 0.8f)
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
		glUniform1f(IDMix, Sky);


		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura1);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura1ID, 0);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textura2);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura2ID, 1);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textura3);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura3ID, 2);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, blendmap);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(blendmap3ID, 3);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, Altura);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(AlturaID, 4);

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
	}

private:
	GLuint VertexArrayID, programID, MatrixID, textura1, Textura1ID, textura2, Textura2ID, textura3, Textura3ID, blendmap, blendmap3ID;
	Shader* shader;
	LoadTexture* Texture;
	Load3dModel* modelLoad;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint Altura, AlturaID, IDMix;
	float Sky = 0.0f;
	bool SkyB = true;
};