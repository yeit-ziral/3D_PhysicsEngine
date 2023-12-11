#include "Framework.h"
#include "Pumpkin.h"

Pumpkin::Pumpkin()
	:Model("Grenade")
{
	collider = new ColliderSphere();

	collider->SetParent(this);

	scale *= 10.0f;

	collider->scale *= 0.066f;

	//collider->

	GetReader()->GetMaterial()[0]->Load(L"Grenade.mat");
}

Pumpkin::~Pumpkin()
{
	delete collider;
}

void Pumpkin::Update()
{
	Model::Update();
	collider->Update();

	//collider->translation = Model::GetGlobalPosition();
	//collider->translation += {0.0f, 0.4f, 0.0f};

	//collider->scale = scale * 0.3f;

	//if (isAppear)
	//{
	//	nowTime += Time::Delta();

	//	translation += direction * speed * Time::Delta();

	//	direction.y -= 0.5f * Time::Delta();

	//	if (nowTime > 4.0f)
	//	{
	//		nowTime = 0.0f;
	//		translation = CAMERA->GetGlobalPosition();
	//		isAppear = false;
	//	}
	//}
}

void Pumpkin::Render()
{
	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Pumpkin::Debug()
{
	Model::Debug();
}

void Pumpkin::Throw()
{
	
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	direction = ray.direction.GetNormalized();

	isAppear = true;
	speed = 20;
}

//void Pumpkin::Appear()
//{
//	if (nowTime / time > 1)
//	{
//		nowTime = 0;
//
//		if (!isAppear)
//		{
//			isAppear = true;
//		}
//		else
//		{
//			isAppear = false;
//
//			translation = RandomPosition();
//		}
//	}
//	else
//	{
//		if (isAppear)
//			Move();
//	}
//}
//
//void Pumpkin::Move()
//{
//	if (nowTime < 1.5f)
//		translation.x += Time::Delta();
//	else
//		translation.x -= Time::Delta();
//}
//
//Vector3 Pumpkin::RandomPosition()
//{
//	Vector3 newPos;
//
//	newPos = CAMERA->GetGlobalPosition();
//
//	newPos.z = 0;
//
//	newPos.x += RandomNum(-2, 2);
//	newPos.y += RandomNum(-2, 2);
//	newPos.z += RandomNum(-2, 2);
//
//	return newPos;
//}
