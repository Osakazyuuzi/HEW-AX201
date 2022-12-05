#ifndef ___VERTEXBUFFER_H___
#define ___VERTEXBUFFER_H___

#include "DirectX.h"

// ���_���̍\����
struct Vertex {
	float pos[3];	// �|���S�����W
	float uv[2];	// �e�N�X�`�����W
};

class VertexBuffer
{
public:
	// ���_�o�b�t�@�̍쐬
	//  pVtx �F ���_���̐擪�A�h���X
	//  num  �F ���_���z��̃f�[�^��
	HRESULT Create(Vertex* pVtx, int num);

	// �|���S���̕`��
	void Draw(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

private:
	ID3D11Buffer* m_pVtxBuf;	// �쐬�������_�o�b�t�@
	int m_vtxNum;	// ���_�̐�
};

#endif