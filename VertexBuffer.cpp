#include "DirectX.h"
#include "VertexBuffer.h"

HRESULT VertexBuffer::Create(Vertex* pVtx, int num)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();

	// 頂点の数 設定
	m_vtxNum = num;

	// バッファ情報 設定
	D3D11_BUFFER_DESC vtxBufDesc;
	ZeroMemory(&vtxBufDesc, sizeof(vtxBufDesc));
	vtxBufDesc.ByteWidth = sizeof(Vertex) * m_vtxNum;	// バッファの大きさ
	vtxBufDesc.Usage = D3D11_USAGE_DEFAULT;			// メモリ上での管理方法
	vtxBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// GPU上での利用方法

	// バッファ初期データ 設定
	D3D11_SUBRESOURCE_DATA vtxSubResource;
	ZeroMemory(&vtxSubResource, sizeof(vtxSubResource));
	vtxSubResource.pSysMem = pVtx;						// バッファに入れ込むデータ

	// 作成
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
	pContext->IASetVertexBuffers(0, 1, &m_pVtxBuf, &stride, &offset);	// 頂点バッファをGPUに渡す
	pContext->Draw(m_vtxNum, 0);		// 描画
}