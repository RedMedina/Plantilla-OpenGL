#ifndef __Agua
#define __Agua
#include <glm/glm.hpp>

class Water
{
public:
	Water(const char* vertexShader, const char* fragmentShader, const char* texture, const char* Normales, const char* Normales2, float ancho, float alto)
	{
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        shader = new Shader;
        programID = shader->LoadShaders(vertexShader, fragmentShader);

        MatrixID = glGetUniformLocation(programID, "MVP");
        modelID = glGetUniformLocation(programID, "model");
        Offset = glGetUniformLocation(programID, "textureOffset");

        Texture = new LoadTexture();
        TexturaAgua = Texture->LoadAnyTexture(texture);
        TexturaAguaID = glGetUniformLocation(programID, "WaterTexture");

        NormalMap = Texture->LoadAnyTexture(Normales);
        TexturNormalID = glGetUniformLocation(programID, "NormalTexture");

        NormalMap2 = Texture->LoadAnyTexture(Normales2);
        TexturNormalID2 = glGetUniformLocation(programID, "NormalTexture2");

        CloudsTexture = Texture->LoadAnyTexture("Assets/Skydome/nube1.png");
        CloudsTextureID = glGetUniformLocation(programID, "ReflectionClouds");

        MixvalueID = glGetUniformLocation(programID, "MixValue");

        TimeID = glGetUniformLocation(programID, "time");
        lightDirectionID = glGetUniformLocation(programID, "lightDirection");

        modelLoad = new Load3dModel();
        modelLoad->loadOBJ("Assets/Agua/Agua.obj", vertices, uvs, normals);

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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);


        glGenBuffers(1, &tangentbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);


        glGenBuffers(1, &bitangentbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);


        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    }

    ~Water()
    {
        // Cleanup VBO and shader
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &uvbuffer);
        glDeleteProgram(programID);
        glDeleteTextures(1, &TexturaAguaID);
        glDeleteVertexArrays(1, &VertexArrayID);
    }

    void Draw(glm::mat4 MVP, int init, glm::mat4 ViewMatrix, glm::mat4 u_ProjectionMatrix, glm::vec3 position, glm::mat4 ModelMatrix, glm::vec3 PositionWater, glm::vec3 Scale)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);

        // Use our shader
        glUseProgram(programID);

        // in the "MVP" uniform
        glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), vec3(1, 1, 1));
        glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), vec3(PositionWater.x, PositionWater.y, PositionWater.z)); // A bit to the left
        glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), vec3(Scale.x, Scale.y, Scale.z));
        glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

        glm::mat4 MVP2 = u_ProjectionMatrix * ViewMatrix * ModelMatrix2;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &ModelMatrix2[0][0]);

        IncrementOffset -= 0.0005f;
        glUniform2f(Offset, OffsetPos.x, OffsetPos.y + IncrementOffset);

        Time += 0.001f;
        glUniform1f(TimeID, Time);

        glm::vec3 lightPos = glm::vec3(0, 0, 4);
        glUniform3f(lightDirectionID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexturaAgua);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TexturaAguaID, 0);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, NormalMap);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TexturNormalID, 1);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, NormalMap2);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TexturNormalID2, 2);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, CloudsTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(CloudsTextureID, 3);


        if (SkyB) {
           Sky += 0.01f; 
        }
        else {
           Sky -= 0.01f;
        }

        if (Sky < 0.0) {
            Sky = 0.0f;
            SkyB = true;
        }
        else if (Sky > 1.0f) {

            Sky = 1.0f;
            SkyB = false;
        }
        glUniform1f(MixvalueID, Sky);


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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((const GLfloat*)&u_ProjectionMatrix[0]);
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

        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

private:
    LoadTexture* Texture;
    Shader* shader;
    GLuint programID;
    GLuint TexturaAgua, CloudsTexture;
    GLuint TexturaAguaID, CloudsTextureID;
    GLuint MatrixID, TimeID, lightDirectionID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    Load3dModel* modelLoad;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    GLuint VertexArrayID;
    GLuint offsetLocation;
    GLuint NormalMap, ViewMatrixID, ModelMatrixID, TexturNormalID, NormalMap2, TexturNormalID2,TexOff, WaveOff, Lighpos, Lightcolor, Offset, modelID, MixvalueID;
    vboindexer* vboIndex;
    tangentspace* tangent;
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    std::vector<glm::vec3> indexed_tangents;
    std::vector<glm::vec3> indexed_bitangents;
    GLuint normalbuffer;
    GLuint tangentbuffer, ProyectionID, CameraIDPos, ModelVID, bitangentbuffer, elementbuffer;
    glm::vec2 OffsetPos = glm::vec2(10.1f, 20.2f);
    float IncrementOffset = 0, Time = 0;
    float Sky = 0.0f;
    bool SkyB = true;
};
#endif