#include "Framework.h"
#include "Material.h"

int Material::ID = 0;

Material::Material()
{
	buffer = new MaterialBuffer();

	char path[128];

	GetCurrentDirectoryA(128, path);

	projectDir = path; // GetCurrentDirectoryA에 string이 안들어가서 간접적으로 넣어줌

	projectDir += "/Texture";

	ID += 1; // Key값 중복 안되게 하려고

	label.resize(20);
	label = "Material" + to_string(ID);

	//Load(ToWString(label + " Data"));
}

Material::Material(wstring file)
{
	buffer = new MaterialBuffer();

	SetShader(file);

	char path[128];

	GetCurrentDirectoryA(128, path);

	projectDir = path;

	projectDir += "Texture/";

	ID += 1;

	label.resize(20);
	label = "Material" + to_string(ID);
}

Material::~Material()
{
	delete buffer;
}

void Material::SetVertexShader(wstring file)
{
	vertexShader = Shader::GetVS(file);
}

void Material::SetPixelShader(wstring file)
{
	pixelShader = Shader::GetPS(file);
}

void Material::SetShader(wstring file)
{
	SetVertexShader(file);
	SetPixelShader(file);
}

void Material::SetMaterial()
{
	vertexShader->SetShader();
	pixelShader->SetShader();

	if (diffuseMap) // diffuseMap 안쓰는 것들의 에러를 막기 위해 diffuseMap이 nullptr이 아닌것만 사용하도록 함
		diffuseMap->PSSetShaderResources(0);

	if (specularMap)
		specularMap->PSSetShaderResources(1);

	if (normalMap)
		normalMap->PSSetShaderResources(2);

	buffer->SetPSBuffer(1);
}

void Material::SetDiffuseMap(wstring file)
{
	diffuseMap = Texture::Get(file);

	buffer->data.hasDiffuseMap = true;
}

void Material::SetDiffuseMap(Texture* texture)
{
	diffuseMap = texture; // 가상으로만 존재해서 포인터 필요없음

	buffer->data.hasDiffuseMap = true;
}

void Material::SetSpecularMap(wstring file)
{
	specularMap = Texture::Get(file);

	buffer->data.hasSpecularMap = true;
}

void Material::SetNormalMap(wstring file)
{
	normalMap = Texture::Get(file);

	buffer->data.hasNormalMap = true;
}

void Material::Debug()
{
	char*  str = (char*)label.data();

	ImGui::InputText("Label", str, 128);

	label = str;

	if (label[0] == '\0')
		label = "NULL"; // ImGui에서 글자를 다 지웠을 때 터지는 오류 막기위함

	if (ImGui::BeginMenu(label.c_str(), true))
	{

		ImGui::ColorEdit4((label + "Diffuse").c_str(), (float*)&buffer->data.diffuse);
		ImGui::ColorEdit4((label + "Specular").c_str(), (float*)&buffer->data.specular);
		ImGui::ColorEdit4((label + "Ambient").c_str(), (float*)&buffer->data.ambient);
		ImGui::ColorEdit4((label + "Emissive").c_str(), (float*)&buffer->data.emissive);

		ImGui::Checkbox((label + "HasDiffuseMap").c_str(), (bool*)&buffer->data.hasDiffuseMap);
		ImGui::Checkbox((label + "HasSpecularMap").c_str(), (bool*)&buffer->data.hasSpecularMap);
		ImGui::Checkbox((label + "HasNormalMap").c_str(), (bool*)&buffer->data.hasNormalMap);

		ImGui::SliderFloat((label + "Shininess").c_str(), &buffer->data.shininess, 1.0f, 50.0f);

		SaveDialog();

		LoadDialog();

		ImGui::EndMenu();
	}

	SelectMap();

	//SaveDialogMap();
	//LoadDialogMap();
}

void Material::SelectMap()
{

	if (ImGui::BeginMenu(label.c_str())) // ImGui는 Begin을 하면 꼭 End를 해줘야 함
	{
		if (ImGui::Button("DiffuseMap"))
			Dialog->OpenDialog("Diffuse", "Select Diffuse", ".png, .jpg, .tga, .TGA, .DDS", "Texture/");

		if (ImGui::Button("NormalMap"))
			Dialog->OpenDialog("Normal", "Select Normal", ".png, .jpg, .tga, .TGA, .DDS", "Texture/");

		if (ImGui::Button("SpecularMap"))
			Dialog->OpenDialog("Specular", "Select Specular", ".png, .jpg, .tga, .TGA, .DDS", "Texture/");

		if (Dialog->Display("Diffuse") || Dialog->Display("Specular") || Dialog->Display("Normal"))
		{
			if (Dialog->IsOk())
			{
				string path = Dialog->GetFilePathName();

				//절대경로를 상대경로로 바꿔줘야 함

				path = path.substr(projectDir.size() + 1, path.size());

				wstring file = ToWString(path);

				// SetDiffuse는 wstring을 받아서 path를 wstring으로 변환시켜줘야 함
				if (Dialog->GetOpenedKey() == "Diffuse")
					SetDiffuseMap(file); 

				if (Dialog->GetOpenedKey() == "Specular")
					SetSpecularMap(file);

				if (Dialog->GetOpenedKey() == "Normal")
					SetNormalMap(file);
			}

			Dialog->Close();
		}

		ImGui::EndMenu();
	}
}

void Material::Save(wstring file)
{
	BinaryWriter data(file);

	data.WriteData(label);

	if (vertexShader)
		data.WriteData(vertexShader->GetPath());
	else
		data.WriteData(L"");

	if (pixelShader)
		data.WriteData(pixelShader->GetPath());
	else
		data.WriteData(L"");

	if (diffuseMap)
		data.WriteData(diffuseMap->GetPath());
	else
		data.WriteData(L"");

	if (specularMap)
		data.WriteData(specularMap->GetPath());
	else
		data.WriteData(L"");

	if (normalMap)
		data.WriteData(normalMap->GetPath());
	else
		data.WriteData(L"");

	data.WriteData(buffer->data.diffuse  );
	data.WriteData(buffer->data.specular );
	data.WriteData(buffer->data.ambient  );
	data.WriteData(buffer->data.emissive );

	data.WriteData(buffer->data.shininess);
}

void Material::Load(wstring file)
{
	BinaryReader data(file);

	if (!data.Succeeded())
		return;

	label = data.ReadString();

	wstring str;

	str = data.ReadWString();
	if (str != L"")
		vertexShader = Shader::GetVS(str);


	str = data.ReadWString();
	if (str != L"")
		pixelShader = Shader::GetPS(str);


	str = data.ReadWString();
	if (str != L"")
		SetDiffuseMap(str);


	str = data.ReadWString();
	if (str != L"")
		SetSpecularMap(str);

	str = data.ReadWString();
	if (str != L"")
		SetNormalMap(str);

	buffer->data.diffuse   = data.ReadVector4();
	buffer->data.specular  = data.ReadVector4();
	buffer->data.ambient   = data.ReadVector4();
	buffer->data.emissive  = data.ReadVector4();

	buffer->data.shininess = data.ReadFloat();
}

void Material::SaveDialog()
{
	if (ImGui::Button(("Save " + label).c_str()))
	{
		Dialog->OpenDialog("Save Material", "Save", ".mat", "_TextData/");
	}

	if (Dialog->Display("Save Material", 32, { 200, 100 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			Save(ToWString(path));
		}

		Dialog->Close();
	}
}

void Material::LoadDialog()
{
	if (ImGui::Button(("Load " + label).c_str()))
	{
		Dialog->OpenDialog("Load Material", "Load", ".mat", "_TextData/");
	}

	if (Dialog->Display("Load Material", 32, { 200, 100 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			Load(ToWString(path));
		}

		Dialog->Close();
	}
}
