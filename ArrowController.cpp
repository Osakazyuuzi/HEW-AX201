#include "ArrowController.h"
#include "ObjectBase.h"
#include "ObjectManager.h"

void ArrowController::Update()
{

}

void ArrowController::OnCollisionEnter(ObjectBase* object)
{
	if (object->GetTag() != TagName::Player) {
		ObjectManager::RemoveObject(GetOwner()->GetThisPtr());
	}
}

void ArrowController::OnCollisionStay(ObjectBase* object)
{
}

void ArrowController::OnCollisionExit(ObjectBase* object)
{

}