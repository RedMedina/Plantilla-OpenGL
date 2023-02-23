#include <iostream>
#include <openGL/glut.h>
#include <GLFW/glfw3.h>

using namespace std;

class Lights
{
public:
	void InitLights()
	{
		glEnable(GL_LIGHTING);

		GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
		GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
		GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
		GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glEnable(GL_LIGHT0);

		// Configurar la propiedad de material
		GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
		GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
		GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat material_shininess[] = { 100.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);


		/*Revisa si se inicializó el objeto*/
		cout << "Luz Inicializada" << endl;
	}
};
