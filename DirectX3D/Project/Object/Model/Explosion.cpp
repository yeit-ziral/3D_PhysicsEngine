#include "Framework.h"
#include "Explosion.h"

Explosion::Explosion()
	:Model("Explosion_S")
{
	rotation.x = XM_PIDIV2;

	scale *= 0.0f;

	collider = new ColliderSphere();

	collider->scale *= 0.0f;

	collider->SetParent(this);
}

Explosion::~Explosion()
{
	delete collider;
}

void Explosion::Update()
{
	if (isExplode)
	{
		Model::Update();

		if (this->GetGlobalScale().x < 0.2f)
			scale += { Time::Delta() * 0.5f, Time::Delta() * 0.5f, Time::Delta() * 0.5f };


		collider->Update();


		if (collider->GetGlobalScale().x < 10)
			collider->scale = scale * 300;

		nowTime += Time::Delta();

		if (nowTime > 3.0f)
		{
			nowTime = 0.0f;

			scale *= 0.0f;

			collider->scale *= 0.0f;

			isExplode = false;
		}
	}
}

void Explosion::Render()
{
	if (isExplode)
	{
		Model::Render();

		collider->Render();
	}
}

void Explosion::Debug()
{
	Model::Debug();

	collider->Render();
}
