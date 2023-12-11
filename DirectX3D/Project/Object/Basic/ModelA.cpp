#include "Framework.h"
#include "ModelA.h"

ModelA::ModelA(string name)
{
	reader = new ModelReader(name);
	worldBuffer = new MatrixBuffer();

	reader->SetShader(L"NormalMapping");
}

ModelA::~ModelA()
{
	delete reader;
	delete worldBuffer;
}

void ModelA::Update()
{

	Transform::Update();
}

void ModelA::Render()
{
	worldBuffer->SetData(world);
	worldBuffer->SetVSBuffer(0);

	for (Material* material : reader->GetMaterials())
	{
		material->SetMaterial();
	}

	for (int i = 0; i < reader->GetMeshes().size(); i++)
	{
		reader->GetMeshes()[i]->GetMesh()->SetMesh();
		DC->DrawIndexed(reader->GetMeshes()[i]->GetIndices().size(), 0, 0);
	}
}
