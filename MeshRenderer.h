#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include <memory>
#include "ConstantBuffer.h"
#include "ComponentBase.h"
#include "DirectXTex/Texture.h"
#include <list>

class MeshRenderer : public ComponentBase
{
public:
	struct Vertex;
	struct Material;
	struct Mesh;

	struct Info {
		Mesh* m_pMeshes;
		unsigned int m_meshNum;
		Material* m_pMaterials;
		unsigned int m_materialNum;
	};

public:
	MeshRenderer();
	~MeshRenderer();

	bool LoadModel(const char* file, float scale);

	void Start();
	void LateUpdate();
	void Draw();
	void End();

	void SetVertexShader(VertexShader* vs);
	void SetPixelShader(PixelShader* ps);

	const Mesh* GetMesh(unsigned int index);
	unsigned int GetMeshNum();


private:
	static VertexShader* m_pDefVS;
	static PixelShader* m_pDefPS;
	static unsigned int m_shaderRef;
	// ƒ‚ƒfƒ‹ƒŠƒXƒg
	static std::list<std::pair<std::string, Info*>> m_ModelList;
private:
	Info m_MeshInfo;

	VertexShader* m_pVS;
	PixelShader* m_pPS; 
	std::shared_ptr<ConstantBuffer> m_pWVP;
	DirectX::XMFLOAT4X4 m_Mat[3];
};


#endif // __SPRITE_H__