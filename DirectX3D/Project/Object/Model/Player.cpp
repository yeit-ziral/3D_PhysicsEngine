#include "Framework.h"
#include "Player.h"

Player::Player()
	:ModelAnimator("Demoman")
{
	scale *= 0.04f;
	ReadClip("Neutral Idle");
	ReadClip("Running");
	ReadClip("Throw");
	CreateTexture();

	reader->GetMaterial()[0]->Load(L"Demoman.mat");

	weapon = new Model("Dwarven_Axe");
	weapon->GetReader()->GetMaterial()[0]->Load(L"Dwarven_Axe.mat");
	//weapon->scale *= 100.0f;

	leftHand = new Transform();

	weapon->SetParent(leftHand);

	weapon->rotation.x = XMConvertToRadians(-44);
	weapon->rotation.y = XMConvertToRadians(64);
	weapon->rotation.z = XMConvertToRadians(244);

	weapon->translation.x = -1;
	weapon->translation.y = 80;
	weapon->translation.z = -20;

	clips[ATTACK]->SetEndEvent(0.7f, bind(&Player::SetClip, this, IDLE)); //bind(위치, 작동시킬 객체, (매개변수를 직접 넣어줘서 매개변수가 없는 함수처럼 사용 가능 아니면 placeholders::_10이용해서 매개변수 사용))

	hpBar = new ProgressBar(L"UI/hp_bar.png", L"UI/hp_bar_BG.png");
	hpBar->SetLabel("HP Bar");
	hpBar->scale.x *= 0.5f;
}

Player::~Player()
{
	delete weapon;
	delete leftHand;

	delete hpBar;
}

void Player::Update()
{
	ModelAnimator::Update();


	//leftHand->Update();

	if (KEY_DOWN('1'))
		PlayClip(0, speed, takeTime);

	if (KEY_DOWN('2'))
		PlayClip(1, speed, takeTime);

	if (KEY_DOWN('3'))
		PlayClip(2, speed, takeTime);

	hpBar->Update();
	hpBar->translation = CAMERA->WorldToScreenPoint(this->globalPosition + V_UP);

	UpdateLeftHand();

	Move();
}

void Player::Render()
{
	ModelAnimator::Render();
	weapon->Render();
}

void Player::Debug()
{
	ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
	ImGui::SliderFloat("TakeTime", &takeTime, 0.0f, 1.0f);

	ModelAnimator::Debug();

	weapon->Debug();

	Transform::Debug();

	label = "Groot";

	static float value = 1.0f;

	hpBar->SetValue(value);

	ImGui::SliderFloat("HP", &value, 0.0f, 1.0f);
}

void Player::PostRender()
{
	Debug();
	hpBar->Render();
}

void Player::UpdateLeftHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:LeftHand");

	Matrix nodeTransform = GetTransformByNode(nodeIndex); // 하나만 들어와서 instanceIndex에 0 넣어줌

	leftHand->GetWorld() = nodeTransform * world;

	weapon->Update();
}

void Player::SetClip(AnimState state)
{
	if (curState != state)
	{
		PlayClip(state);  // 하나만 들어와서 instanceIndex에 0 넣어줌
		curState = state;
	}
}

void Player::Move()
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
