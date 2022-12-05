#ifndef ___SPRITERENDERER_H___
#define ___SPRITERENDERER_H___

#include "ComponentBase.h"
#include "DirectXTex/Texture.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "BlendState.h"
#include <memory>
#include <list>
#include <DirectXMath.h>

class SpriteRenderer : public ComponentBase
{
public:

	struct Info {
		// ���_���
		VertexBuffer* pVtxBuf;
		// UV���
		float uvWidth;		// ����
		float uvHeight;		// �c��
		float uvTopLeftU;	// ����U���W
		float uvTopLeftV;	// ����V���W
		// �e�N�X�`��
		ID3D11ShaderResourceView* pTexture;
		// �s��
		DirectX::XMFLOAT4X4 m_Mat[3];
	};

public:
	// �R���X�g���N�^
	SpriteRenderer();
	// �f�X�g���N�^
	~SpriteRenderer();

	void Start();
	void LateUpdate();
	// �`��
	void Draw();
	void End();

	// �e�N�X�`���ǂݍ���
	ID3D11ShaderResourceView* LoadTexture(std::string);

	//--- �Z�b�g�֐�
	void SetInfo(Info info) { m_SpriteInfo = info; }
	void SetSize(float width, float height);

	//--- �Q�b�g�֐�
	Info* GetInfo() { return &m_SpriteInfo; }

private:
	static VertexShader* m_pDefVS;
	static PixelShader* m_pDefPS;
	static unsigned int m_shaderRef;
	//static BlendState* m_pAlphaBlend;
	// �e�N�X�`�����X�g
	static std::list<std::pair<std::string, ID3D11ShaderResourceView*>> m_TextureList;
private:
	VertexShader* m_pVS;
	PixelShader* m_pPS;
	ConstantBuffer* m_pConst;
	Info m_SpriteInfo;
};

#endif //!___SPRITERENDERER_H___