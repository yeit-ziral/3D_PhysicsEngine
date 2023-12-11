#pragma once
class Explosion : public Model
{
public:
	Explosion();
	~Explosion();

	void Update();
	void Render();
	void Debug();

	ColliderSphere* GetCollider() { return collider; }

	void SetExplode(bool value) { isExplode = value; }

private:
	//Model* exp;

	ColliderSphere* collider;

	float nowTime = 0;

	bool isExplode = false;
};