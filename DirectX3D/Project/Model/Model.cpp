#include "Framework.h"
#include "Model.h"

Model::Model(string name, wstring shaderFile)
{
	reader = new ModelReader(name);
	reader->SetShader(shaderFile);
}

Model::~Model()
{
	delete reader;
}

void Model::Update()
{
	Transform::Update();
}

void Model::Render()
{
	Transform::SetWorld();

	reader->Render();
}

void Model::RenderInstanced(UINT instanceCount)
{
	Transform::SetWorld();

	reader->RenderInstanced(instanceCount);
}

void Model::Debug()
{
	if (ImGui::BeginMenu(label.c_str()))
	{
		ImGui::DragFloat3("Scale", (float*)&scale, 0.01f, 0.01f, 100.0f);

		ImGui::SliderAngle("RotationX", &rotation.x);
		ImGui::SliderAngle("RotationY", &rotation.y);
		ImGui::SliderAngle("RotationZ", &rotation.z);

		ImGui::DragFloat3("Translation", (float*)&translation, 0.01f, -WIN_WIDTH, WIN_WIDTH);



		ImGui::EndMenu();
	}
}
