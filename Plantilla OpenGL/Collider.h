class Collider
{
public:

	Collider()
	{
		
	}

	void InitBoxCollider(vec3 Position, vec3 SizeObject)
	{
		centerBox = Position;
		extents = SizeObject / 2.0f; //Son las dimensiones de la caja, X en el eje x (ancho), Y en el y (alto), Z en z (profundidad)
	}

	void InitCollider(vec3 Position, float Radio)
	{
		center = Position;
		radius = Radio;
	}

	void SetPosition(float Posx, float Posy, float Posz)
	{
		center.x = Posx;
		center.y = Posy;
		center.z = Posz;
	}

	void SetPositionBox(float Posx, float Posy, float Posz)
	{
		centerBox.x = Posx;
		centerBox.y = Posy;
		centerBox.z = Posz;
	}

	glm::vec3 center; // posición de la esfera en el espacio 3D
	float radius; // radio de la esfera
	glm::vec3 centerBox; // posición del centro de la caja en el espacio 3D
	glm::vec3 extents; // mitades de las dimensiones de la caja en cada eje (ancho, alto, profundidad)
	Collider* sig;
	Collider* ant;
};