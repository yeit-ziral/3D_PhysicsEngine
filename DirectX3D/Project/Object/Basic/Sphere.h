#pragma once
class Sphere : public Transform
{
	typedef VertexTextureNormalTangent VertexType;
public:
	Sphere(float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 10);
	~Sphere();

	void Update();
	void Render();

	Material* GetMaterial() { return material; }

	float Radius() { return radius * Max(globalScale.x, globalScale.y, globalScale.z); }

private:
	void CreateMesh();
	void CreateTangent();

private:
	MatrixBuffer* worldBuffer = nullptr;

	vector<VertexType> vertices;
	vector<UINT> indices;

	Material*	material = nullptr;
	Mesh*		mesh	 = nullptr;

	UINT sliceCount = 25;
	UINT stackCount = 25;
	float radius;
};
