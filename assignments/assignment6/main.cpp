#include <iostream>
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Zeben/Shader.h>
#include <Zeben/texture.h>
#include <Zeben/Camera.h>
#include <Zeben/TorusMesh.h>
#include <Zeben/TorusGen.h>
#include "../core/ew/external/stb_image.h"
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void processInput(GLFWwindow *window);
void mouse_Callback(GLFWwindow *window, double xPosition_In, double yPosition_In);
void scroll_Callback(GLFWwindow *window, double xOffset, double yOffset);
void processInput(GLFWwindow *pWwindow);

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX =  float(SCREEN_WIDTH / 2), lastY = float(SCREEN_HEIGHT / 2);
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//lighting
glm::vec3 lightPosition(2.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
float ambientK = 0.05f;
float diffuseK = 0.0f;
float specularK = 0.0f;
float specularShininess = 2.0f;
bool cursorLocked = false;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}



    glfwSetCursorPosCallback(window, mouse_Callback);
    glfwSetScrollCallback(window, scroll_Callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

	//Initialization goes here!

	glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader backgroundShader("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");
    Shader lightCubeShader("assets/lightCubeShader.vert", "assets/lightCubeShader.frag");

    TorusGen ring(3.0f, 0.5f, 36, 18);
    //ring.printSelf();
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
    };

	//VBO means Vertex Buffer Object
	//Vertex Data
	//
	unsigned int cubeVAO, VBO;
	glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Put Data into currently bound VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Modern Verison of glBuffer Binding
	//glNamedBufferData(VBO, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	//Position(XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    //Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //load texture
    texture backgroundTexture;
    unsigned int backgroundTextureName;
    backgroundTexture.load2DTexture(backgroundTextureName, "assets/Oliver texture Resized.jpg");

    backgroundShader.use();
    backgroundShader.setInt("texture1", 0);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorLocked = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
            cursorLocked = false;
        }

        //Draw ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        //Create Window
        ImGui::Begin("Light Settings");
        ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
        ImGui::ColorEdit3("Light Color", &lightColor.x, 0.1f);
        ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular Shininess", &specularShininess, 2.0f, 1024.0f);
        ImGui::Text("Mouse Control Toggle is Right Click");
        ImGui::End();

        //Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTextureName);
        backgroundShader.use();
        backgroundShader.setVec3("lightColor", lightColor);
        backgroundShader.setVec3("lightPosition", lightPosition);
        backgroundShader.setVec3("viewPosition", camera.Position);
        backgroundShader.setFloat("ambientK", ambientK);
        backgroundShader.setFloat("diffuseK", diffuseK);
        backgroundShader.setFloat("specularK", specularK);
        backgroundShader.setFloat("specularHighlight", specularShininess);


        //view projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        backgroundShader.setMat4("projection", projection);
        backgroundShader.setMat4("view", view);

        //world transformations
        glm::mat4 model = glm::mat4(1.0f);
        backgroundShader.setMat4("modelMatrix", model);

        //glBindVertexArray(cubeVAO);
        //Draw Call
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        ring.draw();
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("modelMatrix", model);
        lightCubeShader.setVec3("lightColor", lightColor);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


		//Drawing happens here!
		glfwSwapBuffers(window);
        glfwPollEvents();

	}

	printf("Shutting down...");
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)

        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(SPRINT, deltaTime);
        }
        else
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }

}

void mouse_Callback(GLFWwindow *window, double xPosition_In, double yPosition_In)
{
    if (!cursorLocked) { return; }

    auto xPosition = static_cast<float>(xPosition_In);
    auto yPosition = static_cast<float>(yPosition_In);
    if (firstMouse)
    {
        lastX = xPosition;
        lastY = yPosition;
        firstMouse = false;
    }
    float xOffset = xPosition - lastX;
    float yOffset = lastY - yPosition;

    lastX = xPosition;
    lastY = yPosition;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_Callback(GLFWwindow *window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
};