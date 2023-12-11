#pragma once
class Crate : public Model
{
public:
	Crate();
	~Crate();

	void Update();
	void Render();

	void Debug();

	void Move();

	ColliderBox* GetCollider() { return collider; }

	void SetIsAppear(bool value) { isAppear = value; }

private:
	ModelReader* reader;

	ColliderBox* collider;

	int speed = 1;

	float time = 3.0f;

	float nowTime = 0.0f;

	bool isAppear = true;
};