#pragma once
#include "Scene.h"
class Victory :
    public Scene
{
public:
#pragma region constructora e inicializacion
	Victory() = default;
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

