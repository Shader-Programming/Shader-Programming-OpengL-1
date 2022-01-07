
#include "LightParams.h"

int LightParams::selectedLight = 0;
glm::vec3 LightParams::dirLightCol = glm::vec3(1, 1, 1);
glm::vec3 LightParams::pointLightCol = glm::vec3(1, 0, 1);
glm::vec3 LightParams::dirLightDir = glm::vec3(1, -1, 0);
glm::vec3 LightParams::lightPos = LightParams::dirLightDir * glm::vec3(-0.7);
float LightParams::pLightIntensity = 2;
float LightParams::innerAngleRadians = 12.5f;
bool LightParams::useNormalMap = 1;
bool LightParams::useDispMap = 0;
bool LightParams::toggleBlur = 0;
bool LightParams::toggleBloom = 0;
bool LightParams::toggleDepthOfField = 0;
