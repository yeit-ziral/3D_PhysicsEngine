#include "Framework.h"
#include "Box2.h"

Box2::Box2()
{
	quad1 = new Quad(size); // 앞
	quad2 = new Quad(size); // 뒤
	quad3 = new Quad(size); // 왼
	quad4 = new Quad(size); // 오
	quad5 = new Quad(size); // 위
	quad6 = new Quad(size); // 아래

	quad2->SetParent(quad1);
	quad3->SetParent(quad1);
	quad4->SetParent(quad1);
	quad5->SetParent(quad1);
	quad6->SetParent(quad1);
}

Box2::~Box2()
{
	delete quad1;
	delete quad2;
	delete quad3;
	delete quad4;
	delete quad5;
	delete quad6;
}

void Box2::Update()
{
	quad1->Update();
	quad2->Update();
	quad3->Update();
	quad4->Update();
	quad5->Update();
	quad6->Update();

	quad2->rotation.y = Ridan(180);
	quad2->translation.z = size.x;

	quad3->rotation.y = Ridan(90);
	quad3->translation.z = size.x * 0.5f;
	quad3->translation.x = -size.x * 0.5f;

	quad4->rotation.y = Ridan(-90);
	quad4->translation.z = size.x * 0.5f;
	quad4->translation.x = size.x * 0.5f;

	quad5->rotation.x = Ridan(90);
	quad5->translation.y = size.x * 0.5f;
	quad5->translation.z = size.x * 0.5f;

	quad6->rotation.x = Ridan(-90);
	quad6->translation.y = -size.x * 0.5f;
	quad6->translation.z = size.x * 0.5f;

	if (KEY_PRESS('Q'))
	{
		quad1->rotation.y += Time::Delta();
	}

	if (KEY_PRESS('E'))
	{
		quad1->rotation.y -= Time::Delta();
	}

	if (KEY_PRESS('1'))
	{
		quad1->rotation.x += Time::Delta();
	}

	if (KEY_PRESS('3'))
	{
		quad1->rotation.x += Time::Delta();
	}

	if (KEY_PRESS('W'))
	{
		quad1->translation += quad1->Backward() * Time::Delta();
	}

	if (KEY_PRESS('S'))
	{
		quad1->translation += quad1->Forward() * Time::Delta();
	}
}

void Box2::Render()
{
	quad1->Render();
	quad2->Render();
	quad3->Render();
	quad4->Render();
	quad5->Render();
	quad6->Render();
}