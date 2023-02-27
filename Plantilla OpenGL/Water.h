#ifndef __Agua
#define __Agua
#include <glm/glm.hpp>

class Water
{
public:
	Water(const char* vertexShader, const char* fragmentShader, const char* texture, const char* Normales, float ancho, float alto)
	{
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        shader = new Shader;
        programID = shader->LoadShaders(vertexShader, fragmentShader);

        MatrixID = glGetUniformLocation(programID, "MVP");
        Offset = glGetUniformLocation(programID, "textureOffset");

        Texture = new LoadTexture();
        TexturaAgua = Texture->LoadAnyTexture(texture);
        TexturaAguaID = glGetUniformLocation(programID, "WaterTexture");

        NormalMap = Texture->LoadAnyTexture(Normales);
        TexturNormalID = glGetUniformLocation(programID, "NormalTexture");

        TimeID = glGetUniformLocation(programID, "time");
        lightDirectionID = glGetUniformLocation(programID, "lightDirection");

        modelLoad = new Load3dModel();
        modelLoad->loadOBJ("Assets/Agua/Agua.obj", vertices, uvs, normals);

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
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

    void Draw(glm::mat4 MVP, int init, glm::mat4 ViewMatrix, glm::mat4 u_ProjectionMatrix, glm::vec3 position, glm::mat4 ModelMatrix)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use our shader
        glUseProgram(programID);

        // in the "MVP" uniform
        glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), vec3(1, 1, 1));
        glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), vec3(0, 3, 0)); // A bit to the left
        glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), vec3(1, 1, 1));
        glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

        glm::mat4 MVP2 = u_ProjectionMatrix * ViewMatrix * ModelMatrix2;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);

        IncrementOffset -= 0.0005f;
        glUniform2f(Offset, OffsetPos.x, OffsetPos.y + IncrementOffset);

        Time += 0.5f;
        glUniform1f(TimeID, OffsetPos.x + Time);

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
    LoadTexture* Texture;
    Shader* shader;
    GLuint programID;
    GLuint TexturaAgua;
    GLuint TexturaAguaID;
    GLuint MatrixID, TimeID, lightDirectionID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    Load3dModel* modelLoad;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    GLuint VertexArrayID;
    GLuint offsetLocation;
    GLuint NormalMap, ViewMatrixID, ModelMatrixID, TexturNormalID, TexOff, WaveOff, Lighpos, Lightcolor, Offset;
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
};
#endif