#include "MeshRenderer.h"
#include "DirectXTex/Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ObjectManager.h"
#include "Camera.h"
#include "ObjectBase.h"
#include "Transform.h"

#include "CameraDebug.h"
#include "CameraPlayer.h"

//--- 変数宣言
VertexShader* MeshRenderer::m_pDefVS = nullptr;
PixelShader* MeshRenderer::m_pDefPS = nullptr;
unsigned int MeshRenderer::m_shaderRef = 0;
std::list<std::pair<std::string, MeshRenderer::Info*>> MeshRenderer::m_ModelList;

// 頂点情報
struct MeshRenderer::Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

// マテリアル
struct MeshRenderer::Material
{
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 specular;
	ID3D11ShaderResourceView* pTexture;
};
// メッシュ
struct MeshRenderer::Mesh
{
	Vertex* pVertices;
	unsigned int vertexNum;
	unsigned int* pIndices;
	unsigned int indexNum;
	unsigned int materialID;
	MeshBuffer* pMesh;
};

//--- プロトタイプ宣言
void MakeDefaultShader(VertexShader** vs, PixelShader** ps);

MeshRenderer::MeshRenderer()
{
}
MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Start()
{
	if (m_shaderRef == 0)
	{
		MakeDefaultShader(&m_pDefVS, &m_pDefPS);
	}
	m_pVS = m_pDefVS;
	m_pPS = m_pDefPS;
	++m_shaderRef;

	m_pWVP = std::make_shared<ConstantBuffer>();
	m_pWVP->Create(sizeof(m_Mat));

	// 行列を単位行列にするための設定
	m_Mat[0]._11 = 1.0f;
	m_Mat[0]._22 = 1.0f;
	m_Mat[0]._33 = 1.0f;
	m_Mat[0]._44 = 1.0f;
}

// 後更新
void MeshRenderer::LateUpdate() {
	// 移動行列
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(
		GetOwner()->GetComponent<Transform>()->GetPosition().x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z);
	// X回転行列
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(
		DirectX::XMConvertToRadians(GetOwner()->GetComponent<Transform>()->GetAngle().x));
	// Y回転行列
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(
		DirectX::XMConvertToRadians(GetOwner()->GetComponent<Transform>()->GetAngle().y));
	// Z回転行列
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(
		DirectX::XMConvertToRadians(GetOwner()->GetComponent<Transform>()->GetAngle().z));
	// 拡大縮小行列
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(
		GetOwner()->GetComponent<Transform>()->GetScale().x,
		GetOwner()->GetComponent<Transform>()->GetScale().y,
		GetOwner()->GetComponent<Transform>()->GetScale().z);

	// 全ての行列を一つにまとめる
	DirectX::XMMATRIX mat = S * Ry * Rx * Rz * T;

	// シェーダに渡す前に実行する処理
	mat = DirectX::XMMatrixTranspose(mat);

	DirectX::XMStoreFloat4x4(&m_Mat[0], mat);

	if (ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<Camera>()) {
		m_Mat[1] = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<Camera>()->GetViewMatrix();
		m_Mat[2] = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<Camera>()->GetProjectionMatrix();
	} else
	if (ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraDebug>()) {
		m_Mat[1] = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraDebug>()->GetViewMatrix();
		m_Mat[2] = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraDebug>()->GetProjectionMatrix();
	} else
	if (ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()) {
		m_Mat[1] = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()->GetViewMatrix();
		m_Mat[2] = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()->GetProjectionMatrix();
	}

	m_pWVP->Write(m_Mat);
}

void MeshRenderer::End()
{
	for (unsigned int i = 0; i < m_MeshInfo.m_meshNum; ++i)
	{
		delete[] m_MeshInfo.m_pMeshes[i].pVertices;
		delete[] m_MeshInfo.m_pMeshes[i].pIndices;
		delete m_MeshInfo.m_pMeshes[i].pMesh;
	}
	delete[] m_MeshInfo.m_pMeshes;

	--m_shaderRef;
	if (m_shaderRef <= 0)
	{
		delete m_pDefPS;
		delete m_pDefVS;
	}
}

void MeshRenderer::Draw()
{
	m_pVS->Bind();
	m_pPS->Bind();
	m_pWVP->BindVS(0);
	for (unsigned int i = 0; i < m_MeshInfo.m_meshNum; ++i)
	{
		SetTexturePS(m_MeshInfo.m_pMaterials[m_MeshInfo.m_pMeshes[i].materialID].pTexture, 0);
		m_MeshInfo.m_pMeshes[i].pMesh->Draw();
	}
}

void MeshRenderer::SetVertexShader(VertexShader* vs)
{
	m_pVS = vs;
}
void MeshRenderer::SetPixelShader(PixelShader* ps)
{
	m_pPS = ps;
}
const MeshRenderer::Mesh* MeshRenderer::GetMesh(unsigned int index)
{
	if (index >= GetMeshNum())
	{
		return &m_MeshInfo.m_pMeshes[index];
	}
	return nullptr;
}
uint32_t MeshRenderer::GetMeshNum()
{
	return m_MeshInfo.m_meshNum;
}

void MakeDefaultShader(VertexShader** vs, PixelShader** ps)
{
	*vs = new VertexShader();
	if (FAILED((*vs)->Load("Assets/Shader/ModelVS.cso"))) {
		MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);
	}
	*ps = new PixelShader();
	if (FAILED((*ps)->Load("Assets/Shader/ModelPS.cso"))) {
		MessageBox(nullptr, "ModelPS.cso", "Error", MB_OK);
	}
}

bool MeshRenderer::LoadModel(const char* file, float scale)
{
	// 登録済みのモデルが見つかったら
	// ロードせずに登録済みのモデルデータを返す。
	for (auto it = m_ModelList.begin(); it != m_ModelList.end(); it++)
		if (it->first == file) {
			m_MeshInfo = *it->second;
			return true;
		}

	// assimpの読み込み時の設定
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_FlipUVs;
	//	if (flip) flag |= aiProcess_MakeLeftHanded;
		// assimpで読み込み
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene) {
		MessageBox(nullptr, "モデルファイルをセットしてください", "Error", MB_OK);
		return false;
	}
	// 読み込んだデータを基にメッシュのデータを確保
	m_MeshInfo.m_meshNum = pScene->mNumMeshes;
	m_MeshInfo.m_pMeshes = new Mesh[m_MeshInfo.m_meshNum];

	// メッシュごとに頂点データ、インデックスデータを読み取り
	for (unsigned int i = 0; i < m_MeshInfo.m_meshNum; ++i) {
		// メッシュを基に頂点のデータを確保
		aiVector3D zero(0.0f, 0.0f, 0.0f);
		m_MeshInfo.m_pMeshes[i].vertexNum = pScene->mMeshes[i]->mNumVertices;
		m_MeshInfo.m_pMeshes[i].pVertices = new MeshRenderer::Vertex[m_MeshInfo.m_pMeshes[i].vertexNum];

		// メッシュ内の頂点データを読み取り
		for (unsigned int j = 0; j < m_MeshInfo.m_pMeshes[i].vertexNum; ++j) {
			// 値の吸出し
			aiVector3D pos = pScene->mMeshes[i]->mVertices[j];
			aiVector3D uv = pScene->mMeshes[i]->HasTextureCoords(0) ?
				pScene->mMeshes[i]->mTextureCoords[0][j] : zero;
			// 値を設定
			m_MeshInfo.m_pMeshes[i].pVertices[j] = {
				DirectX::XMFLOAT3(pos.x * scale, pos.y * scale, pos.z * scale),
				DirectX::XMFLOAT2(uv.x, uv.y)
			};
		}

		// メッシュを元にインデックスのデータを確保
		// ※faceはポリゴンの数を表す（１ポリゴンで3インデックス
		m_MeshInfo.m_pMeshes[i].indexNum = pScene->mMeshes[i]->mNumFaces * 3;
		m_MeshInfo.m_pMeshes[i].pIndices = new unsigned int[m_MeshInfo.m_pMeshes[i].indexNum];

		// メッシュ内のインデックスデータを読み取り
		for (unsigned int j = 0; j < pScene->mMeshes[i]->mNumFaces; ++j) {
			aiFace face = pScene->mMeshes[i]->mFaces[j];
			int idx = j * 3;
			m_MeshInfo.m_pMeshes[i].pIndices[idx + 0] = face.mIndices[0];
			m_MeshInfo.m_pMeshes[i].pIndices[idx + 1] = face.mIndices[1];
			m_MeshInfo.m_pMeshes[i].pIndices[idx + 2] = face.mIndices[2];
		}

		// マテリアルの割り当て
		m_MeshInfo.m_pMeshes[i].materialID = pScene->mMeshes[i]->mMaterialIndex;

		// メッシュを元に頂点バッファ作成
		MeshBuffer::Description desc = {};
		desc.pVtx = m_MeshInfo.m_pMeshes[i].pVertices;
		desc.vtxSize = sizeof(Vertex);
		desc.vtxCount = m_MeshInfo.m_pMeshes[i].vertexNum;
		desc.pIdx = m_MeshInfo.m_pMeshes[i].pIndices;
		desc.idxSize = sizeof(unsigned int);
		desc.idxCount = m_MeshInfo.m_pMeshes[i].indexNum;
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_MeshInfo.m_pMeshes[i].pMesh = new MeshBuffer(desc);
	}

	// テクスチャを読み込む場所を探索
	std::string dir = file;
	dir = dir.substr(0, dir.find_last_of('/') + 1);	// 読み込むファイルパスからファイル名を取り除く
													// Assets/Model/xx.fbx → Assets/Model/
	// 読み込んだデータを元にマテリアルのデータの確保
	m_MeshInfo.m_materialNum = pScene->mNumMaterials;
	m_MeshInfo.m_pMaterials = new Material[m_MeshInfo.m_materialNum];

	// マテリアルごとにデータの読み取り
	HRESULT hr;
	for (unsigned int i = 0; i < m_MeshInfo.m_materialNum; ++i) {
		// テクスチャの読み取り
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			// モデルファイルに記録されていたファイルパスから読み込み
			hr = LoadTextureFromFile(path.C_Str(), &m_MeshInfo.m_pMaterials[i].pTexture);
			if (FAILED(hr)) {
				// モデルと同じフォルダ内でテクスチャファイルを読み込み
				std::string file = dir;
				file += path.C_Str();
				hr = LoadTextureFromFile(file.c_str(), &m_MeshInfo.m_pMaterials[i].pTexture);
			}
			if (FAILED(hr)) {
				// モデルと同じフォルダ内で、
				// 一時的なDemoCube用だが、今後使うかも・使えるかもしれない。
				std::string file = path.C_Str();
				// [..\\sourceimages\\]を省略
				file = file.substr(16);
				file = dir + "/" + file;
				hr = LoadTextureFromFile(file.c_str(), &m_MeshInfo.m_pMaterials[i].pTexture);
			}
			if (FAILED(hr)) { 
				MessageBox(nullptr, "モデルファイルと同じフォルダに指定のテクスチャが見つかりません", "Error", MB_OK);
				return false;
			}
		}
		else {
			m_MeshInfo.m_pMaterials[i].pTexture = nullptr;
		}
	}

	// 登録する
	m_ModelList.push_back(std::pair<std::string, Info*>(file, &m_MeshInfo));
	return true;
}
