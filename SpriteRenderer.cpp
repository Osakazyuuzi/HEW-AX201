#include "SpriteRenderer.h"
#include "Shader.h"
#include "ObjectBase.h"
#include "Transform.h"

VertexShader* SpriteRenderer::m_pDefVS = nullptr;
PixelShader* SpriteRenderer::m_pDefPS = nullptr;
unsigned int SpriteRenderer::m_shaderRef = 0;
std::list<std::pair<std::string, ID3D11ShaderResourceView*>> SpriteRenderer::m_TextureList;

// �R���X�g���N�^
SpriteRenderer::SpriteRenderer()
{

}

// �f�X�g���N�^
SpriteRenderer::~SpriteRenderer()
{
	if(m_SpriteInfo.pVtxBuf)
		delete m_SpriteInfo.pVtxBuf;
}

// �e�N�X�`���ǂݍ���
ID3D11ShaderResourceView* SpriteRenderer::LoadTexture(std::string file)
{
	// �o�^�ς݂̃e�N�X�`��������������
	// ���[�h�����ɓo�^�ς݂̃e�N�X�`���f�[�^��Ԃ��B
	for (auto it = m_TextureList.begin(); it != m_TextureList.end(); it++)
		if (it->first == file) {
			m_SpriteInfo.pTexture = it->second;
			return it->second;
		}

	// ������Ȃ�������e�N�X�`����V�������[�h���A
	// �t�@�C���l�[���Ƌ��Ƀe�N�X�`������ۑ����Ă����B
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

	// �s���P�ʍs��ɂ��邽�߂̐ݒ�
	m_SpriteInfo.m_Mat[0]._11 = 1.0f;
	m_SpriteInfo.m_Mat[0]._22 = 1.0f;
	m_SpriteInfo.m_Mat[0]._33 = 1.0f;
	m_SpriteInfo.m_Mat[0]._44 = 1.0f;
}

void SpriteRenderer::LateUpdate()
{
	//--- UI�̓I�u�W�F�N�g�̎�O�ɕ`�悷��
	// 0.25 �㉺0.5/��3 [720]
	// �ړ��s��
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(
		GetOwner()->GetComponent<Transform>()->GetPosition().x / 720 * (0.5f / std::sqrtf(3.0f)),
		GetOwner()->GetComponent<Transform>()->GetPosition().y / 720 * (0.5f / std::sqrtf(3.0f)),
		GetOwner()->GetLayerNum() * 1e-8f);
	// X��]�s��
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(
		0.0f/*GetOwner()->GetComponent<Transform>()->GetAngle().x*/);
	// Y��]�s��
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(
		0.0f/*GetOwner()->GetComponent<Transform>()->GetAngle().y*/);
	// Z��]�s��
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(
		GetOwner()->GetComponent<Transform>()->GetAngle().z);
	// �g��k���s��
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(
		GetOwner()->GetComponent<Transform>()->GetScale().x / 720 * (0.5f / std::sqrtf(3.0f)),
		GetOwner()->GetComponent<Transform>()->GetScale().y / 720 * (0.5f / std::sqrtf(3.0f)),
		1.0f/*GetOwner()->GetComponent<Transform>()->GetScale().z*/);

	// �S�Ă̍s�����ɂ܂Ƃ߂�
	DirectX::XMMATRIX mat = S * Rz * Ry * Rx * T;

	// �V�F�[�_�ɓn���O�Ɏ��s���鏈��
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

// �`��
void SpriteRenderer::Draw()
{
	m_pVS->Bind();
	m_pPS->Bind();
	m_pConst->BindVS(0);

	// �e�N�X�`���ݒ�
	SetTexturePS(m_SpriteInfo.pTexture, 0);

	// �`��
	m_SpriteInfo.pVtxBuf->Draw();
}

void SpriteRenderer::End()
{
	// �e�N�X�`�������
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