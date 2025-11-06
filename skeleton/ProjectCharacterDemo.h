#pragma once
#include "Scene.h"
#include "Character.h"
class ProjectCharacterDemo: public Scene{
public:
#pragma region constructora e inicializacion
	ProjectCharacterDemo() = default;
	void init() override;
#pragma endregion
#pragma region metodos publicos
	void render_interface() override;
	void handle_input(unsigned char key) override;
	void handle_special_input(int key) override;  //para las flechas
	void handle_key_up(unsigned char key) override; //para soltar SPACE
#pragma endregion
protected:
#pragma region atributos
	std::string explication_inputs;
	std::string explication_forces;

	Character* character;
#pragma endregion
private:
	void create_character();
};