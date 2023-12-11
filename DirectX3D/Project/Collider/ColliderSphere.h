#pragma once
class ColliderSphere : public Collider
{
public:
	ColliderSphere(float radius = 1.0f, UINT stackCount = 15, UINT sliceCount = 30);
	~ColliderSphere();

	virtual bool Collision(IN Ray& ray, OUT Contact* contact = nullptr);
	virtual bool Collision(class ColliderBox* other);
	virtual bool Collision(class ColliderSphere* other);
	virtual bool Collision(class ColliderCapsule* other);

	float Radius() { return radius * Max(globalScale.x, globalScale.y, globalScale.z); }

	virtual bool Block(class ColliderBox* other) override;
	virtual bool Block(class ColliderSphere* other) override;

private:
	virtual void CreateMesh();

	float radius;
	UINT stackCount;
	UINT sliceCount;
};