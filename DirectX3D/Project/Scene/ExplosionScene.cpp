#include "Framework.h"
#include "ExplosionScene.h"

ExplosionScene::ExplosionScene()
{
	terrain = new Terrain(L"Landscape/Dirt.png", L"HeightMap/HeightMap.png");

	//bomb = new Bomb3();

	//box = new ColliderBox(Vector3(3,3,3));

	//box->translation.y -= 10;

	groot = new Groot();
	groot->scale *= 0.1f;

	skybox = new SkyBox(L"Landscape/SpaceBox.dds");
}

ExplosionScene::~ExplosionScene()
{
	delete terrain;

	//delete bomb;
	//delete box;

	delete groot;

	delete skybox;
}

void ExplosionScene::Update()
{
	terrain->Update();

	//bomb->Update();

	//if (KEY_PRESS(VK_LBUTTON))
	//{
	//	bomb->Charging();
	//}

	//else if (KEY_UP(VK_LBUTTON) && !bomb->IsAppear())
	//	bomb->Throw();

	//if (box->Collision(bomb->GetCollider()))
	//	bomb->SetCollision(true);

	//box->Update();
	//terrain->OnTheGround(&groot->translation);

	groot->translation.y = terrain->GetHeight(groot->GetGlobalPosition());

	if (groot->GetBombIsExist())
	{
		if (terrain->GetHeight(groot->GetBombGPos()) > groot->GetBombGPos().y)
			groot->BombIsGround(true);
	}

	groot->Update();


	CAMERA->translation = groot->GetGlobalPosition();
	CAMERA->translation.y += 3.0f;
	//CAMERA->translation.z -= 1.0f;
	CAMERA->rotation.y = groot->rotation.y + XM_PI;
}

void ExplosionScene::PreRender()
{
}

void ExplosionScene::Render()
{
	skybox->Render();

	//bomb->Render();

	//box->Render();

	terrain->Render();

	groot->Render();
}

void ExplosionScene::PostRender()
{
	//bomb->Debug();

	groot->Debug();
}
