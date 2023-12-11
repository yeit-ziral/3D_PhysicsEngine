#pragma once
class Pumpkin : public Model
{
public:
	Pumpkin();
	~Pumpkin();

	void Update();
	void Render();

	void Debug();

	void Throw();

	//void Appear();

	//void Move();

	//int RandomNum(int min, int max) { return rand() % (max - min) + min; }

	//Vector3 RandomPosition();

	ColliderSphere* GetCollider() { return collider; }

	void SetIsAppear(bool value) { isAppear = value; }

	bool IsAppear() { return isAppear; }

private:
	ColliderSphere* collider;

	int speed = 0;

	int time = 3;

	float nowTime = 0;

	bool isAppear = true;

	Vector3 direction;
};