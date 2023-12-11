#include "Framework.h"
#include "Bomb.h"

Bomb::Bomb()
	:Model("Bomb")
{
	GetReader()->GetMaterial()[0]->Load(L"Bomb.mat");

	scale *= 1.0f;

	rotation.x += XM_PIDIV2;

	collider = new ColliderSphere();

	collider->SetParent(this);

	exp = new Explosion();
}

Bomb::~Bomb()
{
	delete collider;
	delete exp;
}

void Bomb::Update()
{
	Model::Update();
	collider->Update();

	//collider->translation = Model::GetGlobalPosition();
	//collider->translation += {0.0f, 0.4f, 0.0f};

	//collider->scale = scale * 0.3f;

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

void Bomb::Render()
{
	exp->Render();

	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Bomb::Debug()
{
	ImGui::Text("Bomb");

	//Transform::Debug();

	//Model::Debug();

	//exp->Debug();
}

void Bomb::Throw()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	direction = ray.direction.GetNormalized();

	isAppear = true;

	//speed = 20.0f;
}

void Bomb::Charging()
{
	if (speed < 1000)
		speed += Time::Delta() * 50;

	isExist = true;
}

void Bomb::Explode()
{
	exp->SetExplode(true);
}
