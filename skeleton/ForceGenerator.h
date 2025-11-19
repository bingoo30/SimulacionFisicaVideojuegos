#pragma once
#include "core.hpp"
class Particle;
class ForceGenerator
{
public:
	ForceGenerator();
	virtual ~ForceGenerator() = default;
	virtual void update_force(Particle* particle, double dt);
	virtual void handle_special_input(int key);
	virtual void handle_input(unsigned char key);
	bool is_alive() const;
	void set_alive(bool a);
protected:
	virtual physx::PxVec3 calculate_force(Particle* p, double dt) = 0;
	bool alive;
};

