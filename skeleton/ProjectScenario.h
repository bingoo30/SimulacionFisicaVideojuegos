#pragma once
#include "Scene.h"
class ProjectScenario: public Scene{
public:
#pragma region constructora e inicializacion
	ProjectScenario() = default;
	void init() override;
#pragma endregion
#pragma region metodos publicos
	void render_interface() override;
	void handle_input(unsigned char key) override;
#pragma endregion
protected:
#pragma region atributos
	std::string explication_tramps;
	std::string instructions;
#pragma endregion
private:
	void create_demo_platforms(Platform_Data& pd, Fire_Particle_Data& fpd, const Fire_Deviation_Data& fdd, double offset, int count);
};