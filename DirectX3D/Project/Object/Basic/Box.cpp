#include "Framework.h"
#include "Box.h"

Box::Box()
{
	cube = new Cube({ 1.0f, 0.0f, 0.0f, 1.0f });
	quadTop = new Quad(cube->GetTopVertices());
	quadBottom = new Quad(cube->GetBottomVertices());
	quadFront = new Quad(cube->GetFrontVertices());
	quadBack = new Quad(cube->GetBackVertices());
	quadRight = new Quad(cube->GetRightVertices());
	quadLeft = new Quad(cube->GetLeftVertices());

	   quadTop->SetParent(cube);
	quadBottom->SetParent(cube);
	 quadFront->SetParent(cube);
	  quadBack->SetParent(cube);
	 quadRight->SetParent(cube);
	  quadLeft->SetParent(cube);


	  collider = new ColliderBox({ 2.0f, 2.0f, 2.0f });
}

Box::~Box()
{
	delete    quadTop;
	delete quadBottom;
	delete  quadFront;
	delete   quadBack;
	delete  quadRight;
	delete   quadLeft;

	delete collider;
}

void Box::Update()
{
		  cube->Update();
	   quadTop->Update();
	quadBottom->Update();
	 quadFront->Update();
	  quadBack->Update();
	 quadRight->Update();
	  quadLeft->Update();

	  collider->Update();
}

void Box::Render()
{
		  //cube->Render();
	   quadTop->Render();
	quadBottom->Render();
	 quadFront->Render();
	  quadBack->Render();
	 quadRight->Render();
	  quadLeft->Render();

	  collider->Render();
}

void Box::PostRender()
{
	cube->Debug();

	collider->Debug();
}
