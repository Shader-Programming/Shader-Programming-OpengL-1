#pragma once

#include <glm/glm.hpp>

class LightParams
{
public:
	static int selectedLight;
	static glm::vec3 lightPos;
	static glm::vec3 dirLightCol;
	static glm::vec3 pointLightCol;
	static glm::vec3 dirLightDir;
	static float pLightIntensity;
	static float innerAngleRadians;
	static bool useNormalMap;
	static bool useDispMap;
	static bool toggleBlur;
	static bool toggleBloom;
	static bool toggleDepthOfField;
	static bool toggleShadowMapping;
};
