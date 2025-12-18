#pragma once
#include "Scene.h"
class GameOver : public Scene {
public:
#pragma region constructora e inicializacion
	GameOver() = default;
	void init() override;
#pragma endregion
#pragma region metodos publicos
	void render_interface() override;
	void handle_input(unsigned char key) override;
#pragma endregion
protected:
#pragma region atributos
	std::string title;
	std::string subtitle1;
	std::string subtitle2;
#pragma endregion
};

