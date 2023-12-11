#pragma once
class ColliderBox : public Collider
{
public:
	struct Obb
	{
		Vector3 position;

		Vector3 axis[3];
		Vector3 halfSize;
	};

public:
	ColliderBox(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~ColliderBox();

	virtual bool Collision(IN Ray& ray, OUT Contact* contact = nullptr) override;
	virtual bool Collision(class ColliderBox* other) override;
	virtual bool Collision(class ColliderSphere* other) override;
	virtual bool Collision(class ColliderCapsule* other) override;

	virtual bool Block(class ColliderBox* other) override;
	virtual bool Block(class ColliderSphere* other) override;

	Obb GetOBB();

	bool SeparatedAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2);

private:
	virtual void CreateMesh() override;

private:
	Vector3 size;

};