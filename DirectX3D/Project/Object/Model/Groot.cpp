#include "Framework.h"
#include "Groot.h"

Groot::Groot()
	:ModelAnimator("Groot")
{
	scale *= 0.04f;
	ReadClip("Sad Idle");
	ReadClip("Drunk Run Forward");
	ReadClip("Throw");
	CreateTexture();

	reader->GetMaterial()[0]->Load(L"Groot.mat");

	weapon = new Model("Bomb");
	weapon->GetReader()->GetMaterial()[0]->Load(L"Bomb.mat");
	//weapon->scale *= 100.0f;

	leftHand = new Transform();

	weapon->SetParent(leftHand);

	weapon->rotation.x = XMConvertToRadians(-44);
	weapon->rotation.y = XMConvertToRadians(64);
	weapon->rotation.z = XMConvertToRadians(244);

	weapon->translation.x = -1;
	weapon->translation.y = 80;
	weapon->translation.z = -20;

	clips[ATTACK]->SetEndEvent(0.7f, bind(&Groot::SetClip, this, IDLE)); //bind(위치, 작동시킬 객체, (매개변수를 직접 넣어줘서 매개변수가 없는 함수처럼 사용 가능 아니면 placeholders::_10이용해서 매개변수 사용))

	hpBar = new ProgressBar(L"UI/hp_bar.png", L"UI/hp_bar_BG.png");
	hpBar->SetLabel("HP Bar");
	hpBar->scale.x *= 0.5f;

	weapon->scale *= 50.0f;

	bomb  = new Bomb();
	bomb2 = new Bomb2();
	bomb3 = new Bomb3();

	bomb3->translation = this->GetGlobalPosition() + Vector3(0, 2, 0);
}

Groot::~Groot()
{
	delete weapon;
	delete leftHand;

	delete hpBar;

	delete bomb ;
	delete bomb2;
	delete bomb3;
}

void Groot::Update()
{
	ModelAnimator::Update();


	//leftHand->Update();

	//if (KEY_DOWN('1'))
	//	PlayClip( 0, speed, takeTime);

	//if (KEY_DOWN('2'))
	//	PlayClip( 1, speed, takeTime);

	//if (KEY_DOWN('3'))
	//	PlayClip( 2, speed, takeTime);

	if (KEY_DOWN('1'))
		curBomb = 0;
	if (KEY_DOWN('2'))
		curBomb = 1;
	if (KEY_DOWN('3'))
		curBomb = 2;

	hpBar->Update();
	hpBar->translation = CAMERA->WorldToScreenPoint(this->globalPosition + V_UP);

	//UpdateLeftHand();
	UpdateRightHand();

	Move();

	bomb ->Update();
	bomb2->Update();
	bomb3->Update();

	Throw();
}

void Groot::Render()
{
	//ModelAnimator::Render();
	weapon->Render();
	bomb->Render();
	bomb2->Render();
	bomb3->Render();
}

void Groot::Debug()
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

	ImGui::SliderInt("BombState", &curBomb, 0, 2);

	bomb->Debug();
}

void Groot::PostRender()
{
	Debug();
	hpBar->Render();
}

void Groot::UpdateLeftHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:LeftHand");

	Matrix nodeTransform = GetTransformByNode(nodeIndex); // 하나만 들어와서 instanceIndex에 0 넣어줌

	leftHand->GetWorld() = nodeTransform * world;

	weapon->Update();
}

void Groot::UpdateRightHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:RightHand");

	Matrix nodeTransform = GetTransformByNode(nodeIndex); // 하나만 들어와서 instanceIndex에 0 넣어줌

	leftHand->GetWorld() = nodeTransform * world;

	weapon->Update();
}

void Groot::UpdateNeck()
{
}

void Groot::SetClip(AnimState state)
{
	if (curState != state)
	{
		PlayClip(state);  // 하나만 들어와서 instanceIndex에 0 넣어줌
		curState = state;
	}
}

bool Groot::GetBombIsExist()
{
	if (curBomb == 0)
	{
		return bomb->IsExist();
	}

	if (curBomb == 1)
	{
		return bomb2->IsExist();
	}

	if (curBomb == 2)
	{
		return bomb3->IsExist();
	}
}

Vector3 Groot::GetBombGPos()
{
	if (curBomb == 0)
	{
		return bomb->GetGlobalPosition();
	}

	if (curBomb == 1)
	{
		return bomb2->GetGlobalPosition();
	}

	if (curBomb == 2)
	{
		return bomb3->GetGlobalPosition();
	}
}

void Groot::BombIsGround(bool value)
{
	if (curBomb == 0)
	{
		bomb->SetGround(value);
	}

	if (curBomb == 1)
	{
		bomb2->SetGround(value);
	}

	if (curBomb == 2)
	{
		bomb3->SetGround(value);
	}
}

ColliderSphere* Groot::GetCurBombCollider()
{
	if (curBomb == 0)
	{
		return bomb->GetCollider();
	}

	if (curBomb == 1)
	{
		return bomb2->GetCollider();
	}

	if (curBomb == 2)
	{
		return bomb3->GetCollider();
	}
}

void Groot::SetBombPos(Vector3 value)
{
	if (curBomb == 0)
	{
		bomb->translation = value;
	}

	if (curBomb == 1)
	{
		bomb2->translation = value;
	}

	if (curBomb == 2)
	{
		bomb3->translation = value;
	}
}

void Groot::Move()
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

void Groot::Throw()
{
	if (curBomb == 0)
	{
		if (KEY_PRESS(VK_LBUTTON) && !bomb->IsAppear())
		{
			bomb->Charging();
		}

		else if (KEY_UP(VK_LBUTTON) && !bomb->IsAppear())
			bomb->Throw();
	}

	if (curBomb == 1)
	{
		if (KEY_PRESS(VK_LBUTTON))
		{
			bomb2->Charging();
		}

		else if (KEY_UP(VK_LBUTTON) && !bomb2->IsAppear())
			bomb2->Throw();
	}

	if (curBomb == 2)
	{
		if (KEY_PRESS(VK_LBUTTON))
		{
			bomb3->translation = Vector3(0, 3, 0);
			bomb3->Charging();
		}

		else if (KEY_UP(VK_LBUTTON) && !bomb3->IsAppear())
		{
			bomb3->SetGround(false);
			bomb3->Throw();
		}
	}
}
