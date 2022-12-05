#ifndef ___OBJECTMANAGER_H___
#define ___OBJECTMANAGER_H___

#include <list>
#include <memory>
#include <string>
#include <algorithm>
#include "ObjectBase.h"
#include <vector>

class ObjectBase;

class ObjectManager
{
	friend class ObjectBase;
	using ObjectPool = std::vector<std::shared_ptr<ObjectBase>>;

private:
	// �I�u�W�F�N�g�v�[��
	static ObjectPool m_AppendObjectPool;
	static ObjectPool m_ObjectPool;
	static ObjectPool m_DestroyObjectPool;
public:
	// �R���X�g���N�^
	ObjectManager() {}
	// �f�X�g���N�^
	~ObjectManager() {
		m_AppendObjectPool.clear();
		m_ObjectPool.clear();
		m_DestroyObjectPool.clear();
	}

	// �I�u�W�F�N�g���쐬
	template <class T>
	static std::shared_ptr<ObjectBase> CreateObject(std::string name, std::string tag = TagName::Untagged) {
		std::shared_ptr<ObjectBase> pObject = std::make_shared<T>(name, tag);
		// �������g���w���|�C���^���Z�b�g����
		pObject->m_this = pObject;
		// �I�u�W�F�N�g�v�[���ɃI�u�W�F�N�g��ǉ�
		AddObject(pObject);
		return pObject;
	}

	// �I�u�W�F�N�g��o�^
	static void AddObject(std::shared_ptr<ObjectBase> pObject) {
		// �I�u�W�F�N�g�v�[���ɃI�u�W�F�N�g��ǉ�
		m_AppendObjectPool.push_back(pObject);
	}

	// �I�u�W�F�N�g���폜
	static void RemoveObject(std::shared_ptr<ObjectBase> pObject) {
		// �폜�v�[���ɃI�u�W�F�N�g��ǉ�
		m_DestroyObjectPool.push_back(pObject);
		// �I�u�W�F�N�g�̍폜�����ON��
		pObject->SetDestroy(true);
	}

	// �I�u�W�F�N�g��S�č폜
	static void DestroyAllObject() {
		// �I�u�W�F�N�g�v�[����S�č폜
		m_AppendObjectPool.clear();
		m_ObjectPool.clear();
	}

	// �I�u�W�F�N�g�̎擾
	static std::shared_ptr<ObjectBase> FindObjectWithName(const std::string& name) {
		for (auto it = m_AppendObjectPool.begin(); it != m_AppendObjectPool.end(); it++) {
			// ���O�����������肷��
			if ((*it)->GetName() == name)
				return (*it);
		}
		for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++) {
			// ���O�����������肷��
			if ((*it)->GetName() == name)
				return (*it);
		}
		// ������Ȃ�������nullptr��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g�̎擾
	static std::shared_ptr<ObjectBase> FindObjectWithTag(const std::string& tag) {
		for (auto it = m_AppendObjectPool.begin(); it != m_AppendObjectPool.end(); it++) {
			// �^�O�������������肷��
			if ((*it)->GetTag() == tag)
				return (*it);
		}
		for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++) {
			// �^�O�������������肷��
			if ((*it)->GetTag() == tag)
				return (*it);
		}
		// ������Ȃ�������nullptr��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g�̎擾
	static std::list<std::shared_ptr<ObjectBase>> FindObjectListWithTag(const std::string& tag) {
		std::list< std::shared_ptr<ObjectBase>> pObjList;
		for (auto it = m_AppendObjectPool.begin(); it != m_AppendObjectPool.end(); it++) {
			// �^�O�������������肷��
			if ((*it)->GetTag() == tag)
				// ���X�g�ɒǉ�
				pObjList.push_back((*it));
		}
		for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++) {
			// �^�O�������������肷��
			if ((*it)->GetTag() == tag)
				// ���X�g�ɒǉ�
				pObjList.push_back((*it));
		}
		// �I�u�W�F�N�g���X�g��Ԃ��B
		return pObjList;
	}

	// �I�u�W�F�N�g�̑O�X�V
	static void FixedUpdate() {
		for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++)
			if (!(*it)->GetDestroy())
				(*it)->FixedUpdate();
	}
	// �I�u�W�F�N�g�̍X�V
	static void Update();
	// �I�u�W�F�N�g�̌�X�V
	static void LateUpdate() {
		for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++)
			if (!(*it)->GetDestroy())
				(*it)->LateUpdate();
		Delete();
	}
	// �I�u�W�F�N�g�̕`��
	static void Draw() {
		// �`�揇���C���[�ɂ���ă\�[�g����
		//std::sort(m_ObjectPool.begin(), m_ObjectPool.end());
		for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++)
			if (!(*it)->GetDestroy())
				(*it)->Draw();
	}

private:
	// �폜�I�u�W�F�N�g���폜
	static void Delete() {
		// ���X�g����Ȃ甲����
		if (m_DestroyObjectPool.empty()) return;
		// ���X�g�̒��g���폜
		m_DestroyObjectPool.clear();
	}
};

#endif //!___OBJECTMANAGER_H___