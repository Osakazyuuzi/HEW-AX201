#include "SpriteRenderer.h"
#include "Shader.h"
#include "ObjectBase.h"
#include "Transform.h"

VertexShader* SpriteRenderer::m_pDefVS = nullptr;
PixelShader* SpriteRenderer::m_pDefPS = nullptr;
unsigned int SpriteRenderer::m_shaderRef = 0;
std::list<std::pair<std::string, ID3D11ShaderResourceView*>> SpriteRenderer::m_TextureList;

// コンストラクタ
SpriteRenderer::SpriteRenderer()
{

}

// デストラクタ
SpriteRenderer::~SpriteRenderer()
{
	if(m_SpriteInfo.pVtxBuf)
		delete m_SpriteInfo.pVtxBuf;
}

// テクスチャ読み込み
ID3D11ShaderResourceView* SpriteRenderer::LoadTexture(std::string file)
{
	// 登録済みのテクスチャが見つかったら
	// ロードせずに登録済みのテクスチャデータを返す。
	for (auto it = m_TextureList.begin(); it != m_TextureList.end(); it++)
		if (it->first == file) {
			m_SpriteInfo.pTexture = it->second;
			return it->second;
		}

	// 見つからなかったらテクスチャを新しくロードし、
	// ファイルネームと共にテクスチャ情報を保存しておく。
	ID3D11ShaderResourceView* texture;
	LoadTextureFromFile(file.c_str(), &texture);
	m_TextureList.push_back(std::pair<std::string, ID3D11ShaderResourceView*>(file, texture));
	m_SpriteInfo.pTexture = texture;
	return texture;
}

void SpriteRenderer::Start()
{
	if (m_shaderRef == 0)
	{
		m_pDefVS = new VertexShader();
		if (FAILED(m_pDefVS->Load("Assets/Shader/ModelVS.cso"))) {
			MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);
		}
		m_pDefPS = new PixelShader();
		if (FAILED(m_pDefPS->Load("Assets/Shader/ModelPS.cso"))) {
			MessageBox(nullptr, "ModelPS.cso", "Error", MB_OK);
		}
	}
	++m_shaderRef;
	m_pVS = m_pDefVS;
	m_pPS = m_pDefPS;

	m_pConst = new ConstantBuffer;
	m_pConst->Create(sizeof(m_SpriteInfo.m_Mat));

	// 行列を単位行列にするための設定
	m_SpriteInfo.m_Mat[0]._11 = 1.0f;
	m_SpriteInfo.m_Mat[0]._22 = 1.0f;
	m_SpriteInfo.m_Mat[0]._33 = 1.0f;
	m_SpriteInfo.m_Mat[0]._44 = 1.0f;
}

void SpriteRenderer::LateUpdate()
{
	//--- UIはオブジェクトの手前に描画する
	// 0.25 上下0.5/√3 [720]
	// 移動行列
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(
		GetOwner()->GetComponent<Transform>()->GetPosition().x / 720 * (0.5f / std::sqrtf(3.0f)),
		GetOwner()->GetComponent<Transform>()->GetPosition().y / 720 * (0.5f / std::sqrtf(3.0f)),
		GetOwner()->GetLayerNum() * 1e-8f);
	// X回転行列
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(
		0.0f/*GetOwner()->GetComponent<Transform>()->GetAngle().x*/);
	// Y回転行列
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(
		0.0f/*GetOwner()->GetComponent<Transform>()->GetAngle().y*/);
	// Z回転行列
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(
		GetOwner()->GetComponent<Transform>()->GetAngle().z);
	// 拡大縮小行列
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(
		GetOwner()->GetComponent<Transform>()->GetScale().x / 720 * (0.5f / std::sqrtf(3.0f)),
		GetOwner()->GetComponent<Transform>()->GetScale().y / 720 * (0.5f / std::sqrtf(3.0f)),
		1.0f/*GetOwner()->GetComponent<Transform>()->GetScale().z*/);

	// 全ての行列を一つにまとめる
	DirectX::XMMATRIX mat = S * Rz * Ry * Rx * T;

	// シェーダに渡す前に実行する処理
	mat = DirectX::XMMatrixTranspose(mat);

	DirectX::XMStoreFloat4x4(&m_SpriteInfo.m_Mat[0], mat);

	DirectX::XMStoreFloat4x4(&m_SpriteInfo.m_Mat[1], 
		DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, 0.25f, 0),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0))));
	DirectX::XMStoreFloat4x4(&m_SpriteInfo.m_Mat[2], 
		DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(60.0f), 16.0f / 9.0f, 0.2f, 0.3f)));

	m_pConst->Write(m_SpriteInfo.m_Mat);
}

// 描画
void SpriteRenderer::Draw()
{
	m_pVS->Bind();
	m_pPS->Bind();
	m_pConst->BindVS(0);

	// テクスチャ設定
	SetTexturePS(m_SpriteInfo.pTexture, 0);

	// 描画
	m_SpriteInfo.pVtxBuf->Draw();
}

void SpriteRenderer::End()
{
	// テクスチャを解放
	for (auto it = m_TextureList.begin(); it != m_TextureList.end(); it++)
		it->second->Release();
	m_TextureList.clear();
	delete m_pDefPS;
	delete m_pDefVS;
	delete m_pConst;
}

void SpriteRenderer::SetSize(float width, float height)
{
	Vertex vtx[4] = {
		{{-(width / 2), -(height / 2), 0.0f}, {0.0f, 0.0f}},
		{{ (width / 2), -(height / 2), 0.0f}, {1.0f, 0.0f}},
		{{-(width / 2),  (height / 2), 0.0f}, {0.0f, 1.0f}},
		{{ (width / 2),  (height / 2), 0.0f}, {1.0f, 1.0f}} };
	m_SpriteInfo.pVtxBuf = new VertexBuffer;
	m_SpriteInfo.pVtxBuf->Create(vtx, 4);
}