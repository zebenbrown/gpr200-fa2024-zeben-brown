#include <iostream>
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Zeben/Shader.h"

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 720;






int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!

    Shader triangleShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	float triangleVertices[] = {
	//X		          Y	        Z		  R		    G	      B		    A
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	//VBO means Vertex Buffer Object
	//Vertex Data
	//
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Put Data into currently bound VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	//Modern Verison of glBuffer Binding
	//glNamedBufferData(VBO, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	//Position(XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	





	//Render loop
	while (!glfwWindowShouldClose(window)) {


		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime();
        triangleShader.use();
        triangleShader.setFloat("time", currentTime);
        triangleShader.setVec3("offsetPosition", 0.5, 0.5, 0.1);
		glBindVertexArray(VAO);

		//Draw Call
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Drawing happens here!
		glfwSwapBuffers(window);
        glfwPollEvents();
	}
	printf("Shutting down...");
}
