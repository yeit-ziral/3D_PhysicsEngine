#pragma once

typedef VertexTextureNormalTangentBlend ModelVertex;
#include "Model/ModelMesh.h"

class BodyPart : public Transform//∫Œ¿ß∏∏ ∑ª¥ı
{
public:
	BodyPart(vector<Material*>materials, ModelMesh* _mesh);
	~BodyPart();

	void Update();
	void Render(int num);

private:
	vector<Material*>  materials;
	ModelMesh* _mesh;

	vector<ModelVertex> vertices;
	vector<UINT>		indices;

	Material* material;
	Mesh* mesh;
};

