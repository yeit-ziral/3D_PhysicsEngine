#include "Framework.h"
#include "TextureCube.h"

TextureCube::TextureCube()
{
	CreateQuads();
}

TextureCube::~TextureCube()
{
	for (Quad* quad : quads)
		delete quad;

	quads.clear();
}

void TextureCube::Update()
{
	for (Quad* quad : quads)
		quad->Update();

	Transform::Update();
}

void TextureCube::Render()
{
	for (Quad* quad : quads)
		quad->Render();
}

void TextureCube::CreateQuads()
{
	quads.resize(6);

	quads[Left] = new Quad();
	quads[Left]->translation.x = -0.5f;
	quads[Left]->rotation.y = +XM_PIDIV2;
	quads[Left]->SetParent(this);

	quads[Right] = new Quad();
	quads[Right]->translation.x = +0.5f;
	quads[Right]->rotation.y = -XM_PIDIV2;
	quads[Right]->SetParent(this);

	quads[Bottom] = new Quad();
	quads[Bottom]->translation.y = -0.5f;
	quads[Bottom]->rotation.x = -XM_PIDIV2;
	quads[Bottom]->SetParent(this);

	quads[Top] = new Quad();
	quads[Top]->translation.y = +0.5f;
	quads[Top]->rotation.x = +XM_PIDIV2;
	quads[Top]->SetParent(this);

	quads[Front] = new Quad();
	quads[Front]->translation.z = -0.5f;
	//quads[Front]->rotation.z = +XM_PIDIV2;
	quads[Front]->SetParent(this);

	quads[Back] = new Quad();
	quads[Back]->translation.z = +0.5f;
	quads[Back]->rotation.x = -XM_PI;
	quads[Back]->SetParent(this);
}
