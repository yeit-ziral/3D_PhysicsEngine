#pragma once
class Zombie : public ModelAnimator
{
	enum AnimState
	{
		IDLE, RUN, ATTACK
	}; 
	AnimState curState = AnimState::IDLE;
public:
	Zombie();
	~Zombie();

	void Update();
	void Render();

	void Debug();

	void UpdateLeftHand();

	void SetClip(AnimState state);

private:
	void Move();

private:

	float speed = 1.0f;
	float takeTime = 0.2f;

	//class Knife_A* weapon;
	class Bomb* weapon;


	Transform* leftHand;


	float moveSpeed = 20.0f;

	float rotSpeed = 3.0f;
};