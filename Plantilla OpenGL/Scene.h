#ifndef __Scene
#define __Scene

#include "Modelo3D.h"
#include "Music.h"
#include "Water.h"
#include "Billboards.h"
#include "skybox.h"
#include "Clouds.h"
#include "Terrain.h"
#include "Grass.h"
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
		skydome = new Skybox(500, "Skybox.vertex", "Skybox.fragment", "Assets/Skydome/Dia.png", "Assets/Skydome/Atardecer.png", "Assets/Skydome/SkyBoxNcche2.png");
		/*Nubes*/
		Nubes = new Clouds("Clouds.vertex", "Clouds.fragment", "Assets/Skydome/nube1.png");

		/*Modelos 3D*/
		ModeloTest = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/ArbolAzul/Arbol.obj", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolTextura.bmp", "Assets/Modelos/ArbolAzul/ArbolN.bmp", 10);
		ModeloCaja = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/Cubo/cube.obj", "Assets/Modelos/Cubo/cubeText.bmp", "Assets/Modelos/Cubo/cubeText.bmp", "Assets/Modelos/Cubo/normal.bmp", 1);
		Arbol1 = new Modelo3D("NormalMapping.vertexshader", "NormalMapping.fragmentshader", "Assets/Modelos/Arbol/Arbol1.obj", "Assets/Modelos/Arbol/TexturaArbol.png", "Assets/Modelos/Arbol/TexturaArbol.png", "Assets/Modelos/Arbol/ArbolNormales.png", 3);

		/*Agua*/
		Agua = new Water("WaterVertex.vertexhader", "WaterFragment.fragmentshader", "Assets/Agua/AguaTextura3.png", "Assets/Agua/AguaN1.jpg", "Assets/Agua/AguaN2.jpg", 50, 50);

		/*Billboards*/
		bill1 = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/grass2.png", "Assets/Terreno/Blend3.png");
		Pasto = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/grass7.png", "Assets/Terreno/Blend3.png");
		Arbusto = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/arbusto.png", "Assets/Terreno/Blend3.png");
		Arbol = new Billboards("billboard.vertex", "billboard.fragment", "Assets/Billboards/Arbol.png", "Assets/Terreno/Blend3.png");

		/*Terreno*/
		Terreno = new Terrain("Terrain.vertex", "Terrain.fragment", "Assets/Terreno/mon.png", "Assets/Terreno/monN.png", "Assets/Terreno/grass.png", "Assets/Terreno/grassN.png", "Assets/Terreno/octostoneAlbedo.png", "Assets/Terreno/CaminoNormal2.png", "Assets/Terreno/Blendmap3.png", "Assets/Terreno/Heightmap3.png");

		/*Pasto*/
		Pastoso = new Grass("Grass.vertex", "Grass.fragment", "Assets/Grass/Grass.png");

		/*Colliders*/
		PlayerCollider.InitBoxCollider(vec3(0, 0, 5), vec3(5, 25, 5));
		//PlayerCollider.InitCollider(vec3(0, 0, 5), 10);
	    Arbol1Collider.InitBoxCollider(vec3(10, Terreno->GetHeightFromRealVector(glm::vec3(10, 0, 3)), 3), vec3(1, 15, 1));
		//Arbol1Collider.InitCollider(vec3(10, Terreno->GetHeightFromRealVector(glm::vec3(10, 0, 3)), 3), 10);
		PushCollider(Arbol1Collider);
	}

	void Render(GLFWwindow* window, glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ModelMatrix, glm::mat3 ModelView3x3Matrix, glm::mat4 ProjectionMatrix, glm::vec3 position)
	{

		/*Aqui se ejecutara todo el juego*/

		// Borra el buffer de color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*Carga el skydome*/
		skydome->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, window, DayTransicionDuration);
		Nubes->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, DayTransicionDuration);

		/*Carga los modelos*/                                                              //Posicion                                                          Escala     Rotacion
		//ModeloTest->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(5, Terreno->GetHeightFromRealVector(glm::vec3(5, 0, 1)), 1), vec3(1,1,1),  0, DayTransicionDuration);

		/*Modelos para desbugear el Terreno no preguntes, solo deja esto como est? xd*/
		ModeloCaja->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(1, -200, 10), vec3(1, 1, 1), 0, DayTransicionDuration);
		/*Carga el Terreno*/
		Terreno->SetRenderSize(800.0f, 250, 800.0f);
		Terreno->Render(MVP, ViewMatrix, ProjectionMatrix, ModelView3x3Matrix, ModelMatrix, DayTransicionDuration, position);

		/*Carga el agua*/
		Agua->Draw(MVP, init, ViewMatrix, ProjectionMatrix, position, ModelMatrix, vec3(0, Terreno->GetHeightFromRealVector(glm::vec3(0, 0, 0)) - 3.5, 0), vec3(30, 30, 30));

		/*Se coloca en la varible Y_Position la posci?n del Y del terreno apartir de la posici?n de la camara*/
		Y_Position = Terreno->GetHeightFromRealVector(position) + 3.0f;

		Arbol1->Draw(MVP, ViewMatrix, ModelMatrix, ModelView3x3Matrix, ProjectionMatrix, vec3(10, Terreno->GetHeightFromRealVector(glm::vec3(10, 0, 3)), 3), vec3(1, 1, 1), 0, DayTransicionDuration);
		/*Carga los billboards*/                              //Posicion  Esta funci?n me dice que altura es la del terreno en esas coords Escala
																   //X                            Y                            Z
		bill1->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(-5, Terreno->GetHeightFromRealVector(glm::vec3(-5, 0, 1)) + 3, 1), vec2(1, 1), DayTransicionDuration);

		Pasto->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(0.2, Terreno->GetHeightFromRealVector(glm::vec3(0.2, 0, 1)) + 0.2, 1), vec2(1, 1), DayTransicionDuration);

		Arbusto->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(5, Terreno->GetHeightFromRealVector(glm::vec3(5, 0, 10)) + 2, 10), vec2(7, 7), DayTransicionDuration);
		Arbol->Draw(ViewMatrix, ProjectionMatrix, ModelMatrix, vec3(7, Terreno->GetHeightFromRealVector(glm::vec3(7, 0, 40)) + 6.5, 40), vec2(13, 13), DayTransicionDuration);

		/*Pasto*/
		Pastoso->Draw(MVP, ViewMatrix, ModelMatrix, ProjectionMatrix, DayTransicionDuration, vec3(15, Terreno->GetHeightFromRealVector(glm::vec3(15, 0, -10)) - 0.2, -10));

		/*Se Actualiza la posicion del collider del jugador*/
		//PlayerCollider.SetPosition(position.x, position.y, position.z);
		PlayerCollider.SetPositionBox(position.x, position.y, position.z);

		/*Verifica las colisiones*/
		ColliderBackPos = false;
		Collider* AuxDetectColl = Colliders;
		while (AuxDetectColl != NULL)
		{
			if (intersectBox(PlayerCollider, AuxDetectColl)) {
				// colisi?n detectada
				ColliderBackPos = true;
			}
			AuxDetectColl = AuxDetectColl->sig;
		}

		/*Revisa si se dibuj? el objeto*/
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


	bool intersectBox(Collider box1, Collider* box2) {
		glm::vec3 box1Min = box1.centerBox - box1.extents;
		glm::vec3 box1Max = box1.centerBox + box1.extents;
		glm::vec3 box2Min = box2->centerBox - box2->extents;
		glm::vec3 box2Max = box2->centerBox + box2->extents;

		return (box1Min.x <= box2Max.x && box1Max.x >= box2Min.x) &&
			(box1Min.y <= box2Max.y && box1Max.y >= box2Min.y) &&
			(box1Min.z <= box2Max.z && box1Max.z >= box2Min.z);
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
		AuxColliders->centerBox = datos.centerBox;
		AuxColliders->radius = datos.radius;
		AuxColliders->extents = datos.extents;
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
	Grass* Pastoso;
	float DayTransicionDuration = 0.00005f;
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