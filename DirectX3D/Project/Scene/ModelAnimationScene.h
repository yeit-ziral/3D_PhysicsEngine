#pragma once
class ModelAnimationScene : public Scene
{
public:
	ModelAnimationScene();
	~ModelAnimationScene();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

private:
	//Zombie* zombie;
	Groot* groot;

	Terrain* terrain;

	Quad* crosshair;

	SkyBox* sky;
};