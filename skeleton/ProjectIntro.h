#pragma once
#include "Scene.h"
class ProjectIntro: public Scene{
public:
#pragma region constructora e inicializacion
	ProjectIntro() = default;
	void init() override;
#pragma endregion
#pragma region metodos publicos
	void update(double t) override;
	void render_interface() override;
	void handle_input(unsigned char key) override;
#pragma endregion
protected:
#pragma region atributos
	std::string title;
	std::string subtitle;
#pragma endregion
};