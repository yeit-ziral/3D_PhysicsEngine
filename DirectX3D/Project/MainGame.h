#pragma once



class MainGame
{

public:
	MainGame();
	~MainGame();

	void Update();
	void Render();

private:
	void Initialize();
	void Release();

private:
	Scene* scene = nullptr;

	bool isWireFrame = false;
};

