#include <FreeImage.h>
#include "LoadTexture.h"
#include "Load3dmodel.h"
#include "vertexBufferObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

class Terrain
{
public:

	Terrain(const char* vertexShader, const char* fragmentShader, const char* Textura1, const char* Textura1Normal, const char* Textura2, const char* Textura2Normal, const char* Textura3, const char* Textura3Normal, const char* Blendmap, const char* Alturas)
	{
		//	InitTerrainWithModel(vertexShader, fragmentShader, Textura1, Textura2, Textura3, Blendmap, Alturas);
		CreateTerrain(vertexShader, fragmentShader, Textura1, Textura1Normal, Textura2, Textura2Normal, Textura3, Textura3Normal, Blendmap, Alturas);
	}

	void InitTerrainWithModel(const char* vertexShader, const char* fragmentShader, const char* Textura1, const char* Textura2, const char* Textura3, const char* Blendmap, const char* Alturas)
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

	bool CreateTerrain(const char* vertexShader, const char* fragmentShader, const char* Textura1, const char* Textura1Normal, const char* Textura2, const char* Textura2Normal, const char* Textura3, const char* Textura3Normal, const char* Blendmap, const char* Alturas)
	{
		if (bLoaded)
		{
			bLoaded = false;
			//ReleaseHeightmap();
		}
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib(0);

		fif = FreeImage_GetFileType(Alturas, 0); // Check the file signature and deduce its format

		if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(Alturas);

		if (fif == FIF_UNKNOWN) // If still unknown, return failure
			return false;

		if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
			dib = FreeImage_Load(fif, Alturas);
		if (!dib)
			return false;

		BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
		iRows = FreeImage_GetHeight(dib);
		iCols = FreeImage_GetWidth(dib);

		// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
		if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
			return false;

		// How much to increase data pointer to get to next pixel data
		unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
		// Length of one row in data
		unsigned int row_step = ptr_inc * iCols;

		/*Shader Program*/
		shader = new Shader;
		programID = shader->LoadShaders(vertexShader, fragmentShader);
		MatrixID = glGetUniformLocation(programID, "MVP");

		MVID = glGetUniformLocation(programID, "MV");
		ModelID = glGetUniformLocation(programID, "Model");
		MV3x3 = glGetUniformLocation(programID, "MV3x3");
		VID = glGetUniformLocation(programID, "V");

		Texture = new LoadTexture();
		textura1 = Texture->LoadAnyTexture(Textura1);
		Textura1ID = glGetUniformLocation(programID, "Texture1");

		textura2 = Texture->LoadAnyTexture(Textura2);
		Textura2ID = glGetUniformLocation(programID, "Texture2");

		textura3 = Texture->LoadAnyTexture(Textura3);
		Textura3ID = glGetUniformLocation(programID, "Texture3");

		textura1Normal = Texture->LoadAnyTexture(Textura1Normal);
		Textura1IDNormal = glGetUniformLocation(programID, "Texture1Normal");

		textura2Normal = Texture->LoadAnyTexture(Textura2Normal);
		Textura2IDNormal = glGetUniformLocation(programID, "Texture2Normal");

		textura3Normal = Texture->LoadAnyTexture(Textura3Normal);
		Textura3IDNormal = glGetUniformLocation(programID, "Texture3Normal");

		texturaBackNormal = Texture->LoadAnyTexture("Assets/Terreno/normalMap.png");
		TexturaBackNormalId = glGetUniformLocation(programID, "BackNormal");

		blendmap = Texture->LoadAnyTexture(Blendmap);
		blendmap3ID = glGetUniformLocation(programID, "Blendmap");

		Altura = Texture->LoadAnyTexture(Alturas);
		AlturaID = glGetUniformLocation(programID, "heightmap");

		IDMix = glGetUniformLocation(programID, "time");

		IdHeight = glGetUniformLocation(programID, "HeightmapScaleMatrix");

		/*Grass---------------------------------------------------------
		GrassShader = new Shader();
		GrassShaderID = GrassShader->LoadSGeometryhaders("Grass.vertex", "Grass.fragment", "Grass.geom");
		LoadGrassTexture = new LoadTexture();
		GrassTextura = LoadGrassTexture->LoadDDS("Assets/Terreno/grassPack.dds");
		GraddTexturaID = glGetUniformLocation(programID, "gSampler");
		ProyeccionIDGrass = glGetUniformLocation(programID, "projMatrix");
		ModeloIDGrass = glGetUniformLocation(programID, "modelMatrix");
		VistaIDGrass = glGetUniformLocation(programID, "viewMatrix");
		IDTiempoPasado = glGetUniformLocation(programID, "fTimePassed");
		IDPosicionGrass = glGetUniformLocation(programID, "vEyePosition");
		/*--------------------------------------------------------------*/

		vboHeightmapData.CreateVBO();
		// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
		vVertexData = vector< vector< glm::vec3> >(iRows, vector<glm::vec3>(iCols));
		vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols));

		float fTextureU = float(iCols) * 0.1f;
		float fTextureV = float(iRows) * 0.1f;

		FOR(i, iRows)
		{
			FOR(j, iCols)
			{
				float fScaleC = float(j) / float(iCols - 1);
				float fScaleR = float(i) / float(iRows - 1);
				float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
				vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
				vCoordsData[i][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
			}
		}

		vector< vector<glm::vec3> > vNormals[2];
		FOR(i, 2)vNormals[i] = vector< vector<glm::vec3> >(iRows - 1, vector<glm::vec3>(iCols - 1));

		FOR(i, iRows - 1)
		{
			FOR(j, iCols - 1)
			{
				glm::vec3 vTriangle0[] =
				{
					vVertexData[i][j],
					vVertexData[i + 1][j],
					vVertexData[i + 1][j + 1]
				};
				glm::vec3 vTriangle1[] =
				{
					vVertexData[i + 1][j + 1],
					vVertexData[i][j + 1],
					vVertexData[i][j]
				};

				glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
				glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

				vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
				vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
			}
		}
		vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

		FOR(i, iRows)
			FOR(j, iCols)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals

			glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if (j != 0 && i != 0)
				FOR(k, 2)vFinalNormal += vNormals[k][i - 1][j - 1];
			// Look for upper-right triangles
			if (i != 0 && j != iCols - 1)vFinalNormal += vNormals[0][i - 1][j];
			// Look for bottom-right triangles
			if (i != iRows - 1 && j != iCols - 1)
				FOR(k, 2)vFinalNormal += vNormals[k][i][j];
			// Look for bottom-left triangles
			if (i != iRows - 1 && j != 0)
				vFinalNormal += vNormals[1][i][j - 1];
			vFinalNormal = glm::normalize(vFinalNormal);

			vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
		}
		// First, create a VBO with only vertex data
		vboHeightmapData.CreateVBO(iRows * iCols * (2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
		FOR(i, iRows)
		{
			FOR(j, iCols)
			{
				vboHeightmapData.AddData(&vVertexData[i][j], sizeof(glm::vec3)); // Add vertex
				vboHeightmapData.AddData(&vCoordsData[i][j], sizeof(glm::vec2)); // Add tex. coord
				vboHeightmapData.AddData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
			}
		}
		// Now create a VBO with heightmap indices
		vboHeightmapIndices.CreateVBO();
		int iPrimitiveRestartIndex = iRows * iCols;
		FOR(i, iRows - 1)
		{
			FOR(j, iCols)
				FOR(k, 2)
			{
				int iRow = i + (1 - k);
				int iIndex = iRow * iCols + j;
				vboHeightmapIndices.AddData(&iIndex, sizeof(int));
			}
			// Restart triangle strips
			vboHeightmapIndices.AddData(&iPrimitiveRestartIndex, sizeof(int));
		}

		glGenVertexArrays(1, &uiVAO);
		glBindVertexArray(uiVAO);
		// Attach vertex data to this VAO
		vboHeightmapData.BindVBO();
		//vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);
		// And now attach index data to this VAO
		// Here don't forget to bind another type of VBO - the element array buffer, or simplier indices to vertices
		vboHeightmapIndices.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
		//vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);

		bLoaded = true; // If get here, we succeeded with generating heightmap

		// Get a handle for our "LightPosition" uniform
		glUseProgram(programID);
		LightID = glGetUniformLocation(programID, "lightPos");

		return true;
	}

	void Render(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 u_ProjectionMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 Model, float DayTransicionDuration, glm::vec3 position)
	{
		glDisable(GL_CULL_FACE);
		// Use our shader
		glUseProgram(programID);

		glm::mat4 HeightT = glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale));
		glUniformMatrix4fv(IdHeight, 1, GL_FALSE, &HeightT[0][0]);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glm::mat4 MV = ViewMatrix * Model;
		glUniformMatrix4fv(MVID, 1, GL_FALSE, &MV[0][0]);
		glUniformMatrix4fv(VID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix3fv(MV3x3, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);

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

		glm::mat4 rotationMatrixLight = glm::rotate(glm::mat4(1.0f), glm::radians(Sky*360), vec3(1, 1, 0));
		glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 4.0f);
		glm::vec4 lightPosition(0.0f, 0.0f, 4.0f, 4.0f); //ejemplo de posición de luz
		lightPosition = rotationMatrixLight * lightPosition;
		glUniform3f(LightID, lightPosition.x, lightPosition.y, lightPosition.z);


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

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, textura1Normal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura1IDNormal, 5);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, textura2Normal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura2IDNormal, 6);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, textura3Normal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura3IDNormal, 7);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, texturaBackNormal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TexturaBackNormalId, 8);

		/*Grass----------------------------------------------------------------------/
		glUseProgram(GrassShaderID);
		glUniformMatrix4fv(ProyeccionIDGrass, 1, GL_FALSE, &u_ProjectionMatrix[0][0]);
		glUniformMatrix4fv(ModeloIDGrass, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(VistaIDGrass, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniform3f(IDPosicionGrass, position.x, position.y, position.z);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, GrassTextura);
		glUniform1i(GraddTexturaID, 5);
		//---------------------------------------------------------------------------*/

		if (unoA < 1) {
			vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);
		}
		if (unoA < 1) {
			vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);
		}

		// Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
		// Texture coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
		// Normal vectors
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

		// Index buffer
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		unoA++;
		glBindVertexArray(uiVAO);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(iRows * iCols);

		int iNumIndices = (iRows - 1) * iCols * 2 + iRows - 1;
		glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);

		glEnable(GL_CULL_FACE);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glUseProgram(0);
	}

	void SetRenderSize(float fRenderX, float fHeight, float fRenderZ)
	{
		vRenderScale = glm::vec3(fRenderX, fHeight, fRenderZ);
	}

	float GetHeightFromRealVector(glm::vec3 vRealPosition)
	{
		int iColumn = int((vRealPosition.x + vRenderScale.x * 0.5f) * float(iCols) / (vRenderScale.x));
		int iRow = int((vRealPosition.z + vRenderScale.z * 0.5f) * float(iRows) / (vRenderScale.z));

		iColumn = glm::min(iColumn, iCols - 1);
		iRow = glm::min(iRow, iRows - 1);

		iColumn = glm::max(iColumn, 0);
		iRow = glm::max(iRow, 0);

		return vVertexData[iRow][iColumn].y * vRenderScale.y;
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

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, textura1Normal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura1IDNormal, 5);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, textura2Normal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura2IDNormal, 6);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, textura3Normal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Textura3IDNormal, 7);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, texturaBackNormal);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TexturaBackNormalId,8);

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
	GLuint uiVAO, MVID, ModelID, MV3x3, uiGrassVAO, textura1Normal, textura2Normal, textura3Normal, Textura1IDNormal, Textura2IDNormal, Textura3IDNormal;
	GLuint VertexArrayID, programID, LightID, VID, GrassShaderID, MatrixID, textura1, Textura1ID, textura2, Textura2ID, textura3, Textura3ID, blendmap, blendmap3ID, IdHeight, GrassTextura, GraddTexturaID, ModeloIDGrass, VistaIDGrass, ProyeccionIDGrass, IDTiempoPasado, IDPosicionGrass, texturaBackNormal, TexturaBackNormalId;
	Shader* shader, *GrassShader;
	LoadTexture* Texture, *LoadGrassTexture;
	Load3dModel* modelLoad;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	vector<vector<glm::vec3>>vVertexData;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
	vboindexer* vboIndex;
	tangentspace* tangent;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint tangentbuffer;
	GLuint bitangentbuffer;
	GLuint elementbuffer;
	GLuint Altura, AlturaID, IDMix;
	float Sky = 0.0f;
	bool SkyB = true;
	int iRows;
	int iCols;
	bool bLoaded;
	glm::vec3 vRenderScale;
	CVertexBufferObject vboHeightmapData;
	CVertexBufferObject vboHeightmapIndices;
	int unoA = 0;
	CVertexBufferObject vboGrassData;
	int iNumGrassTriangles;
	float fTimePassed;
};