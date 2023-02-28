#ifndef __Scene
#define __Scene

#include "Modelo3D.h"
#include "Music.h"
#include "Water.h"
#include "Billboards.h"
#include "skybox.h"
#include "Clouds.h"
#include "Terrain.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "Lights.h"
#include "Camera.h"


using namespace std;

class Scene
{
public:
	Scene()
	{
		/*Inicializamos algo*/

		/*Luces*/
		lights = new Lights;
		lights->InitLights();
		
		/*Skybox*/
		skydome = new Skybox(500,"Skybox.vertex", "Skybox.fragment", "Assets/Skydome/Dia.png", "Assets/Skydome/Atardecer.png","Assets/Skydome/SkyBoxNcche2.png");
		/*Nubes*/
		Nubes = new Clouds("Clouds.vertex", "Clouds.fragment", "Assets/Skydome/nube1.png");

		/*Modelos 3D*/
		ModeloTest = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/ArbolAzul/Arbol.obj", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolN.bmp");
		ModeloCaja = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/Cubo/cube.obj", "Assets/Modelos/Cubo/cubeText.bmp", "Assets/Modelos/Cubo/cubeText.bmp", "Assets/Modelos/Cubo/normal.bmp");
		
		/*Agua*/
		Agua = new Water("WaterVertex.vertexhader", "WaterFragment.fragmentshader", "Assets/Agua/AguaTextura3.png", "Assets/Agua/AguaN1.jpg", "Assets/Agua/AguaN2.jpg", 50, 50);
		
		/*Billboards*/
		bill1 = new Billboards("billboard.vertex","billboard.fragment","Assets/Billboards/grass2.png");
		Pasto = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/grass7.png");

		/*Terreno*/
		Terreno = new Terrain("Terrain.vertex", "Terrain.fragment", "Assets/Terreno/Monte.jpg", "Assets/Terreno/Pasto.jpg", "Assets/Terreno/Tierra.jpg", "Assets/Terreno/Blend3.png", "Assets/Terreno/Alturas2.png");
	}

	void Render(GLFWwindow* window , glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 ProjectionMatrix, glm::vec3 position)
	{
		/*Aqui se ejecutara todo el juego*/

		// Borra el buffer de color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*Carga el skydome*/
		skydome->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, window, DayTransicionDuration);
		Nubes->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, DayTransicionDuration);

		/*Carga los modelos*/                                                              //Posicion                                                          Escala     Rotacion
		ModeloTest->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(5, Terreno->GetHeightFromRealVector(glm::vec3(5, 0, 1)), 1), vec3(1,1,1),  0, DayTransicionDuration);
		
		/*Modelos para desbugear el Terreno no preguntes, solo deja esto como está xd*/
		ModeloCaja->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(1, -200, 10), vec3(1, 1, 1), 0, DayTransicionDuration);
		/*Carga el Terreno*/
		Terreno->SetRenderSize(300.0f, 75.0f, 300.0f);
		Terreno->Render(MVP, ViewMatrix, ProjectionMatrix, ModelMatrix, DayTransicionDuration, position);
		/*Carga el agua*/
		Agua->Draw(MVP, init, ViewMatrix, ProjectionMatrix, position, ModelMatrix);
		/*Se coloca en la varible Y_Position la posción del Y del terreno apartir de la posición de la camara*/
		Y_Position = Terreno->GetHeightFromRealVector(position) + 3.0f;

		/*Carga los billboards*/                              //Posicion  Esta función me dice que altura es la del terreno en esas coords Escala
		                                                           //X                            Y                            Z
		bill1->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(-5, Terreno->GetHeightFromRealVector(glm::vec3(-5, 0, 1)), 1), vec2(1, 1));
		Pasto->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(-8, Terreno->GetHeightFromRealVector(glm::vec3(-8, 0, 1)), 1), vec2(1, 1));

		/*Revisa si se dibujó el objeto*/
		if (init < 1)
		{
			cout << "Scene cargada totalmente!" << endl;
		}
		init++;
	}

	float Get_YPosition()
	{
		/*Se envia la variable a main.cpp*/
		return Y_Position;
	}

	~Scene()
	{
		/*Vaciamos la memoria del juego*/
		delete skydome;
		delete lights;
		delete ModeloTest;
		delete ModeloCaja;
		delete bill1;
		delete Nubes;
		delete Pasto;
		cout << "Memoria liberada." << endl;
	}
private:
	int init = 0;
	Skybox* skydome;
	Lights* lights;
	Modelo3D* ModeloTest;
	Modelo3D* ModeloCaja;
	Billboards* bill1;
	Billboards* Pasto;
	Water* Agua;
	Clouds* Nubes;
	float DayTransicionDuration = 0.00005f;
	Terrain* Terreno;
	float Y_Position = 0;
};

#endif