#pragma once
class BlockScene : public Scene //Block 실험해보기 위해 만든 Scene
{
public:
	BlockScene();
	~BlockScene();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

private:
	Crate* crate;

	Groot* groot;
};