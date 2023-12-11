#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	viewBuffer = new ViewBuffer();

	//Transform::translation = { 100, 120, -100 };
	//Transform::rotation.x = 0.65f;
	//Transform::rotation.y = 0.65f;

	Load();
}

Camera::~Camera()
{
	Save();

	delete viewBuffer;
}

void Camera::Update()
{
	CalculateFrustum();

	if (target == nullptr)
		FreeMode();
	else
		TargetMode(MODE2);
}

void Camera::Debug()
{
	ImGuiIO io = ImGui::GetIO();
	distance -= io.MouseWheel * moveSpeed;

	Transform::Debug();


	if (ImGui::TreeNode("Camera Option"))
	{
		Vector3 pos = translation;
		Vector3 rot = rotation;

		ImGui::Text("Camera Pos : %f, %f, %f", pos.x, pos.y, pos.z);// const char* => char*은 string을 넣으라는 소리이고, const는 "abcd" 이 형식으로 넣으라는 소리이다 string a가 아니라
		ImGui::Text("Camera Rot : %.3f, %.3f, %.3f", rot.x, rot.y, rot.z); // f앞에 .n 을 적어서 몇번째 자리까지 표현할지 정할 수 있음
		// 서식 지정자 %d(정수), %f(실수), %c(charactor), %s(string), %p(pointer) 등에 맞게 넣어줘야 함

		ImGui::SliderFloat("Height",   &height,   -10.0f, 100.0f);
		ImGui::SliderFloat("Distance", &distance, -10.0f, 100.0f);

		ImGui::SliderFloat("RotationY", &rotY, 0, XM_2PI);

		ImGui::SliderFloat("Camera MoveDamping", &moveDamping, 0.0f, 30.0f);
		ImGui::SliderFloat("Camera RotationDamping", &rotDamping, 0.0f, 30.0f);

		ImGui::TreePop();
	}
}

Ray Camera::ScreenPointToRay(Vector3 screenPos) // screenPos : Near Plane에 찍히는 점의 위치
{
	Ray ray;
	ray.origin = translation;

	///////////////////Direction(Inverse ViewPort)

	Vector3 point;

	point.x = +(2.0f * screenPos.x) / WIN_WIDTH - 1.0f;
		//y를 뒤집어줘야 함-> viewport
	point.y = -(2.0f * screenPos.y) / WIN_HEIGHT + 1.0f;
	point.z = 1.0f; //FarZ

	//////////////Invporjection/////////////// 맨 아래 윈쪽을 0,0으로 유지하기 위해서

	Matrix projection = Environment::GetInstance()->GetProjMatrix();

	XMFLOAT4X4 proj;

	XMStoreFloat4x4(&proj, projection);

	point.x /= proj._11;
	point.y /= proj._22;

	////////////////invView/////////////

	Matrix invView = GetWorld();

	ray.direction = point * invView;
	ray.direction.Normalize();

	return ray;
}

Vector3 Camera::WorldToScreenPoint(Vector3 worldPos)
{
	Vector3 screenPos;

	screenPos = XMVector3TransformCoord(worldPos, viewMatrix);
	screenPos = XMVector3TransformCoord(screenPos, Environment::GetInstance()->GetProjMatrix());

	screenPos = (screenPos + Vector3(1, 1, 1)) * 0.5f;

	screenPos.x *= WIN_WIDTH;
	screenPos.y *= WIN_HEIGHT;

	return screenPos;
}

bool Camera::ContainPoint(Vector3 point)
{
	for (UINT i = 0; i < 6; i++)
	{
		Vector3 dot = XMPlaneDotCoord(planes[i], point);

		if (dot.x < 0.0f)
			return false;
	}

	return true;
}

bool Camera::ContainSphere(Vector3 center, float radius)
{
	Vector3 edge;
	Vector3 dot;

	for (UINT i = 0; i < 6; i++)
	{
		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f) 
			continue;

		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);

		if (dot.x > 0.0f)
			continue;

		return false;
	}

	return true;
}

void Camera::FreeMode()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		if (KEY_PRESS(VK_LSHIFT))
			moveSpeed = 50.0f;
		else
			moveSpeed = 20.0f;

		if (KEY_PRESS('A'))
			translation += Transform::Left()			* moveSpeed * Time::Delta();

		if (KEY_PRESS('D'))
			Transform::translation += Transform::Right()		* moveSpeed * Time::Delta();

		if (KEY_PRESS('S'))
			Transform::translation += Transform::Backward()		* moveSpeed * Time::Delta();

		if (KEY_PRESS('W'))
			Transform::translation += Transform::Forward()		* moveSpeed * Time::Delta();

		if (KEY_PRESS('Q'))
			Transform::translation += Transform::Up()			* moveSpeed * Time::Delta();

		if (KEY_PRESS('E'))
			Transform::translation += Transform::Down()			* moveSpeed * Time::Delta();

		Vector3 dir = mousePos - oldPos;

		if (abs(dir.x) > 15.0f || abs(dir.y) > 15.0f)
			dir = Vector3(0.0f, 0.0f, 0.0f);

		Transform::rotation.y += dir.x * rotSpeed;
		Transform::rotation.x += dir.y * rotSpeed;

	}

	viewMatrix = XMMatrixInverse(nullptr, Transform::GetWorld());

	oldPos = mousePos;

	SetView();
}

void Camera::TargetMode(Mode mode)
{

	if (KEY_PRESS(VK_UP))
		height += moveSpeed * Time::Delta();
	if (KEY_PRESS(VK_DOWN))
		height -= moveSpeed * Time::Delta();

	switch (mode)
	{
	case Camera::MODE1: 
	{
		if (KEY_PRESS(VK_RBUTTON))
		{
			Vector3 dir = mousePos - oldPos;

			rotY += dir.x * rotSpeed;
		}
		oldPos = mousePos;

		destRotY = LERP(destRotY, rotY, rotDamping * Time::Delta());

		XMMATRIX rotMatrix = XMMatrixRotationY(destRotY + rotY);

		Vector3 forward = V_FORWARD * rotMatrix;

		destination = target->GetGlobalPosition() + forward * distance + V_UP * height;

		Transform::translation = LERP(Transform::translation, destination, moveDamping * Time::Delta());

		viewMatrix = XMMatrixLookAtLH(Transform::translation, target->translation, V_UP);

	}
		break;
	case Camera::MODE2:
	{


		if (KEY_PRESS(VK_RBUTTON))
		{
			Vector3 dir = mousePos - oldPos;

			Transform::rotation.y += dir.x * rotSpeed;
			Transform::rotation.x += dir.y * rotSpeed;


			rotY += dir.x * rotSpeed * Time::Delta();
		}

		oldPos = mousePos;

		destRotX = LERP(destRotX, Transform::rotation.x, rotDamping * Time::Delta());
		destRotY = LERP(destRotY, Transform::rotation.y, rotDamping * Time::Delta());

		XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(destRotX, destRotY + rotY, 0.0f);

		Vector3 forward = V_FORWARD * rotMatrix;

		destination = target->GetGlobalPosition() + forward * distance;

		Transform::translation = LERP(Transform::translation, destination, moveDamping * Time::Delta());

		Vector3	   eyePos = Transform::translation;
		Vector3 targetPos =	   target->translation;

		   eyePos.y += height;
		targetPos.y += height;

		viewMatrix = XMMatrixLookAtLH(eyePos, targetPos, V_UP);

		// viewMatrix *= XMMatrixTranslation(0, -height, 0);
	}
		break;
	default:
		break;
	}

	SetView();
}
//
//void Camera::TargetMode()
//{
//	//destination = target->translation - target->Backward() * distance + V_UP * height;
//
//	//Transform::translation = destination;
//
//	//viewMatrix = XMMatrixLookAtLH(pos, target->translation, V_UP);
//
//	//destination = target->translation - V_FORWARD * distance + V_UP * height; // 기존 탑뷰
//
//	//Transform::translation = destination;
//
//	//viewMatrix = XMMatrixLookAtLH(destination, target->translation, V_UP);
//
//	//destRot = LERP(destRot, target->rotation.y, rotDamping * Time::Delta()); // 살짝 숄더뷰 + 뎀핑
//
//	//XMMATRIX rotMatrix = XMMatrixRotationY(destRot + rotY);
//
//	//Vector3 forward = V_FORWARD * rotMatrix;
//
//	//destination = target->GetGlobalPosition() + forward * distance + V_UP * height;
//
//	//Transform::translation = LERP(Transform::translation, destination, moveDamping * Time::Delta());
//
//	//viewMatrix = XMMatrixLookAtLH(Transform::translation, target->translation, V_UP);
//
//}

void Camera::SetView() 	// Hull Shader 추가 후 주석 풀 것 있음
{
	Transform::Update();

	//XMVECTOR   eyePos = Transform::translation;
	//XMVECTOR focusPos = Transform::translation + Transform::Forward();
	//XMVECTOR upVector = Transform::Up();

	//viewMatrix = XMMatrixLookAtLH(eyePos, focusPos, upVector);


	viewBuffer->SetData(viewMatrix, Transform::GetWorld()); // Transform::GetWorld()가 Inverse View이다
	viewBuffer->SetVSBuffer(1);
	// Hull Shader 추가 후 주석 풀 것
	//viewBuffer->SetHSBuffer(1);
}

void Camera::Save()
{
	BinaryWriter data(L"CameraData");

	data.WriteData(Transform::scale);
	data.WriteData(Transform::rotation);
	data.WriteData(Transform::translation);
}

void Camera::Load()
{
	BinaryReader data(L"CameraData");

	if (!data.Succeeded())
		return;

	Transform::scale		= data.ReadVector3();
	Transform::rotation		= data.ReadVector3();
	Transform::translation	= data.ReadVector3();
}

void Camera::CalculateFrustum()
{
	XMFLOAT4X4 VP;
	XMStoreFloat4x4(&VP, viewMatrix * Environment::GetInstance()->GetProjMatrix());

	//Left
	a = VP._14 + VP._11;
	b = VP._24 + VP._21;
	c = VP._34 + VP._31;
	d = VP._44 + VP._41;

	planes[0] = XMVectorSet(a, b, c, d);

	//Right
	a = VP._14 - VP._11;
	b = VP._24 - VP._21;
	c = VP._34 - VP._31;
	d = VP._44 - VP._41;

	planes[1] = XMVectorSet(a, b, c, d);

	//Bottom
	a = VP._14 + VP._12;
	b = VP._24 + VP._22;
	c = VP._34 + VP._32;
	d = VP._44 + VP._42;

	planes[2] = XMVectorSet(a, b, c, d);

	//Top
	a = VP._14 - VP._12;
	b = VP._24 - VP._22;
	c = VP._34 - VP._32;
	d = VP._44 - VP._42;

	planes[3] = XMVectorSet(a, b, c, d);

	//Near
	a = VP._14 + VP._13;
	b = VP._24 + VP._23;
	c = VP._34 + VP._33;
	d = VP._44 + VP._43;

	planes[4] = XMVectorSet(a, b, c, d);

	//Far
	a = VP._14 - VP._13;
	b = VP._24 - VP._23;
	c = VP._34 - VP._33;
	d = VP._44 - VP._43;

	planes[5] = XMVectorSet(a, b, c, d);

	for (UINT i = 0; i < 6; i++)
	{
		planes[i] = XMPlaneNormalize(planes[i]);
	}
}
