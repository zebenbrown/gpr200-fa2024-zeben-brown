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
#include "../core/ew/external/stb_image.h"
#include <filesystem>

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialization goes here!

	glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader backgroundShader("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[]
    {
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
        glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-15.0f, 15.0f)),
    };

	//VBO means Vertex Buffer Object
	//Vertex Data
	//
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Put Data into currently bound VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Modern Verison of glBuffer Binding
	//glNamedBufferData(VBO, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	//Position(XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Texture Coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    //load texture
    texture backgroundTexture;
    unsigned int backgroundTextureName;
    backgroundTexture.load2DTexture(backgroundTextureName, "assets/Oliver texture Resized.jpg");


    backgroundShader.use();
    backgroundShader.setInt("backgroundShader", 0);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTextureName);
        backgroundShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
        backgroundShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        backgroundShader.setMat4("view", view);


        /*float randomScale = ew::RandomRange(0.5f, 5.0f);
        if (randomScale < 0.1) { randomScale = 1.0f;}*/

        glBindVertexArray(VAO);

          for(unsigned int i = 0; i < 20; i++)
          {
              //float randomRotation = ew::RandomRange(45.0f, 90.0f);
              glm::mat4 model = glm::mat4(1.0f);
              //tried to add the random scale but it was trying to render it in two different places and I cant think of the fix right now
              //model = glm::scale(model,glm::vec3(randomScale, randomScale, 1));
              model = glm::translate(model, cubePositions[i]);
              //Couldn't get delta time to work properly with rotations
              float angle = currentTime * 60;//randomRotation * deltaTime;
              model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

              backgroundShader.setMat4("modelMatrix", model);
              //Draw Call
              glDrawArrays(GL_TRIANGLES, 0, 36);
          }

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
        camera.ProcessKeyboard(UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

}

void mouse_Callback(GLFWwindow *window, double xPosition_In, double yPosition_In)
{
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