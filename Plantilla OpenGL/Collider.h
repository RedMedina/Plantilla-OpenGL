class Collider
{
public:

	Collider()
	{
		
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

	glm::vec3 center; // posición de la esfera en el espacio 3D
	float radius; // radio de la esfera
	Collider* sig;
	Collider* ant;
};