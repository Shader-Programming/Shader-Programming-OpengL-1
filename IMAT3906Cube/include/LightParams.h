#pragma once

#include <glm/glm.hpp>

class LightParams
{
public:
	static int selectedLight;
	static glm::vec3 lightPos;
	static glm::vec3 pointLightPos;
	static glm::vec3 spotLightPos;
	static glm::vec3 dirLightCol;
	static glm::vec3 spotLightCol;
	static glm::vec3 pointLightCol;
	static glm::vec3 dirLightDir;
	static float pLightIntensity;
	static float spotLightInnerRad;
	static float spotLightOuterRad;
	static bool useNormalMap;
	static bool useDispMap;
	static bool toggleBlur;
	static bool toggleBloom;
	static bool toggleDepthOfField;
	static bool toggleShadowMapping;
};
