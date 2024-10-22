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
#include "../core/ew/external/stb_image.h"
#include <filesystem>

void processInput(GLFWwindow *window);
void mouse_Callback(GLFWwindow *window, double xPosition, double yPosition);
void scroll_Callback(GLFWwindow *window, double xOffset, double yOffset);

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 720;
float currentTime = glfwGetTime();

glm::vec3 cameraPosition   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX =  float(SCREEN_WIDTH / 2), lastY = float(SCREEN_HEIGHT / 2);
bool firstMouse = true;
float fieldOfView = 45.0f;

void processInput(GLFWwindow *pWwindow);

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tranformations", NULL, NULL);
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
    Shader spriteShader("assets/spriteVertexShader.vert", "assets/spriteFragmentShader.frag");

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

    glm::vec3 cubePositions[] {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
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
    //glUniform1i(glGetUniformLocation(backgroundShader.ID, "backgroundShader"), 0);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

        float currentTime = glfwGetTime();


        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTextureName);
        backgroundShader.use();

        glm::mat4 modelMatrix = glm::mat4(1);
        glm::mat4 view = glm::mat4(1);
        glm::mat4 projection = glm::mat4(1);

        const float RADIUS = 10.0f;
        float cameraX = sin(currentTime) * RADIUS;
        float cameraZ = cos(currentTime) * RADIUS;

        // create transformations
        /*glm::mat4 backgroundTransform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        backgroundTransform = glm::translate(backgroundTransform, glm::vec3(0.25f, -0.25f, 0.0f));
        backgroundTransform = glm::scale(backgroundTransform, glm::vec3(0.5f, 0.5f, 0.0f));
        backgroundTransform = glm::rotate(backgroundTransform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));*/

        /*unsigned int backgroundTransformLocation = glGetUniformLocation(backgroundShader.ID, "transform");
        glUniformMatrix4fv(backgroundTransformLocation, 1, GL_FALSE, glm::value_ptr(backgroundTransform));*/



        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);




          modelMatrix = glm::rotate(modelMatrix, currentTime, glm::vec3(0.5f, 1.0f, 0.0f));
          //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0));
          projection = glm::perspective(glm::radians(fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

          //unsigned int modelMatrixLocation = glGetUniformLocation(backgroundShader.ID, "modelMatrix");
          unsigned int viewLocation = glGetUniformLocation(backgroundShader.ID, "view");

          //glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
          //glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

          backgroundShader.setMat4("projection", projection);
          backgroundShader.setMat4("view", view);
          glBindVertexArray(VAO);

          for(unsigned int i = 0; i < 10; i++)
          {
              glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, cubePositions[i]);
              float angle = deltaTime * i;
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

    float cameraSpeed = 3.0f * deltaTime;

    glm::vec3 mouseDirection;
    mouseDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    mouseDirection.y = sin(glm::radians(pitch));
    mouseDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(mouseDirection);




    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void mouse_Callback(GLFWwindow *window, double xPosition, double yPosition)
{
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

    const float SENSITIVITY = 0.1f;
    xOffset *= SENSITIVITY;
    yOffset *= SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) {pitch = 89.0f;}
    if (pitch < -89.0f) {pitch = -89.0f;}


}

void scroll_Callback(GLFWwindow *window, double xOffset, double yOffset)
{
    fieldOfView -= (float)yOffset;
    if (fieldOfView < 1.0f) { fieldOfView = 1.0f;}
    if (fieldOfView > 45.0f) { fieldOfView = 45.0f;}
};