#pragma once
#include <glm/glm.hpp>
#include <string>
#include "ErrorManagement.h"
#define GOLD 0
#define CHROME 1
#define LIGHT 2
#define NUMMATERIALS 3

struct material {
	glm::vec3 ambient; glm::vec3 diffuse; glm::vec3 specular; float shininess;
	int type;
};

class MaterialManager {
	material _materialData[NUMMATERIALS];
	std::string _materialType[NUMMATERIALS];
	void createMaterialDefinitions();
public:
	MaterialManager();
	~MaterialManager();
	material getMaterialComponents(int materialID);
	int getMaterialID(std::string materialName);
};

