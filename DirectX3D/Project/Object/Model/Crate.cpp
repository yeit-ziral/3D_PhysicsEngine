#include "Framework.h"
#include "Crate.h"

Crate::Crate()
    :Model("Crate")
{
	reader = new ModelReader("Crate");
	GetReader()->GetMaterial()[0]->Load(L"Crate.mat");

	collider = new ColliderBox();

	collider->SetParent(this);

	scale *= 0.02;

	scale.z *= 0.9f;

	collider->scale.z *= 10.0f / 9.0f;

	collider->scale *= 60;

	collider->translation.z -= 35;
}

Crate::~Crate()
{
	delete reader;
	delete collider;
}

void Crate::Update()
{
	if (!isAppear)
		return;

	Model::Update();

	collider->Update();
}

void Crate::Render()
{
	if (!isAppear)
		return;
	Model::Render();
	collider->Render();
}

void Crate::Debug()
{
	ImGui::Text("Crate");
	//Transform::Debug();
	//Model::Debug();
}

void Crate::Move()
{
}
