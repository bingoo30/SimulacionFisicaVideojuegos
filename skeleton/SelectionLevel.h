#pragma once
#include "Scene.h"
class SelectionLevel :
    public Scene
{
#pragma region constructora e inicializacion
	SelectionLevel() = default;
	void init() override;
#pragma endregion
#pragma region metodos publicos
	void render_interface() override;
	void handle_special_input(int key) override;
	void handle_input(unsigned char key) override;
#pragma endregion
protected:
#pragma region atributos
	std::string title;
	std::string subtitle;
	int firstLevel = States::LEVEL1;
	int lastLevel = States::LEVEL2;
	int selectedLevel = States::LEVEL1;
#pragma endregion
};

