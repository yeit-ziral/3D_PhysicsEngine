#include "Framework.h"
#include "BinaryReader.h"

BinaryReader::BinaryReader(string path)
{
	if (!StartsWith(path, "_")) 	// 있으면 있는거 리턴
		path = "_TextData/" + path;

	file = CreateFileA
	(
		path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
}

BinaryReader::BinaryReader(wstring path)
{
	if (!StartsWith(path, L"_")) 	// 있으면 있는거 리턴
		path = L"_TextData/" + path;

	file = CreateFile
	(
		path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
}

BinaryReader::~BinaryReader()
{
	CloseHandle(file);
}

int BinaryReader::ReadInt()
{
	int data;

	ReadFile(file, &data, sizeof(int), &size, nullptr); // 작성한 순서대로 읽어야 함

    return data;
}

UINT BinaryReader::ReadUINT()
{
	UINT data;

	ReadFile(file, &data, sizeof(UINT), &size, nullptr); // 작성한 순서대로 읽어야 함

	return data;
}

float BinaryReader::ReadFloat()
{
	float data;

	assert(ReadFile(file, &data, sizeof(float), &size, nullptr)); // 작성한 순서대로 읽어야 함

	return data;
}

string BinaryReader::ReadString()
{
	UINT dataSize = ReadUINT();

	char* data = new char[dataSize + 1]; // 문자열을 끝낸다는 null문자(back slash)를 문자열 맨 끝에 붙여주기 때문


	ReadFile(file, data, sizeof(char) * dataSize, &size, nullptr);

	data[dataSize] = '\0';

	string str = data;

	delete[] data;

    return str;
}

wstring BinaryReader::ReadWString()
{
	UINT dataSize = ReadUINT();

	wchar_t* data = new wchar_t[dataSize + 1]; // 문자열을 끝낸다는 null문자(back slash)를 문자열 맨 끝에 붙여주기 때문

	ReadFile(file, data, sizeof(wchar_t) * dataSize, &size, nullptr);

	data[dataSize] = '\0';

	return data;
}

Vector3 BinaryReader::ReadVector3()
{
	Vector3 data;

	data.x = ReadFloat();
	data.y = ReadFloat();
	data.z = ReadFloat();

    return data;
}

Vector4 BinaryReader::ReadVector4()
{
	Vector4 data;

	data.x = ReadFloat();
	data.y = ReadFloat();
	data.z = ReadFloat();
	data.w = ReadFloat();

	return data;
}

XMFLOAT4X4 BinaryReader::ReadFloat4X4()
{
	XMFLOAT4X4 data;

	ReadFile(file, &data, sizeof(data), &size, nullptr);

	return data;
}

Matrix BinaryReader::ReadMatrix()
{
	Matrix data;

	ReadFile(file, &data, sizeof(data), &size, nullptr);

	return data;
}

void BinaryReader::ReadData(OUT void** data, UINT dataSize)
{
	ReadFile(file, *data, dataSize, &size, nullptr);
}

bool BinaryReader::Succeeded()
{
    return file != INVALID_HANDLE_VALUE;
}
