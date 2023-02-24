#include "Modelo3D.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "skydome.h"
#include "Lights.h"
#include "Camera.h"
#include "Primitivos.h"


using namespace std;

class Scene
{
public:
	Scene()
	{
		/*Inicializamos algo*/
		lights = new Lights;
		lights->InitLights();
		ModeloTest = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/ArbolAzul/Arbol.obj", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolN.bmp");
	}

	void Render(glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 ProjectionMatrix)
	{
		/*Aqui se ejecutara todo el juego*/

		// Borra el buffer de color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*Carga los modelos*/
		ModeloTest->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix);

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
};