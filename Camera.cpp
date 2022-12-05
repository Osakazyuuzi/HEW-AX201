#include "Camera.h"
#include "ObjectBase.h"
#include "Transform.h"

// ビュー行列
DirectX::XMFLOAT4X4 Camera::GetViewMatrix() {
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;
	// ビュー行列を作成
	view = DirectX::XMMatrixLookAtLH(
		// 位置
		DirectX::XMVectorSet(
			GetOwner()->GetComponent<Transform>()->GetPosition().x,
			GetOwner()->GetComponent<Transform>()->GetPosition().y,
			GetOwner()->GetComponent<Transform>()->GetPosition().z, 0),
		// 注視点
		DirectX::XMVectorSet(m_LookPoint.x, m_LookPoint.y, m_LookPoint.z, 0),
		// アップベクトル
		DirectX::XMVectorSet(m_upVector.x, m_upVector.y, m_upVector.z, 0));
	// 転置する
	view = DirectX::XMMatrixTranspose(view);
	// XMFLOAT4x4に変換
	DirectX::XMStoreFloat4x4(&mat, view);
	// 転置済みのビュー行列を返す
	return mat;
}

// プロジェクション行列
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() {
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX proj;
	// プロジェクション行列を作成
	proj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_fovy), m_aspect, m_near, m_far);
	// 転置する
	proj = DirectX::XMMatrixTranspose(proj);
	// XMFLOAT4x4に変換
	DirectX::XMStoreFloat4x4(&mat, proj);
	// 転置済みのプロジェクション行列を返す
	return mat;
}