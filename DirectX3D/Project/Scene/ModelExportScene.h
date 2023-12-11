#pragma once
class ModelExportScene : public Scene
{
public:
	ModelExportScene();
	~ModelExportScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	ModelExporter* exporter;

	vector<BodyPart*> bodyparts;

	ModelReader* reader;

	//ModelA* model;

	Model* model;

	ModelAnimator* modelAnimator;

	Groot* groot;
	Ghost* pumpkin;
};