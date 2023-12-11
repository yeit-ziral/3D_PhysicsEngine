#pragma once
class Box2 : public Transform
{
public:
    Box2();
    ~Box2();

    void Update();
    void Render();

	float Ridan(float value) { return value * XM_PI / 180; }

private:
	Quad* quad1;
	Quad* quad2;
	Quad* quad3;
	Quad* quad4;
	Quad* quad5;
	Quad* quad6;

	Vector2 size = { 1,1 };

	ColliderBox* collider;
};