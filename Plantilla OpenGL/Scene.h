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
#include "Collider.h"

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
		Arbol1 = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/Arbol/Arbol1.obj", "Assets/Modelos/Arbol/TexturaArbol.png", "Assets/Modelos/Arbol/TexturaArbol.png", "Assets/Modelos/Arbol/ArbolNormales.png");

		/*Agua*/
		Agua = new Water("WaterVertex.vertexhader", "WaterFragment.fragmentshader", "Assets/Agua/AguaTextura3.png", "Assets/Agua/AguaN1.jpg", "Assets/Agua/AguaN2.jpg", 50, 50);
		
		/*Billboards*/
		bill1 = new Billboards("billboard.vertex","billboard.fragment","Assets/Billboards/grass2.png");
		Pasto = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/grass7.png");
		Arbusto = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/arbusto.png");
		Arbol = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/Arbol.png");

		/*Terreno*/
		Terreno = new Terrain("Terrain.vertex", "Terrain.fragment", "Assets/Terreno/Monte.jpg", "Assets/Terreno/MonteNormal.png","Assets/Terreno/Pasto.jpg", "Assets/Terreno/PasNormal.png", "Assets/Terreno/Tierra.jpg", "Assets/Terreno/TierraNormal.png", "Assets/Terreno/Blend3.png", "Assets/Terreno/Alturas2.png");
	
		/*Colliders*/
		PlayerCollider.InitCollider(vec3(0, 0, 5), 6);
		ArbolAzulCollider.InitCollider(vec3(5, Terreno->GetHeightFromRealVector(glm::vec3(5, 0, 1)), 1), 10);
		Arbol1Collider.InitCollider(vec3(20, Terreno->GetHeightFromRealVector(glm::vec3(20, 0, 20)), 20), 10);
		PushCollider(Arbol1Collider);
		PushCollider(ArbolAzulCollider);
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
		Terreno->Render(MVP, ViewMatrix, ProjectionMatrix, ModelView3x3Matrix,ModelMatrix, DayTransicionDuration, position);
		/*Carga el agua*/
		Agua->Draw(MVP, init, ViewMatrix, ProjectionMatrix, position, ModelMatrix);
		/*Se coloca en la varible Y_Position la posción del Y del terreno apartir de la posición de la camara*/
		Y_Position = Terreno->GetHeightFromRealVector(position) + 3.0f;

		Arbol1->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(20, Terreno->GetHeightFromRealVector(glm::vec3(20, 0, 20)), 20), vec3(1, 1, 1), 0, DayTransicionDuration);
		/*Carga los billboards*/                              //Posicion  Esta función me dice que altura es la del terreno en esas coords Escala
		                                                           //X                            Y                            Z
		bill1->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(-5, Terreno->GetHeightFromRealVector(glm::vec3(-5, 0, 1)), 1), vec2(1, 1), DayTransicionDuration);
		Pasto->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(-8, Terreno->GetHeightFromRealVector(glm::vec3(-8, 0, 1)), 1), vec2(1, 1), DayTransicionDuration);
		Arbusto->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(5, Terreno->GetHeightFromRealVector(glm::vec3(5, 0, 10))+2, 10), vec2(7, 7), DayTransicionDuration);
		Arbol->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(7, Terreno->GetHeightFromRealVector(glm::vec3(7, 0, 40))+6.5, 40), vec2(13, 13), DayTransicionDuration);

		/*Se Actualiza la posicion del collider del jugador*/
		PlayerCollider.SetPosition(position.x, position.y, position.z);

		/*Verifica las colisiones*/
		ColliderBackPos = false;
		Collider* AuxDetectColl = Colliders;
		while (AuxDetectColl != NULL)
		{
			if (intersect(PlayerCollider, AuxDetectColl)) {
				// colisión detectada
				ColliderBackPos = true;
			}
			AuxDetectColl = AuxDetectColl->sig;
		}

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

	bool SetNewPosCollider()
	{
		return ColliderBackPos;
	}

	bool intersect(Collider sphere1, Collider* sphere2) {
		float distance = glm::distance(sphere1.center, sphere2->center);
		return distance <= (sphere1.radius + sphere2->radius);
	}

	void PushCollider(Collider datos) {
		if (Colliders == NULL) {
			Colliders = new Collider();
			Colliders->ant = NULL;
			AuxColliders = Colliders;
		}
		else {
			AuxColliders->sig = new Collider();
			AuxColliders->sig->ant = new Collider();
			AuxColliders->sig->ant = AuxColliders;
			AuxColliders = AuxColliders->sig;
		}
		AuxColliders->center = datos.center;
		AuxColliders->radius = datos.radius;
		AuxColliders->sig = NULL;
	}

	~Scene()
	{
		/*Vaciamos la memoria del juego*/
		delete skydome;
		delete lights;
		delete ModeloTest;
		delete ModeloCaja;
		delete Agua;
		delete Terreno;
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
	Modelo3D* Arbol1;
	Billboards* bill1;
	Billboards* Pasto;
	Billboards* Arbusto;
	Billboards* Arbol;
	Water* Agua;
	Clouds* Nubes;
	float DayTransicionDuration = 0.0005f;
	Terrain* Terreno;
	float Y_Position = 0;
	float MovLuna = 0;
	bool ColliderBackPos = false;

	/*Collider*/
	Collider PlayerCollider;
	Collider ArbolAzulCollider;
	Collider Arbol1Collider;

	Collider* Colliders = NULL;
	Collider* AuxColliders = NULL;
};

#endif