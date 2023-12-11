#include "Framework.h"
#include "Transform.h"

Transform::Transform()
{
	world = XMMatrixIdentity();
	
	worldBuffer = new WorldBuffer();
}

Transform::~Transform()
{
	delete worldBuffer;
}

void Transform::Update()
{
	UpdateWorld();
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation
	(
		pivot, XMQuaternionIdentity(), scale,
		pivot, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z), // RollPitchYaw가 들어가서 Quaternion이 아니라 오일러 회전을 하고 있고 값만 Quaternion처럼 보여줌
		translation
	);

	if (parent != nullptr)
		world *= parent->world; // parent의 world가 문제 없는 이유는 parent도 Transform이고 this도 Transform인 동일 클래스여서 가능하다.

	XMFLOAT4X4 fWorld;

	XMStoreFloat4x4(&fWorld, world);

	right	= Vector3(fWorld._11, fWorld._12, fWorld._13).GetNormalized();
	up		= Vector3(fWorld._21, fWorld._22, fWorld._23).GetNormalized();
	forward = Vector3(fWorld._31, fWorld._32, fWorld._33).GetNormalized();

	XMVECTOR outS, outR, outT;

	XMMatrixDecompose(&outS, &outR, &outT, world);

	globalScale = outS;
	globalRotation = outR; // 쓰기 어려워서 자주 쓰이지 않다
	globalPosition = outT;
}

void Transform::Debug()
{
	if (ImGui::BeginMenu(label.c_str()))
	{
		ImGui::DragFloat3("Scale", (float*)&scale, 0.01f, 0.01f, 100.0f);
		//ImGui::DragFloat3("Rotation",       (float*)&rotation,      0.01f,      -XM_2PI,    XM_2PI);

		ImGui::SliderAngle("RotationX", &rotation.x);
		ImGui::SliderAngle("RotationY", &rotation.y);
		ImGui::SliderAngle("RotationZ", &rotation.z);

		ImGui::DragFloat3("Translation", (float*)&translation, 0.01f, -WIN_WIDTH, WIN_WIDTH);



		ImGui::EndMenu();
	}
}

void Transform::SetWorld(int hasAnimation)
{
	worldBuffer->SetData(world, hasAnimation);
	worldBuffer->SetVSBuffer(0);
}
