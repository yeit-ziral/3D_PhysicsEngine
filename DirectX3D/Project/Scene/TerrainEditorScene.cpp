#include "Framework.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
{
	terrainEditor = new TerrainEditor(200, 200);


	//RawData();

	wstring str = GetProjectDir();

	BinaryWriter data(L"Data");
	data.WriteData(10);
	data.WriteData(1.5f);
	data.WriteData("HelloWorld");
}

TerrainEditorScene::~TerrainEditorScene()
{
	delete terrainEditor;
}

void TerrainEditorScene::Update()
{
	terrainEditor->Update();
}

void TerrainEditorScene::PreRender()
{
}

void TerrainEditorScene::Render()
{
	terrainEditor->Render();
}

void TerrainEditorScene::PostRender()
{
	terrainEditor->Debug();
	//terrainEditor->GetMaterial()->PostRender();
}

void TerrainEditorScene::RawData()
{
	ComputeShader* shader = Shader::GetCS(L"ByteAddress");

	struct Output
	{
		UINT groupID[3];
		UINT groupThreadID[3];
		UINT dispatchThreadID[3];
		UINT groupIndex;
	};

	UINT size = 10 * 8 * 3 * 2;

	Output* output = new Output[size];

	RawBuffer* buffer = new RawBuffer(nullptr, sizeof(Output) * size);

	ID3D11UnorderedAccessView* uav = buffer->GetUAV();

	shader->SetShader();

	DC->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	DC->Dispatch(2, 1, 1); // computeShader에서 가장 중요한 함수, compute shade의 시작을 알리는 함수

	buffer->Copy(output, sizeof(Output)* size);

	FILE* file;
	fopen_s(&file, "_TextData/RawTest.csv", "w");

	for (UINT i = 0; i < size; i++)
	{
		fprintf
		(
			file,
			"%d, %d,%d,%d, %d,%d,%d, %d,%d,%d, %d\n",
			i,
			output[i].groupID[0], // 첫번째 3쌍
			output[i].groupID[1],
			output[i].groupID[2],
			output[i].groupThreadID[0], // 두번째 3쌍
			output[i].groupThreadID[1],
			output[i].groupThreadID[2],
			output[i].dispatchThreadID[0],
			output[i].dispatchThreadID[1],
			output[i].dispatchThreadID[2],
			output[i].groupIndex // 그룹 내의 인덱스
		);
	}

	fclose(file);

	delete[] output;
}