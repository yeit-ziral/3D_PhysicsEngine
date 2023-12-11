#pragma once
class Skeleton : public Model
{
public:
	Skeleton();
	~Skeleton();

	void Update();
	void Render();

	void Debug();

	void Appear();

	void Move();

	int RandomNum(int min, int max) { return rand() % (max - min) + min; }

	Vector3 RandomPosition();

	ColliderSphere* GetCollider() { return collider; }

	void SetIsAppear(bool value) { isAppear = value; }

private:
	ModelReader* reader;

	ColliderSphere* collider;

	int speed = 1;

	float time = 3.0f;

	float nowTime = 0.0f;

	bool isAppear = false;
};