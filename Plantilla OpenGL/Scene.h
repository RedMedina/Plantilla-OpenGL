#ifndef __Scene
#define __Scene

#include "Modelo3D.h"
#include "Water.h"
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
		lights = new Lights;
		lights->InitLights();
		//skydome = new Skydome("Skydome.vertex", "Skydome.fragment",32, 32, 500,"Assets/Skydome/sky.jpg");
		ModeloTest = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/ArbolAzul/Arbol.obj", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolN.bmp");
		ModeloCaja = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/Cubo/cube.obj", "Assets/Modelos/Cubo/cubeText.bmp", "Assets/Modelos/Cubo/cubeText.bmp", "Assets/Modelos/Cubo/normal.bmp");
		//Agua = new Water("WaterVertex.vertexhader", "WaterFragment.fragmentshader", "Assets/Agua/AguaC.jpg", 50, 50);
	}

	void Render(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 ProjectionMatrix, glm::vec3 position)
	{
		/*Aqui se ejecutara todo el juego*/

		// Borra el buffer de color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*Carga el skydome*/
		//skydome->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, vec3(5, 1, 1), vec3(1, 1, 1), vec3(1, 1, 1));

		/*Carga los modelos*/                                                              //Posicion       Escala       Rotacion
		ModeloTest->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(5, 1, 1), vec3(1,1,1), vec3(1,1,1));
		ModeloCaja->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(1, 1, 1), vec3(1,1,1), vec3(1,1,1));
		//Agua->Draw(MVP, init, ViewMatrix, ProjectionMatrix, position, ModelMatrix);

		/*Revisa si se dibujó el objeto*/
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
	//Skydome* skydome;
	Lights* lights;
	Modelo3D* ModeloTest;
	Modelo3D* ModeloCaja;
	Water* Agua;
};

#endif