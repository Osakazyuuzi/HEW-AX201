#ifndef ___CUBE_H___
#define ___CUBE_H___
#include "Object3D.h"
#include "MeshRenderer.h"

class Cube : public Object3D
{
public:
	// コンストラクタ
	Cube(std::string name, std::string tag) : Object3D(name, tag) {
		// キューブのモデルをロードする
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Cube.fbx", 1.0f);
	}
	// デストラクタ
	~Cube(){}
};

#endif //!___CUBE_H___