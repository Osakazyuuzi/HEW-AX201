#include "DirectX.h"
#include "VertexBuffer.h"

HRESULT VertexBuffer::Create(Vertex* pVtx, int num)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();

	// ���_�̐� �ݒ�
	m_vtxNum = num;

	// �o�b�t�@��� �ݒ�
	D3D11_BUFFER_DESC vtxBufDesc;
	ZeroMemory(&vtxBufDesc, sizeof(vtxBufDesc));
	vtxBufDesc.ByteWidth = sizeof(Vertex) * m_vtxNum;	// �o�b�t�@�̑傫��
	vtxBufDesc.Usage = D3D11_USAGE_DEFAULT;			// ��������ł̊Ǘ����@
	vtxBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// GPU��ł̗��p���@

	// �o�b�t�@�����f�[�^ �ݒ�
	D3D11_SUBRESOURCE_DATA vtxSubResource;
	ZeroMemory(&vtxSubResource, sizeof(vtxSubResource));
	vtxSubResource.pSysMem = pVtx;						// �o�b�t�@�ɓ��ꍞ�ރf�[�^

	// �쐬
	hr = pDevice->CreateBuffer(&vtxBufDesc, &vtxSubResource, &m_pVtxBuf);
	if (FAILED(hr)) { return hr; }

	return hr;
}

void VertexBuffer::Draw(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	ID3D11DeviceContext* pContext = GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pContext->IASetPrimitiveTopology(topology);
	pContext->IASetVertexBuffers(0, 1, &m_pVtxBuf, &stride, &offset);	// ���_�o�b�t�@��GPU�ɓn��
	pContext->Draw(m_vtxNum, 0);		// �`��
}