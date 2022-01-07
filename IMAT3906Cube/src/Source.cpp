#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "renderer.h"
#include "LightParams.h"

#include<string>
#include <iostream>
#include <numeric>




// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);



// camera
Camera camera(glm::vec3(0,0,9));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;






// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;



//Parameters
bool toggleInterract = 0;



int main()
{
	

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	//Renderer
	Renderer renderer(SCR_WIDTH, SCR_HEIGHT);

	renderer.shaders[2].use();
	renderer.setFBOColour();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	

	static const char* items[]{ "Directional Light","Point Light","Spot Light" };



	renderer.assignCamera(camera);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//First pass to fill shadow map
		glBindFramebuffer(GL_FRAMEBUFFER, renderer.FBODepthMap);
		glViewport(0,0, 4096, 4096);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.shaders[7].use();
		renderer.shaders[7].setMat4("lightSpaceMatrix", renderer.lightSpaceMatrix);
		renderer.RenderShadowMap();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.RenderScene(camera);



		//// 1st pass to FBO
		//glBindFramebuffer(GL_FRAMEBUFFER, renderer.FBO);


		////UI stuff
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();


		//glEnable(GL_DEPTH_TEST);
		//
		//renderer.RenderScene(camera);
		//renderer.shaders[0].use();
		//renderer.shaders[0].setBool("toggleNormalMap", LightParams::useNormalMap);
		//renderer.setUniforms(renderer.shaders[0],camera);
		//renderer.setUniforms(renderer.shaders[1],camera);

		//renderer.shaders[1].use();
		//renderer.shaders[1].setBool("toggleDispMap", LightParams::useDispMap);
		//renderer.shaders[1].setBool("toggleNormalMap", LightParams::useNormalMap);

		//renderer.shaders[2].use();
		//renderer.shaders[2].setInt("image", renderer.colourAttachment[0]);

		//renderer.updateSpotLightUniforms();
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glActiveTexture(GL_TEXTURE9);
		//glBindTexture(GL_TEXTURE_2D, 9);

		//// Blur
		//glBindFramebuffer(GL_FRAMEBUFFER, renderer.FBOBlur);




		//renderer.shaders[4].use();
		//renderer.shaders[4].setBool("blurToggle", LightParams::toggleBlur);
		//renderer.shaders[4].setBool("depthOfFieldToggle", LightParams::toggleDepthOfField);
		//renderer.shaders[4].setInt("image", 7);
		//renderer.shaders[4].setInt("image2", 8);
		//renderer.shaders[4].setInt("depthMap", 9);
		//renderer.shaders[4].setBool("horizontal",true);
		//renderer.quad.Draw(renderer.shaders[4],renderer.colourAttachment[0], renderer.colourAttachment[1]);
		//renderer.shaders[4].setBool("horizontal", false);
		//renderer.quad.Draw(renderer.shaders[4], renderer.colourAttachment[0], renderer.colourAttachment[1]);









		//renderer.shaders[6].use();
		//renderer.shaders[6].setInt("image", 7);
		//renderer.shaders[6].setInt("bloomBlur", 8);


		////renderer.quad.Draw(renderer.shaders[5], renderer.colourAttachment[0], renderer.blurredTextures[0]);
		//	

		////3rd pass render to screen - quad vao
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//
		//renderer.shaders[2].use();
		//renderer.shaders[2].setInt("image", 7);

		//if (LightParams::toggleBloom)
		//{
		//	LightParams::toggleBlur = true;
		//	renderer.quad.Draw(renderer.shaders[6], renderer.colourAttachment[0], renderer.blurredTextures[1]);
		//}
		//else 
		//	renderer.quad.Draw(renderer.shaders[2], renderer.blurredTextures[0]);







		//ImGui::SetNextWindowPos(ImVec2(0, 0));
		//ImGui::Begin("Options Window");
		//ImGui::Text("You can change parameters in real time using this panel");
		//ImGui::Text("Press left alt to interract");




		//ImGui::BeginTabBar("#tabs");
		//if (ImGui::BeginTabItem("Light Casters"))
		//{
		//	ImGui::ListBox("", &LightParams::selectedLight, items, IM_ARRAYSIZE(items));
		//	switch (LightParams::selectedLight)
		//	{
		//	case 0:
		//		ImGui::ColorEdit3("Color", (float*)&LightParams::dirLightCol);
		//		ImGui::InputFloat3("Light Direction", (float*)&LightParams::dirLightDir);
		//		break;
		//	case 1:
		//		ImGui::ColorEdit3("Color", (float*)& LightParams::pointLightCol);
		//		ImGui::SliderFloat("Intensity", &LightParams::pLightIntensity, 0.1f, 15.f);
		//		break;
		//	case 3:
		//		ImGui::SliderAngle("Angle", &LightParams::innerAngleRadians);
		//		break;
		//	}
		//	ImGui::EndTabItem();
		//}
		//if (ImGui::BeginTabItem("Light Maps"))
		//{
		//	ImGui::Checkbox("Use normal map", &LightParams::useNormalMap);
		//	ImGui::Checkbox("Use displaycement map", &LightParams::useDispMap);
		//	ImGui::EndTabItem();
		//}
		//if (ImGui::BeginTabItem("Post Processing"))
		//{
		//	ImGui::Checkbox("Enable Blur", &LightParams::toggleBlur);
		//	ImGui::Checkbox("Enable Bloom", &LightParams::toggleBloom);
		//	ImGui::Checkbox("Enable Depth of Field", &LightParams::toggleDepthOfField);

		//	ImGui::EndTabItem();
		//}
		//ImGui::EndTabBar();


		//

		//ImGui::End();

		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the key is pressed only this callback function executes
// ---------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
	{
		if (toggleInterract == 1)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			toggleInterract = 0;
		}
			
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			toggleInterract = 1;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!toggleInterract)
	{

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		//Update mouse coordinates to not offset camera pos when returning input
		lastX = xpos;
		lastY = ypos;
	}
	
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




