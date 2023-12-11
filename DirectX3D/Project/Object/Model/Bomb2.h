#pragma once
class Bomb2 : public Model
{
public:
	Bomb2();
	~Bomb2();

	void Update();
	void Render();

	void Debug();

	void Throw();

	ColliderSphere* GetCollider() { return collider; }

	void SetIsAppear(bool value) { isAppear = value; }

	bool IsAppear() { return isAppear; }

	void Charging();

	ColliderSphere* GetExpCollider() { return exp->GetCollider(); }

	void Explode();

	void SetCollision(bool value) { isCollision = value; }

	void SetGround(bool value) { isGround = value; }

	bool IsExist() { return isExist; }

private:
	ColliderSphere* collider;

	int time = 3;

	float speed = 5;

	float nowTime = 0;

	bool isAppear = false;

	Vector3 direction;

	Explosion* exp;

	bool isCollision = false;

	bool isExist = false;

	bool isGround = false;
};