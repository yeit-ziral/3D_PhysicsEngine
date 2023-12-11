#pragma once
class Box : public Transform
{
public:
	Box();
	~Box();

	void Update();
	void Render();
	void PostRender();

	

private:
	Cube* cube;
	Quad* quadTop;
	Quad* quadBottom;
	Quad* quadFront;
	Quad* quadBack;
	Quad* quadRight;
	Quad* quadLeft;

	ColliderBox* collider;
};