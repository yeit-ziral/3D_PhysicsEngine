#pragma once
class ColliderCapsule : public Collider
{
public:
	ColliderCapsule(float radius = 1.0f, float height = 2.0f,
		UINT stackCount = 15, UINT sliceCount = 30); // scale Á¶Á¤ÀÌ Èûµë
	~ColliderCapsule();


	virtual bool Collision(IN Ray& ray, OUT Contact* contact = nullptr) override;
	virtual bool Collision(class ColliderBox* other) override;
	virtual bool Collision(class ColliderSphere* other) override;
	virtual bool Collision(class ColliderCapsule* other) override;

	float Radius() { return radius * max(Transform::GetGlobalScale().x, max(Transform::GetGlobalScale().y, Transform::GetGlobalScale().z)); }
	float Height() { return height * Transform::GetGlobalScale().y; }

private:
	virtual void CreateMesh() override;

private:
	float radius;
	float height;

	UINT stackCount;
	UINT sliceCount;
};