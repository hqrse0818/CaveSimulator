#include "OBJ_RockA.h"

OBJ_RockA::OBJ_RockA()
{
	p_mModelCom = new Com_StaticMesh();
	p_mModelCom->SetModelData("RockA");
	p_mModelCom->SetVertex("VS_MODEL");
	p_mModelCom->SetPixel("PS_MODEL");
	AddComponent(p_mModelCom);
}

OBJ_RockA::OBJ_RockA(const char* _name)
	: OBJ_RockA()
{
	sObjectName = _name;
}
