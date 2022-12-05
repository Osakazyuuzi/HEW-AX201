#include "Game3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Input.h"

Game3D::Game3D()
{
	// タイトルシーンへ変更
	SceneManager::LoadScene(SceneName::SceneTitle);
}
Game3D::~Game3D()
{
}
void Game3D::Update()
{
	ObjectManager::FixedUpdate();
	ObjectManager::Update();

	if (IsKeyPress(VK_F1)) SceneManager::LoadScene(SceneName::SceneTitle);
	if (IsKeyPress(VK_F2)) SceneManager::LoadScene(SceneName::SceneGame01);
}

void Game3D::LateUpdate()
{
	ObjectManager::LateUpdate();
}

void Game3D::Draw()
{
	ObjectManager::Draw();
}