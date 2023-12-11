#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	pumpkin = new Pumpkin();
	skeleton = new Skeleton();
	ghost = new Ghost();
}

ShootingScene::~ShootingScene()
{
	delete pumpkin;
	delete ghost;
	delete skeleton;
}

void ShootingScene::Update()
{
	pumpkin ->Update();
	ghost	->Update();
	skeleton->Update();

	if (pumpkin->GetCollider()->Collision(ghost->GetCollider()))
	{
		pumpkin->SetIsAppear(false);
		ghost->SetIsAppear(false);
	}
	if (pumpkin->GetCollider()->Collision(skeleton->GetCollider()))
	{
		pumpkin->SetIsAppear(false);
		skeleton->SetIsAppear(false);
	}

	if (KEY_DOWN(VK_LBUTTON) && !pumpkin->IsAppear())
		pumpkin->Throw();
}

void ShootingScene::PreRender()
{
}

void ShootingScene::Render()
{
	pumpkin ->Render();
	ghost	->Render();
	skeleton->Render();
}

void ShootingScene::PostRender()
{
	pumpkin ->Debug();
	ghost	->Debug();
	skeleton->Debug();
}
