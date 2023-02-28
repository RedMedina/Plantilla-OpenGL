#include <fmod.hpp>
#include <fmod.h>

class Music
{
public:

	Music(const char* mp3)
	{
		// Crear una instancia del sistema FMOD
		result = FMOD::System_Create(&system);
		if (result != FMOD_OK) {
			std::cout << "Error al crear el sistema FMOD: " << result << std::endl;
		}
		// Inicializar el sistema FMOD
		result = system->init(32, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK) {
			std::cout << "Error al inicializar el sistema FMOD: " << result << std::endl;
		}

		// Cargar el archivo de audio
		result = system->createSound(mp3, FMOD_DEFAULT, 0, &sound);
		if (result != FMOD_OK) {
			std::cout << "Error al cargar el archivo de audio: " << result << std::endl;
		}

		
	}

	void Play()
	{
		// Reproducir el archivo de audio
		result = system->playSound(sound, 0, false, &channel);
		if (result != FMOD_OK) {
			std::cout << "Error al reproducir el archivo de audio: " << result << std::endl;
		}
	}

	void Playback()
	{
		if(!isPlaying)
		{
			system->playSound(sound, 0, false, &channel);
			isPlaying = true;
		}
	}

	void Update(float volumen)
	{
		channel->isPlaying(&isPlaying);
		channel->setVolume(volumen);
		system->update();
		
	}

	void Stop()
	{
		if(isPlaying)
		{
			isPlaying = false;
			channel->stop();
		}
	}

	~Music()
	{
		// Liberar recursos
		sound->release();
		system->close();
		system->release();
	}

private:
	FMOD::System* system;
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD_RESULT result;
	bool isPlaying=true;
};
