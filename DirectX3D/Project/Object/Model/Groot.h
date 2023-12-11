#pragma once
class Groot : public ModelAnimator
{
	enum AnimState
	{
		IDLE, RUN, ATTACK
	} curState = IDLE;

public:
	Groot();
	~Groot();

	void Update();
	void Render();

	void Debug();
	void PostRender();

	void UpdateLeftHand();
	void UpdateRightHand();
	void UpdateNeck();

	void SetClip(AnimState state);

	bool GetBombIsExist();

	Vector3 GetBombGPos();

	void BombIsGround(bool value);

	ColliderSphere* GetCurBombCollider();

	void SetBombPos(Vector3 value);

private:
	void Move();

	void Throw();

private:

	float speed = 1.0f;
	float takeTime = 0.2f;

	Model* weapon;

	Transform* leftHand;

	float moveSpeed = 20.0f;

	float rotSpeed = 3.0f;

	ProgressBar* hpBar;

	int curBomb = 0; // 0일때 Bomb, 1일때 Bomb2, 2일때 Bomb3

	class Bomb*  bomb;
	class Bomb2* bomb2;
	class Bomb3* bomb3;
};