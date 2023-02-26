#ifndef __Agua
#define __Agua
#include <glm/glm.hpp>

class Water
{
public:
	Water(const char* vertexShader, const char* fragmentShader, const char* texture, float ancho, float alto)
	{
        vboIndex = new vboindexer();
        tangent = new tangentspace();

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        shader = new Shader;
        programID = shader->LoadShaders(vertexShader, fragmentShader);
        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        CameraIDPos = glGetUniformLocation(programID, "u_CameraPosition");
        ModelMatrixID = glGetUniformLocation(programID, "u_ViewMatrix");
        ProyectionID = glGetUniformLocation(programID, "u_ProjectionMatrix");
        ModelVID = glGetUniformLocation(programID, "model");

        Texture = new LoadTexture();
        TexturaAgua = Texture->LoadAnyTexture(texture);
        NormalMap = Texture->LoadAnyTexture("Assets/Agua/AguaN.jpg");

        TexturaAguaID = glGetUniformLocation(programID, "u_Texture");
        TexturNormalID = glGetUniformLocation(programID, "u_NormalMap");

        TexOff = glGetUniformLocation(programID, "u_TexOffset");
        WaveOff = glGetUniformLocation(programID, "u_WaveOffset");

        Lighpos = glGetUniformLocation(programID, "u_LightPosition");
        Lightcolor = glGetUniformLocation(programID, "u_LightColor");

        modelLoad = new Load3dModel();
        bool res = modelLoad->loadOBJ("Assets/Agua/Agua.obj", vertices, uvs, normals);

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

        // Generate a buffer for the indices as well

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


        glUseProgram(programID);
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
        // Use our shader
        glUseProgram(programID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        glUniformMatrix4fv(ProyectionID, 1, GL_FALSE, &u_ProjectionMatrix[0][0]);
        glUniformMatrix4fv(ModelVID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniform3f(CameraIDPos, position.x, position.y, position.z);

        glm::vec3 lightPos = glm::vec3(0, 0, 4);
        glUniform3f(Lighpos, lightPos.x, lightPos.y, lightPos.z);

        glm::vec3 lightColor = glm::vec3(1, 1, 1);
        glUniform3f(Lightcolor, lightColor.x, lightColor.y, lightColor.z);

        // Establecer el valor de la variable uniform "offset"
        glm::vec2 offsetValue(0.5f, 0.5f);
        glUniform2f(TexOff, offsetValue.x, offsetValue.y);

        // Establecer el valor de la variable uniform "offset"
        // glm::vec2 offsetValue(10.0f + init, 10.0f + init);
        glUniform2f(WaveOff, offsetValue.x, offsetValue.y);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexturaAgua);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TexturaAguaID, 0);

        // Bind our normal texture in Texture Unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, NormalMap);
        // Set our "Normal	TextureSampler" sampler to user Texture Unit 0
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
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

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

private:
    LoadTexture* Texture;
    Shader* shader;
    GLuint programID;
    GLuint TexturaAgua;
    GLuint TexturaAguaID;
    GLuint MatrixID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    Load3dModel* modelLoad;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    GLuint VertexArrayID;
    GLuint offsetLocation;
    GLuint NormalMap, ViewMatrixID, ModelMatrixID, TexturNormalID, TexOff, WaveOff, Lighpos, Lightcolor;
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
};
#endif