#include "Framework.h"
#include "Knife_A.h"

Knife_A::Knife_A()
	:Model("Knife_A")
{
}

Knife_A::~Knife_A()
{
}

void Knife_A::Update()
{
	Model::Update();
}

void Knife_A::Render()
{
	Model::Render();
}

void Knife_A::Debug()
{
	Model::Debug();
}
