#include "LoadTexture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

class Skybox{

public:
    Skybox(float radius, const char* vertexShader, const char* fragmentShader, const char* texturaDia, const char* texturaAtardecer, const char* texturaNoche)
    {
        /*Musica*/
        PajarosFX = new Music("Assets/Music/BackgroundBirds.mp3");
        //PajarosFX->Play();
        //PajarosFX->Playback();
       // PajarosFX->
        BackgroundNoche = new Music("Assets/Music/NocheFX.mp3");
        //BackgroundNoche->Play();
        //BackgroundNoche->Stop();

        Escala = radius;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        shaders = new Shader;
        ProgramID = shaders->LoadShaders(vertexShader, fragmentShader);

        MatrixID = glGetUniformLocation(ProgramID, "MVP");

        textura = new LoadTexture();
    
        DiaTextura = textura->LoadAnyTexture(texturaDia);
        TexturaIDdia = glGetUniformLocation(ProgramID, "TexturaDia");

        DiaAtar = textura->LoadAnyTexture(texturaAtardecer);
        DiaAtarID = glGetUniformLocation(ProgramID, "TexturaAtardecer");

        DiaNoche = textura->LoadAnyTexture(texturaNoche);
        DiaNocheID = glGetUniformLocation(ProgramID, "TexturaNoche");

        IDMix = glGetUniformLocation(ProgramID, "mixValue");

        Model = new Load3dModel();

        //Model->loadOBJ("Assets/Skydome/sphere.obj", vertices, uvs, normals);
        Model->loadOBJ("Assets/Skydome/cube.obj", vertices, uvs, normals);

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}

    void Draw(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat4 ProjectionMatrix, GLFWwindow* window, float DayTransicionDuration)
    {
        // Deshabilitar culling de caras
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        // Use our shader
        glUseProgram(ProgramID);

        skyRotation < 360 ? skyRotation += 0.025f : skyRotation = 0;
        // in the "MVP" uniform
        glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(skyRotation), vec3(0, 1, 0));
        glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), vec3(0, 0, 0)); // A bit to the left
        glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), vec3(1* Escala, 1* Escala, 1* Escala));
        glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;

        int width= 1024, height = 768;
        glfwGetFramebufferSize(window, &width, &height);
        float aspectRatio = (float)width / (float)height;
        // Actualizar la matriz de proyección con una matriz ortográfica
        //ProjectionMatrix = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);

        glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);

        //Lights
        if (SkyB) {
            if (Sky > 0.8f && Sky <= 1)
            {
                Sky += DayTransicionDuration * 3.0f;
                PajarosFX->Stop();
                BackgroundNoche->Playback();
            }
            else if (Sky > 0.2 && Sky < 0.8)
            {
                Sky += DayTransicionDuration * 5.0f;
                BackgroundNoche->Stop();
                PajarosFX->Playback();
            }
            else if (Sky < 0.2 && Sky >= 0)
            {
                Sky += DayTransicionDuration;
                BackgroundNoche->Stop();
                PajarosFX->Playback();
            }
        }
        else {
            if (Sky > 0.8f && Sky <= 1)
            {
                Sky -= DayTransicionDuration * 3.0f;
                PajarosFX->Stop();
                BackgroundNoche->Playback();
            }
            else if (Sky > 0.2 && Sky < 0.8)
            {
                Sky -= DayTransicionDuration * 5.0f;
                BackgroundNoche->Stop();
                PajarosFX->Playback();
            }
            else if (Sky < 0.2 && Sky >= 0)
            {
                Sky -= DayTransicionDuration;
                BackgroundNoche->Stop();
                PajarosFX->Playback();
            }
        }

        /*Reproduce la musica*/
        PajarosFX->Update(0.5);
        BackgroundNoche->Update(0.8f);

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
        glBindTexture(GL_TEXTURE_2D, DiaTextura);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TexturaIDdia, 0);

        // Bind our texture in Texture Unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, DiaAtar);
        // Set our "myTextureSampler" sampler to user Texture Unit 1
        glUniform1i(DiaAtarID, 1);

        // Bind our texture in Texture Unit 2
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, DiaNoche);
        // Set our "myTextureSampler" sampler to user Texture Unit 2
        glUniform1i(DiaNocheID, 2);

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

        // Establecer una matriz de proyección ortográfica
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        int w= 1024, h= 768;
        glfwGetFramebufferSize(window, &w, &h);
        glOrtho(0, w, 0, h, -1, 1);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Habilitar culling de caras nuevamente
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    GLuint vbo, ebo, tbo;
    GLuint ProgramID, VertexArrayID, MatrixID, DiaTextura, TexturaIDdia, DiaAtar, DiaAtarID, DiaNoche, DiaNocheID, IDMix;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    Shader* shaders;
    LoadTexture* textura;
    Load3dModel* Model;
    float Escala;
    float Sky = 0.0f;
    bool SkyB = true;
    float skyRotation = 0;
    Music* BackgroundNoche, *PajarosFX;
};

