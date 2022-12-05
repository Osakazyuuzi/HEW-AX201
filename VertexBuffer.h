#ifndef ___VERTEXBUFFER_H___
#define ___VERTEXBUFFER_H___

#include "DirectX.h"

// 頂点情報の構造体
struct Vertex {
	float pos[3];	// ポリゴン座標
	float uv[2];	// テクスチャ座標
};

class VertexBuffer
{
public:
	// 頂点バッファの作成
	//  pVtx ： 頂点情報の先頭アドレス
	//  num  ： 頂点情報配列のデータ数
	HRESULT Create(Vertex* pVtx, int num);

	// ポリゴンの描画
	void Draw(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

private:
	ID3D11Buffer* m_pVtxBuf;	// 作成した頂点バッファ
	int m_vtxNum;	// 頂点の数
};

#endif