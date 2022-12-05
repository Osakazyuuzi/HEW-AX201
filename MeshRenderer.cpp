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

//--- �ϐ��錾
VertexShader* MeshRenderer::m_pDefVS = nullptr;
PixelShader* MeshRenderer::m_pDefPS = nullptr;
unsigned int MeshRenderer::m_shaderRef = 0;
std::list<std::pair<std::string, MeshRenderer::Info*>> MeshRenderer::m_ModelList;

// ���_���
struct MeshRenderer::Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

// �}�e���A��
struct MeshRenderer::Material
{
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 specular;
	ID3D11ShaderResourceView* pTexture;
};
// ���b�V��
struct MeshRenderer::Mesh
{
	Vertex* pVertices;
	unsigned int vertexNum;
	unsigned int* pIndices;
	unsigned int indexNum;
	unsigned int materialID;
	MeshBuffer* pMesh;
};

//--- �v���g�^�C�v�錾
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

	// �s���P�ʍs��ɂ��邽�߂̐ݒ�
	m_Mat[0]._11 = 1.0f;
	m_Mat[0]._22 = 1.0f;
	m_Mat[0]._33 = 1.0f;
	m_Mat[0]._44 = 1.0f;
}

// ��X�V
void MeshRenderer::LateUpdate() {
	// �ړ��s��
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(
		GetOwner()->GetComponent<Transform>()->GetPosition().x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z);
	// X��]�s��
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(
		DirectX::XMConvertToRadians(GetOwner()->GetComponent<Transform>()->GetAngle().x));
	// Y��]�s��
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(
		DirectX::XMConvertToRadians(GetOwner()->GetComponent<Transform>()->GetAngle().y));
	// Z��]�s��
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(
		DirectX::XMConvertToRadians(GetOwner()->GetComponent<Transform>()->GetAngle().z));
	// �g��k���s��
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(
		GetOwner()->GetComponent<Transform>()->GetScale().x,
		GetOwner()->GetComponent<Transform>()->GetScale().y,
		GetOwner()->GetComponent<Transform>()->GetScale().z);

	// �S�Ă̍s�����ɂ܂Ƃ߂�
	DirectX::XMMATRIX mat = S * Ry * Rx * Rz * T;

	// �V�F�[�_�ɓn���O�Ɏ��s���鏈��
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
	// �o�^�ς݂̃��f��������������
	// ���[�h�����ɓo�^�ς݂̃��f���f�[�^��Ԃ��B
	for (auto it = m_ModelList.begin(); it != m_ModelList.end(); it++)
		if (it->first == file) {
			m_MeshInfo = *it->second;
			return true;
		}

	// assimp�̓ǂݍ��ݎ��̐ݒ�
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_FlipUVs;
	//	if (flip) flag |= aiProcess_MakeLeftHanded;
		// assimp�œǂݍ���
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene) {
		MessageBox(nullptr, "���f���t�@�C�����Z�b�g���Ă�������", "Error", MB_OK);
		return false;
	}
	// �ǂݍ��񂾃f�[�^����Ƀ��b�V���̃f�[�^���m��
	m_MeshInfo.m_meshNum = pScene->mNumMeshes;
	m_MeshInfo.m_pMeshes = new Mesh[m_MeshInfo.m_meshNum];

	// ���b�V�����Ƃɒ��_�f�[�^�A�C���f�b�N�X�f�[�^��ǂݎ��
	for (unsigned int i = 0; i < m_MeshInfo.m_meshNum; ++i) {
		// ���b�V������ɒ��_�̃f�[�^���m��
		aiVector3D zero(0.0f, 0.0f, 0.0f);
		m_MeshInfo.m_pMeshes[i].vertexNum = pScene->mMeshes[i]->mNumVertices;
		m_MeshInfo.m_pMeshes[i].pVertices = new MeshRenderer::Vertex[m_MeshInfo.m_pMeshes[i].vertexNum];

		// ���b�V�����̒��_�f�[�^��ǂݎ��
		for (unsigned int j = 0; j < m_MeshInfo.m_pMeshes[i].vertexNum; ++j) {
			// �l�̋z�o��
			aiVector3D pos = pScene->mMeshes[i]->mVertices[j];
			aiVector3D uv = pScene->mMeshes[i]->HasTextureCoords(0) ?
				pScene->mMeshes[i]->mTextureCoords[0][j] : zero;
			// �l��ݒ�
			m_MeshInfo.m_pMeshes[i].pVertices[j] = {
				DirectX::XMFLOAT3(pos.x * scale, pos.y * scale, pos.z * scale),
				DirectX::XMFLOAT2(uv.x, uv.y)
			};
		}

		// ���b�V�������ɃC���f�b�N�X�̃f�[�^���m��
		// ��face�̓|���S���̐���\���i�P�|���S����3�C���f�b�N�X
		m_MeshInfo.m_pMeshes[i].indexNum = pScene->mMeshes[i]->mNumFaces * 3;
		m_MeshInfo.m_pMeshes[i].pIndices = new unsigned int[m_MeshInfo.m_pMeshes[i].indexNum];

		// ���b�V�����̃C���f�b�N�X�f�[�^��ǂݎ��
		for (unsigned int j = 0; j < pScene->mMeshes[i]->mNumFaces; ++j) {
			aiFace face = pScene->mMeshes[i]->mFaces[j];
			int idx = j * 3;
			m_MeshInfo.m_pMeshes[i].pIndices[idx + 0] = face.mIndices[0];
			m_MeshInfo.m_pMeshes[i].pIndices[idx + 1] = face.mIndices[1];
			m_MeshInfo.m_pMeshes[i].pIndices[idx + 2] = face.mIndices[2];
		}

		// �}�e���A���̊��蓖��
		m_MeshInfo.m_pMeshes[i].materialID = pScene->mMeshes[i]->mMaterialIndex;

		// ���b�V�������ɒ��_�o�b�t�@�쐬
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

	// �e�N�X�`����ǂݍ��ޏꏊ��T��
	std::string dir = file;
	dir = dir.substr(0, dir.find_last_of('/') + 1);	// �ǂݍ��ރt�@�C���p�X����t�@�C��������菜��
													// Assets/Model/xx.fbx �� Assets/Model/
	// �ǂݍ��񂾃f�[�^�����Ƀ}�e���A���̃f�[�^�̊m��
	m_MeshInfo.m_materialNum = pScene->mNumMaterials;
	m_MeshInfo.m_pMaterials = new Material[m_MeshInfo.m_materialNum];

	// �}�e���A�����ƂɃf�[�^�̓ǂݎ��
	HRESULT hr;
	for (unsigned int i = 0; i < m_MeshInfo.m_materialNum; ++i) {
		// �e�N�X�`���̓ǂݎ��
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			// ���f���t�@�C���ɋL�^����Ă����t�@�C���p�X����ǂݍ���
			hr = LoadTextureFromFile(path.C_Str(), &m_MeshInfo.m_pMaterials[i].pTexture);
			if (FAILED(hr)) {
				// ���f���Ɠ����t�H���_���Ńe�N�X�`���t�@�C����ǂݍ���
				std::string file = dir;
				file += path.C_Str();
				hr = LoadTextureFromFile(file.c_str(), &m_MeshInfo.m_pMaterials[i].pTexture);
			}
			if (FAILED(hr)) { 
				MessageBox(nullptr, "���f���t�@�C���Ɠ����t�H���_�Ɏw��̃e�N�X�`����������܂���", "Error", MB_OK);
				return false;
			}
		}
		else {
			m_MeshInfo.m_pMaterials[i].pTexture = nullptr;
		}
	}

	// �o�^����
	m_ModelList.push_back(std::pair<std::string, Info*>(file, &m_MeshInfo));
	return true;
}