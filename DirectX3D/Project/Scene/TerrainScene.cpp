#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain(L"Landscape/Dirt.png", L"HeightMap/HeightMap.png");

	//terrain->GetMaterial()->SetDiffuseMap(L"Landscape/Fieldstone_DM.tga");
	//terrain->GetMaterial()->SetSpecularMap(L"Landscape/Fieldstone_SM.tga");

	// normalMap 추가하기
	//terrain->GetMaterial()->SetNormalMap(L"Landscape/fieldsone_NM.tga");

	//sphere = new Sphere();
	zombie = new Zombie();
	zombie->scale *= 0.001;

	CAMERA->SetParent(zombie);
	CAMERA->translation = { 0.0f, 1000.0f, 0.0f };
}

TerrainScene::~TerrainScene()
{
	delete terrain;

	delete zombie;
}

void TerrainScene::Update()
{
	terrain->Update();

	//sphere->Update();
	zombie->Update();

	if (KEY_DOWN(VK_LBUTTON))
	{
		terrain->Picking(&pickedPos);

		//sphere->translation = pickedPos;
		//sphere->translation.y += sphere->scale.y;

		zombie->translation = pickedPos;
	}

	if (KEY_PRESS('W'))
	{
		zombie->translation.z += Time::Delta() * 50;
		terrain->OnTheGround(&zombie->translation);
		//class AnimState* state = AnimState::RUN;
		//zombie->SetClip();
	}
	if (KEY_PRESS('S'))
	{
		zombie->rotation.y -= Time::Delta() * 50;
		terrain->OnTheGround(&zombie->translation);
	}
	if (KEY_PRESS('A'))
	{
		zombie->rotation.y += Time::Delta() * 50;
	}
	if (KEY_PRESS('D'))
	{
		zombie->translation.x -= Time::Delta() * 50;
	}
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();

	//sphere->Render();

	zombie->Render();
}

void TerrainScene::PostRender()
{
	terrain->GetMaterial()->Debug();

	ImGui::Text("PickedPos : %.1f, %.1f,%.1f", pickedPos.x, pickedPos.y, pickedPos.z);
}
