#include "Framework.h"
#include "Bomb3.h"

Bomb3::Bomb3()
	:Model("Grenade")
{
	GetReader()->GetMaterial()[0]->Load(L"Bomb.mat");

	scale *= 5.0f;

	rotation.x += XM_PIDIV2;

	collider = new ColliderSphere();

	collider->SetParent(this);

	collider->scale *= 0.06f;

	exp = new Explosion();
}

Bomb3::~Bomb3()
{
	delete collider;
	delete exp;
}

void Bomb3::Update()
{
	Model::Update();
	collider->Update();

	if (isExist)
	{
		if (isAppear)
		{
			translation += direction * speed * Time::Delta();

			Vector3 temp = this->GetGlobalPosition();

			direction.y -= 0.5f * Time::Delta();

			if (isCollision || isGround)
			{
				exp->translation = this->GetGlobalPosition();
				Explode();
				translation = CAMERA->GetGlobalPosition();
				speed = 0.0f;
				isAppear = false;

				isGround = false;
			}
		}
	}

	exp->Update();
}

void Bomb3::Render()
{
	exp->Render();

	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Bomb3::Debug()
{
	Model::Debug();

	exp->Debug();
}

void Bomb3::Throw()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	direction = ray.direction.GetNormalized();

	isAppear = true;
}

void Bomb3::Explode()
{
	exp->SetExplode(true);
}

void Bomb3::Charging()
{
	if (speed < 1000)
		speed += Time::Delta() * 50;

	isExist = true;
}
