#include "Framework.h"
#include "Texture.h"

map<wstring, Texture*> Texture::textures = {};

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
	:srv(srv), image(move(image)) 
// move : 이동 생성자 => 얕은 복사, 이전의 주소값을 받아오는 것을 끊고 지금 객체에만 주소값을 들고 있게 함, 메모리를 아낄 수 있음
// 안해주면 image가 삭제된 파일이라고 나옴
{

}

Texture::~Texture()
{
	if (!isReferred) // 참조된 것은 RenderTarget에서 지워주기 때문
		if (srv != nullptr)
			srv->Release();
}

Texture* Texture::Get(wstring file)
{
	wstring path = file;

	//if (file == L"Texture/Model/Pumpkin/Pumpkin.fbm\\Pumpkin_DefaultMaterial_BaseColor.png")
	//	file = L"Texture/Model/Pumpkin/Pumpkin_DefaultMaterial_BaseColor.png";

	if (file == L"")
		return nullptr;

	if (!StartsWith(file, L"Texture")) 	// 있으면 있는거 리턴
		file = L"Texture/" + file;

	//if (file == L"Texture/Model/player/player robot 1.fbm\\M_Alliance.png")
	//	file = L"Texture/Model/player\\M_Alliance.png";

	assert(PathFileExists(file.c_str())); // 없는 파일을 추가했을때 실제로 파일이 있는지 확인(PathFileExists)하고 없으면 중단(assert)

	if (textures.count(file) > 0)
		return textures[file];

	wstring extension = GetExtension(file);

	// 없으면 만들어서 리턴
	ScratchImage image;

	if (extension == L"tga" || extension == L"TGA")
		LoadFromTGAFile(file.c_str(), nullptr, image);

	else if (extension == L"dds" || extension == L"DDS")
		LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);

	else
		LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image); // ScratchImage가 이동 생성자여서 이 함수가 끝나도 지역변수인 image 가 삭제되지 않음 소유권을 여기서 넘겼기 때문


	ID3D11ShaderResourceView* srv = nullptr;

	CreateShaderResourceView
	(
		DEVICE,
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		&srv
	);

	textures[file] = new Texture(srv, image);

	textures[file]->path = path;

	return textures[file];
}

Texture* Texture::Get(wstring key, ID3D11ShaderResourceView* srv)
{
	if (textures.count(key) > 0)
		return textures[key];

	ScratchImage image;

	textures.emplace(key, new Texture(srv, image));
	textures[key]->isReferred = true;

	return textures[key];
}

Texture* Texture::Load(wstring file)
{
	wstring path = file; // 멤버변수인 path는 static변수라서 같은 static변수나 함수 포인터만 받음

	if (file == L"")
		return nullptr;

	// 있으면 있는거 리턴
	file = L"Texture/" + file;

	assert(PathFileExists(file.c_str())); // 없는 파일을 추가했을때 실제로 파일이 있는지 확인(PathFileExists)하고 없으면 중단(assert)

	if (textures.count(file) > 0)
	{
		textures.erase(file);
	}

	wstring extension = GetExtension(file);

	// 없으면 만들어서 리턴
	ScratchImage image; // 위의 path와 같은 경우

	if (extension == L"tga" || extension == L"TGA")
		LoadFromTGAFile(file.c_str(), nullptr, image);

	else if (extension == L"dds" || extension == L"DDS")
		LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);

	else
		LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image); // ScratchImage가 이동 생성자여서 이 함수가 끝나도 지역변수인 image 가 삭제되지 않음 소유권을 여기서 넘겼기 때문


	ID3D11ShaderResourceView* srv = nullptr;

	CreateShaderResourceView
	(
		DEVICE,
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		&srv
	);

	textures[file] = new Texture(srv, image);

	textures[file]->path = path;

	return textures[file];
}

void Texture::Delete()
{
	for (pair<wstring, Texture*> pair : textures)
		delete pair.second;

	textures.clear();
}

void Texture::PSSetShaderResources(UINT slot)
{
	DC->PSSetShaderResources(slot, 1, &srv);
}

vector<Vector4> Texture::ReadPixels()
{
	unsigned char* pixels = image.GetPixels(); // 리턴값을 uint8_t(unsigned char)*로 받음 -> 배열의 주소가 연속적이여서 첫번째 주소값만 알아도 나머지 값을 다 가져올 수 있다.

	UINT size = image.GetPixelsSize() * 0.25f; // 픽셀 갯수

	vector<Vector4> colors(size); // = color; color.resize(size)

	float scale = 1.0f / 255.0f;

	UINT count = 0;

	for (Vector4& color : colors) // call by reference로 원본값을 변환시켜줌
	{
		color.x = pixels[count++] * scale;
		color.y = pixels[count++] * scale;
		color.z = pixels[count++] * scale;
		color.w = pixels[count++] * scale;
	}

	return colors;
}
