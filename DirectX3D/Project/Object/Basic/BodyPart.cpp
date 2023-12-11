#include "Framework.h"
#include "BodyPart.h"

BodyPart::BodyPart(vector<Material*>materials, ModelMesh* _mesh)
	: materials(materials), _mesh(_mesh)
{

	for (ModelVertex vertex : _mesh->GetVertices())
	{
		vertices.push_back(vertex);
	}

	for (UINT index : _mesh->GetIndices())
	{
		indices.push_back(index);
	}

	for (Material* _material : materials)
	{
		material = _material;
	}
	material = materials[0];

	mesh = new Mesh(vertices, indices);
}

BodyPart::~BodyPart()
{
}

void BodyPart::Update()
{
	Transform::Update();
}

void BodyPart::Render(int num)
{
	Transform::SetWorld();

	mesh->SetMesh();
	material->SetMaterial();

	DC->DrawIndexed(indices.size(), 0, 0);
}
