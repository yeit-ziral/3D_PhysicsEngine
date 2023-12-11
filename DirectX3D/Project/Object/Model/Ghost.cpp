#include "Framework.h"
#include "Ghost.h"

Ghost::Ghost()
	:Model("Ghost")
{
	reader = new ModelReader("Ghost");
	GetReader()->GetMaterial()[0]->Load(L"Ghost.mat");

	rotation.y = 90.0f;

	collider = new ColliderSphere();

	collider->scale *= this->scale.x;

	translation = RandomPosition();
}

Ghost::~Ghost()
{
	delete collider;
}

void Ghost::Update()
{
	Model::Update();

	collider->Update();

	collider->translation = Model::GetGlobalPosition();

	collider->translation += {0.0f, 0.3f, 0.0f};

	nowTime += Time::Delta();

	Appear();

	if (KEY_DOWN(VK_LBUTTON))
		if (translation.x == mousePos.x && translation.y == mousePos.y)
			isAppear = false;
}

void Ghost::Render()
{
	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Ghost::Debug()
{
	Model::Debug();
}

void Ghost::Appear()
{
	if (nowTime / time > 1)
	{
		nowTime = 0;

		if (!isAppear)
		{
			isAppear = true;
		}
		else
		{
			isAppear = false;

			translation = RandomPosition();
		}
	}
	else
	{
		if (isAppear)
			Move();
	}
}

void Ghost::Move()
{
	if (nowTime < 1.5f)
		translation.x += Time::Delta();
	else
		translation.x -= Time::Delta();
}

Vector3 Ghost::RandomPosition()
{
	Vector3 newPos;

	newPos = CAMERA->GetGlobalPosition();

	newPos.z = 0;

	newPos.x += RandomNum(-2, 2);
	newPos.y += RandomNum(-2, 2);
	newPos.z += RandomNum(-2, 2);

	return newPos;
}
