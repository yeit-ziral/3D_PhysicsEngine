#include "Framework.h"
#include "Skeleton.h"

Skeleton::Skeleton()
	: Model("Skeleton")
{
	reader = new ModelReader("Skeleton");
	GetReader()->GetMaterial()[0]->Load(L"Skeleton.mat");

	scale *= 2;

	collider = new ColliderSphere();

	collider->scale *= this->scale.x * 0.4f;

	translation = RandomPosition();
}

Skeleton::~Skeleton()
{
	delete collider;
}

void Skeleton::Update()
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

void Skeleton::Render()
{
	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Skeleton::Debug()
{
	Model::Debug();
}

void Skeleton::Appear()
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

void Skeleton::Move()
{
	if (nowTime < 1.5f)
		translation.x += Time::Delta();
	else
		translation.x -= Time::Delta();
}

Vector3 Skeleton::RandomPosition()
{
	Vector3 newPos;

	newPos = CAMERA->GetGlobalPosition();

	newPos.z = 0;

	newPos.x += RandomNum(-2, 2);
	newPos.y += RandomNum(-2, 2);
	newPos.z += RandomNum(-2, 2);

	return newPos;
}
