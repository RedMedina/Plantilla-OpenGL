#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>



struct Vertices {
	float Posx, Posy, Posz;
	float Normx, Normy, Normz;
	float u, v;
};

struct Maya {
	Vertices* maya;
	GLuint* indices;
};

class Geometria
{
public:
	Geometria() //mis pensamientos!
	{}

	Maya Esfera(GLuint stacks, GLuint slices, float radio, float inicio, float final)
	{
		Vertices* verticesxyzSD = new Vertices[stacks * slices * 3];
		GLuint* indices = new GLuint[stacks * slices * 6];
		Maya salida;

		for (int i = 0; i < slices; i++)
		{
			for (int j = 0; j < stacks; j++)
			{
				int indice = (i * stacks + j);
				verticesxyzSD[indice].Posx = radio * cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					cos(2.0 * M_PI * (double)i / (slices - 1));
				verticesxyzSD[indice].Posy = radio * sin(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0);
				verticesxyzSD[indice].Posz = radio * cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					sin(2.0 * M_PI * (double)i / (slices - 1));

				verticesxyzSD[indice].Normx = cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					cos(2.0 * M_PI * (double)i / (slices - 1));
				verticesxyzSD[indice].Normy = sin(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0);
				verticesxyzSD[indice].Normz = cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					sin(2.0 * M_PI * (double)i / (slices - 1));

				verticesxyzSD[indice].u = (float)1 * (1 - (float)i / (stacks - 1));
				verticesxyzSD[indice].v = (float)1 * (1 - (float)j / (slices - 1));
			}
		}

		int indice = 0;
		for (int i = 0; i < slices - 1; i++)
		{
			for (int j = 0; j < stacks - 1; j++)
			{
				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
				indices[indice++] = i * stacks + j + 1;

				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
			}
		}
		salida.maya = verticesxyzSD;
		salida.indices = indices;

		return salida;
	}
};
