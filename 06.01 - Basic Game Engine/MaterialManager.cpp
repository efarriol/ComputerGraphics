#include "MaterialManager.h"
MaterialManager::MaterialManager() {
	createMaterialDefinitions();
}


MaterialManager::~MaterialManager() {
}

void MaterialManager::createMaterialDefinitions() {
	
	_materialData[0].ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
	_materialData[0].diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
	_materialData[0].specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
	_materialData[0].shininess = 0.4f;

	_materialData[1].ambient = glm::vec3(0.25f, 0.25f, 0.25f);
	_materialData[1].diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	_materialData[1].specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);
	_materialData[1].shininess = 0.6f;
	// Set up Light
	_materialData[2] = _materialData[1];
	_materialData[2].shininess = 0.0f; //????????????????????????????????????
	
}

material MaterialManager::getMaterialComponents(int materialID) {
	return _materialData[materialID];
}

int MaterialManager::getMaterialID(std::string materialName) {
	
	if (materialName == "Gold" || materialName == "GOLD" || materialName == "gold") {
		return GOLD;
	}
	else if (materialName == "Chrome" ||materialName == "CHROME" || materialName == "chrome") {
		return CHROME;
	}
	else if (materialName == "Light" || materialName == "LIGHT" || materialName == "light") {
		return LIGHT;
	}
	else {
		ErrorManagement::errorRunTime("Name " + materialName + " is not an existent material");
	}
	
}
