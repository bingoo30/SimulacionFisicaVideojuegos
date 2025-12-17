#pragma once
#include "Scene.h"
class Intro: public Scene{
public:
#pragma region constructora e inicializacion
	Intro() = default;
	void init() override;
#pragma endregion
#pragma region metodos publicos
	void render_interface() override;
	void handle_input(unsigned char key) override;
#pragma endregion
protected:
#pragma region atributos
	std::string title;
	std::string subtitle;
#pragma endregion
};