#include "Framework.h"
#include "Zombie.h"

Zombie::Zombie()
	: ModelAnimator("Zombie")
{
	scale *= 0.35f;
	ReadClip("Happy Idle");
	ReadClip("Throw");
	ReadClip("Walking");
	CreateTexture();

	reader->GetMaterial()[0]->SetDiffuseMap(L"Model/Zombie/creature_Base_color.png");
	reader->GetMaterial()[0]->SetNormalMap(L"Model/Zombie/creature_Normal_OpenGL.png");

	weapon = new Bomb();
	//weapon->scale *= 100.0f;

	leftHand = new Transform();

	weapon->SetParent(leftHand);
}

Zombie::~Zombie()
{
	delete weapon;
	delete leftHand;
}

void Zombie::Update()
{
	ModelAnimator::Update();


	//leftHand->Update();

	if (KEY_DOWN('1'))
		PlayClip(0, speed, takeTime);

	if (KEY_DOWN('2'))
		PlayClip(1, speed, takeTime);

	if (KEY_DOWN('3'))
		PlayClip(2, speed, takeTime);

	UpdateLeftHand();
}

void Zombie::Render()
{
	ModelAnimator::Render();
	weapon->Render();
}

void Zombie::Debug()
{
	ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
	ImGui::SliderFloat("TakeTime", &takeTime, 0.0f, 1.0f);

	ModelAnimator::Debug();

	weapon->Debug();
}

void Zombie::UpdateLeftHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:LeftHand");

	Matrix nodeTransform = GetTransformByNode( nodeIndex);

	leftHand->GetWorld() = nodeTransform * world;

	weapon->Update();
}

void Zombie::SetClip(AnimState state)
{
	if (curState != state)
	{
		PlayClip( state);
		curState = state;
	}
}

void Zombie::Move()
{
	if (!KEY_PRESS(VK_LBUTTON))
	{
		if (KEY_PRESS('W'))
		{
			//Ray ray = CAMERA->ScreenPointToRay({ WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0.0f });

			//Vector2 dir = { ray.direction.x, ray.direction.z };
			//
			//float theta = -atan2f(dir.y, dir.x) * 2;

			//rotation.y = theta + (PI * 1.304f);//CAMERA->GetRotY();
			translation -= Forward() * moveSpeed * Time::Delta();
			SetClip(RUN);
		}

		if (KEY_PRESS('S'))
		{
			translation -= Backward() * moveSpeed * Time::Delta();
			SetClip(RUN);
		}
		if (KEY_UP('W') || KEY_UP('S'))
			SetClip(IDLE);

		if (KEY_PRESS('A'))
		{
			rotation.y -= rotSpeed * Time::Delta();
		}
		if (KEY_PRESS('D'))
		{
			rotation.y += rotSpeed * Time::Delta();
		}

		if (KEY_DOWN(VK_LBUTTON))
		{
			SetClip(ATTACK);
		}
	}
}
