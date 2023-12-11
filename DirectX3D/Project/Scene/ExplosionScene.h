#pragma once
class ExplosionScene : public Scene
{
public:
	ExplosionScene();
	~ExplosionScene();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

private:
	//Bomb3* bomb;

	//ColliderBox* box;

	Groot* groot;

	Terrain* terrain;

	SkyBox* skybox;
};

