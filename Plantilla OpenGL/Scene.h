#ifndef __Scene
#define __Scene

#include "Modelo3D.h"
#include "Water.h"
#include "Billboards.h"
#include "skybox.h"
#include "Clouds.h"
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
		//Agua = new Water("WaterVertex.vertexhader", "WaterFragment.fragmentshader", "Assets/Agua/AguaC.jpg", 50, 50);
		
		/*Billboards*/
		bill1 = new Billboards("billboard.vertex","billboard.fragment","Assets/Billboards/grass2.png");
	}

	void Render(GLFWwindow* window , glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 ProjectionMatrix, glm::vec3 position)
	{
		/*Aqui se ejecutara todo el juego*/

		// Borra el buffer de color
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/*Carga el skydome*/
		skydome->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, window);
		Nubes->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix);

		/*Carga los modelos*/                                                              //Posicion       Escala       Rotacion
		ModeloTest->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(5, 1, 1), vec3(1,1,1), vec3(1,1,1));
		ModeloCaja->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(1, 1, 10), vec3(1,1,1), vec3(1,1,1));
		
		//Agua->Draw(MVP, init, ViewMatrix, ProjectionMatrix, position, ModelMatrix);

		/*Carga los billboards*/                 //Posicion       Escala
		bill1->Draw(ViewMatrix, ProjectionMatrix, vec3(-5, 1, 1), vec2(1, 1));

		/*Revisa si se dibuj� el objeto*/
		if (init < 1)
		{
			cout << "Scene cargada totalmente!" << endl;
		}
		init++;
	}

	~Scene()
	{
		/*Vaciamos la memoria del juego*/
		//delete skydome;
		delete lights;
		delete ModeloTest;
		cout << "Memoria liberada." << endl;
	}
private:
	int init = 0;
	Skybox* skydome;
	Lights* lights;
	Modelo3D* ModeloTest;
	Modelo3D* ModeloCaja;
	Billboards* bill1;
	Water* Agua;
	Clouds* Nubes;
};

#endif