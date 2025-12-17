#pragma once
#include "Scene.h"
#include "StructForEntities.h"
class SelectionLevel :
    public Scene
{
public:
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
	int selectedLevel = 0;
#pragma endregion
};

