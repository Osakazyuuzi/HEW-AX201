#ifndef ___OBJECT_H___
#define ___OBJECT_H___

#include <iostream>
#include <typeinfo>
#include <memory>
#include <list>
#include <string>
#include "ComponentBase.h" 
#include "TagName.h"

class ObjectManager;

class ObjectBase
{
	friend class ObjectManager;
	using ComponentPool = std::list<std::shared_ptr<ComponentBase>>;
public:
	// コンストラクタ
	ObjectBase();
	// 引数付きコンストラクタ
	ObjectBase(std::string name, std::string tag)
		: ObjectBase()
	{
		m_name = name;
		m_tag = tag;
	}
	// デストラクタ
	virtual ~ObjectBase(){}

	// オブジェクトを削除する
	void Destroy();

	void Start() {
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
			(*it)->Start();
	}

	// コンポーネントの前更新処理
	void FixedUpdate()
	{
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
			(*it)->FixedUpdate();
	}

	// コンポーネントの更新処理
	void Update();

	// コンポーネントの後更新処理
	void LateUpdate() {
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
			(*it)->LateUpdate();
	}

	// 描画
	virtual void Draw() {};

	// コンポーネントを追加
	template <class T>
	std::shared_ptr<T> AddComponent() {
		// コンポーネントを作成（メモリ確保）
		std::shared_ptr<T> pComp = std::make_shared<T>();
		// 自分のポインタをセット
		pComp->SetOwner(this);
		// コンポーネントプールに追加
		m_ComponentPool.push_back(pComp);
		// スマートポインタを返す
		return pComp;
	}

	// コンポーネントを取得
	template <class T>
	std::shared_ptr<T> GetComponent() {
		// プールを反復する
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
			// 同じ型のコンポーネントか判定
			if (typeid(T) == typeid(*(*it))) 
				// 見つかったらスマートポインタを返す
				return std::static_pointer_cast<T>(*it);
		// 見つからなかったらnullptrを返す
		return nullptr;
	}

	// コンポーネントを持っているか
	template <class T>
	bool HasComponent() {
		// プールを反復する
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
			// 同じ型のコンポーネントデータか判定
			if (typeid(T) == typeid(*(*it)))
				// 見つかったらtrueを返す
				return true;
		// 見つからなかったらfalseを返す
		return false;
	}

	// コンポーネントを破棄
	template <class T>
	bool RemoveComponent() {
		// プールを反復する
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
			// 同じ型のコンポーネントデータか判定
			if (typeid(T) == typeid(*(*it))) {
				// 見つかったらコンテナから削除する
				m_ComponentPool.erase(it);
				// 削除できた判定を返す
				return true;
			}
		// 削除できなかった判定を返す
		return false;
	}

	//--- 当たり判定
	void CallCollisionEnter(ObjectBase* object) {
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); ++it)
			(*it)->OnCollisionEnter(object);
	}
	void CallCollisionStay(ObjectBase* object) {
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); ++it)
			(*it)->OnCollisionStay(object);
	}
	void CallCollisionExit(ObjectBase* object) {
		for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); ++it)
			(*it)->OnCollisionExit(object);
	}

	//--- ゲット関数
	std::string GetName() { return m_name; }
	std::string GetTag() { return m_tag; }
	bool GetDestroy() { return m_isDestroy; }
	std::shared_ptr<ObjectBase> GetParent() { return m_Parent; }
	int GetLayerNum() { return m_LayerNum; }
	std::shared_ptr<ObjectBase> GetThisPtr() { return m_this; }

	//--- セット関数
	void SetDestroy(bool destroy) { m_isDestroy = destroy; }
	void SetParent(std::shared_ptr<ObjectBase> parent) { m_Parent = parent; }
	void SetLayerNum(int layerNum) { m_LayerNum = layerNum; }

protected:
	// ネーム
	std::string m_name;
	// タグ
	std::string m_tag;
	// 削除フラグ
	bool m_isDestroy;

	// コンポーネント格納プール
	ComponentPool m_ComponentPool;
	ComponentPool m_DestroyComponentPool;

	// 自身のポインタ
	std::shared_ptr<ObjectBase> m_this;

	// 親オブジェクトのポインタ
	std::shared_ptr<ObjectBase> m_Parent;

	// レイヤー番号
	int m_LayerNum;
};

//--- レイヤー番号による比較
//bool operator<(const std::shared_ptr<ObjectBase>& left, const std::shared_ptr<ObjectBase>& right);
//bool operator>(const std::shared_ptr<ObjectBase>& left, const std::shared_ptr<ObjectBase>& right);

#endif //!___OBJECT_H___