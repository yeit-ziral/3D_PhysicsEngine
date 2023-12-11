#pragma once
class ModelMesh
{
	friend class ModelReader;
public:
	ModelMesh();
	~ModelMesh();

	vector<ModelVertex> GetVertices() { return vertices; }
	vector<UINT>		GetIndices()  { return indices; }

	Mesh* GetMesh() { return mesh; }

	Material* GetMaterial() { return material; }

	void Create();

	void Render();

	void RenderInstanced(UINT instanceCount);

private:
	string name;

	Material* material;

	Mesh* mesh;

	vector<ModelVertex> vertices;
	vector<UINT>		 indices;
};