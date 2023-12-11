#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	//zombie = new Zombie();
	groot = new Groot();

	groot->scale *= 0.05;

	terrain = new Terrain(L"LandScape/Dirt.png", L"HeightMap/Heightmap.png");

	terrain->scale *= 2.5;

	groot->translation = { 82.2f, 4.6f, 55.4f };

	//CAMERA->SetTarget(groot);

	crosshair = new Quad();
	crosshair->GetMaterial()->SetDiffuseMap(L"UI/CrossHair.png");
	crosshair->GetMaterial()->SetShader(L"Texture");
	crosshair->scale = { 200, 200, 1 };

	//CAMERA->SetParent(groot);

	//CAMERA->translation.y += 700.0f;

	sky = new SkyBox(L"Landscape/SpaceBox.dds");

	CAMERA->rotation.y = groot->Forward().y - XM_PI;
}

ModelAnimationScene::~ModelAnimationScene()
{
	//delete zombie;
	delete groot;

	delete  terrain;

	delete crosshair;

	delete sky;
}

void ModelAnimationScene::Update()
{
	//zombie->Update();
	groot->Update();

	terrain->Update();

	groot->translation.y = terrain->GetHeight(groot->GetGlobalPosition());

	CAMERA->translation = groot->GetGlobalPosition();
	CAMERA->translation.y += 10;


	if (KEY_PRESS('A'))
	{
		CAMERA->rotation.y -= 3.0f * Time::Delta();
	}
	if (KEY_PRESS('D'))
	{
		CAMERA->rotation.y += 3.0f * Time::Delta();
	}


	crosshair->translation.x = mousePos.x;
	crosshair->translation.y = 2 * WIN_HEIGHT - mousePos.y;

	crosshair->Update();

	//groot->rotation.y = CAMERA->rotation.y;
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	sky->Render();
	//zombie->Render();
	groot->Render();

	terrain->Render();

}

void ModelAnimationScene::PostRender()
{
	//StateManager::GetInstance()->AlphaBegin();

	crosshair->Render();

	//StateManager::GetInstance()->AlphaEnd();

	terrain->Debug();
	groot->PostRender();

	//sky->PostRender();
}