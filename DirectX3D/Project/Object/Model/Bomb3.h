#pragma once
class Bomb3 : public Model
{
public:
	Bomb3();
	~Bomb3();

	void Update();
	void Render();

	void Debug();

	void Throw();

	ColliderSphere* GetCollider() { return collider; }

	void SetIsAppear(bool value) { isAppear = value; }

	bool IsAppear() { return isAppear; }

	ColliderSphere* GetExpCollider() { return exp->GetCollider(); }

	void Explode();

	bool IsCollisionBox(ColliderBox* value) { return collider->Collision(value); }
	bool IsCollisionSphere(ColliderSphere* value) { return collider->Collision(value); }

	void SetCollision(bool value) { isCollision = value; }

	void Charging();

	void SetGround(bool value) { isGround = value; }

	bool IsExist() { return isExist; }

private:
	ColliderSphere* collider;

	float speed = 0;

	bool isAppear = false;

	Vector3 direction;

	Explosion* exp;

	bool isCollision = false;

	bool isExist = false;

	bool isGround = false;
};