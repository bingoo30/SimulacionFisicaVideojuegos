#pragma once
#include "Scene.h"
class CharacterRBSystem;
class Level :
    public Scene
{
public:
	enum types
	{
		GROUND, DOOR, KEY, CHARACTER, TRAMP, FORCE, NUM_TYPES
	};
#pragma region constructora e inicializacion
	Level(int i);
	void init() override; 
#pragma endregion
#pragma region metodos publicos
	void handle_special_input(int key) override;
	void handle_special_key_up(int key) override;
	void handle_input(unsigned char key) override;
	void handle_key_up(unsigned char key) override;
	void update(double dt) override;
#pragma endregion
protected:
#pragma region atributos
	std::string file;
	CharacterRBSystem* character = nullptr;

	physx::PxFilterData groundFilterData;
	physx::PxFilterData trampFilterData;
	physx::PxFilterData playerFilterData;
	physx::PxFilterData keyFilterData;
	physx::PxFilterData doorFilterData;
#pragma endregion
};

