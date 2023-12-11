#include "Framework.h"
#include "Bomb2.h"

Bomb2::Bomb2()
	:Model("Dynamite")
{
	GetReader()->GetMaterial()[0]->Load(L"Bomb.mat");

	scale *= 0.1f;

	rotation.x += XM_PIDIV2;

	collider = new ColliderSphere();

	collider->SetParent(this);

	exp = new Explosion();
}

Bomb2::~Bomb2()
{
	delete collider;
	delete exp;
}

void Bomb2::Update()
{
	Model::Update();
	collider->Update();

	if (isExist)
	{
		nowTime += Time::Delta();

		if (isAppear)
		{
			translation += direction * speed * Time::Delta();

			direction.y -= 0.5f * Time::Delta();

			if (isGround)
				direction.y = 0;

			if (nowTime > 3.0f)
			{
				nowTime = 0.0f;
				exp->translation = this->GetGlobalPosition();
				Explode();
				translation = CAMERA->GetGlobalPosition();
				speed = 0.0f;
				isAppear = false;
				isExist = false;
				isGround = false;
			}
		}
	}

	exp->Update();
}

void Bomb2::Render()
{
	exp->Render();

	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Bomb2::Debug()
{
	Model::Debug();

	exp->Debug();
}

void Bomb2::Throw()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	direction = ray.direction.GetNormalized();

	isAppear = true;
}

void Bomb2::Charging()
{
	if (speed < 1000)
		speed += Time::Delta() * 50;

	isExist = true;
}

void Bomb2::Explode()
{
	exp->SetExplode(true);
}
