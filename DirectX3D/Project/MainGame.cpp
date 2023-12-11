#include "Framework.h"
#include "MainGame.h"

MainGame::MainGame()
{
	srand(time(NULL));

	Initialize();

	//scene = new TextureScene();
	//scene = new TutorialScene();
	//scene = new RobotScene();
	//scene = new BoxScene();
	//scene = new TerrainScene();
	//scene = new SphereScene();
	//scene = new TerrainEditorScene();
	//scene = new ModelExportScene();
	//scene = new ModelAnimationScene();
	//scene = new CollisionScene();

	SCENE->Create("Grid", new GridScene());
	//SCENE->Create("Collision", new CollisionScene());
	//SCENE->Create("ModelExport", new ModelExportScene);
	//SCENE->Create("ModelAnimation", new LightScene);
	//SCENE->Create("Shooting", new ShootingScene);
	SCENE->Create("Start", new ExplosionScene());
	//SCENE->Create("Start", new BlockScene());

	//SCENE->Add("Grid");
	//SCENE->Add("Collision");
	//SCENE->Add("ModelExport");
	//SCENE->Add("ModelAnimation");
	//SCENE->Add("Shooting");
	SCENE->Add("Start");

	FONT->Add("D2Coding", L"D2Coding");
}

MainGame::~MainGame()
{
	Release();

	//FONT->Delete();
}

void MainGame::Update()
{
	SCENE->Update();

	Time::GetInstance()->Update();
	Keyboard::GetInstance()->Update();

	//if (KEY_PRESS('0')) // Scene 바꿈
	//{
	//	SCENE->Remove("Start");
	//	SCENE->Add("Grid");
	//}
}

void MainGame::Render()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (isWireFrame)
	{
		RS->ChangeState(D3D11_FILL_WIREFRAME);
	}
	else
	{
		RS->ChangeState(D3D11_FILL_SOLID);
	}

	SCENE->PreRender();

	Device::GetInstance()->Clear();

	Environment::GetInstance()->SetEnvironment();

	SCENE->Render();
	Time::GetInstance()->Render();

	Environment::GetInstance()->PostSet();

	SCENE->PostRender();


	Environment::GetInstance()->PostRender();

	ImGui::Checkbox("WireFrame", &isWireFrame);

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Render 뒤에만 있으면 맨 앞에 그려짐

	Device::GetInstance()->Present();
}

void MainGame::Initialize()
{
			Device::GetInstance(); // 안해줘도 생성시 만들어짐
	   Environment::GetInstance();
		  Keyboard::GetInstance();
			  Time::GetInstance();
	  StateManager::GetInstance();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);

}

void MainGame::Release()
{
	Device::Delete();
	Shader::Delete();
	Environment::Delete();
	StateManager::Delete();
	Texture::Delete();
	SceneManager::Delete();

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
