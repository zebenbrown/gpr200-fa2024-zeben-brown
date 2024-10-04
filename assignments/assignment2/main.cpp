#include <iostream>
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Zeben/Shader.h>
#include <Zeben/texture.h>
#include "../core/ew/external/stb_image.h"
#include <filesystem>


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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader backgroundShader("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");
    Shader spriteShader("assets/spriteVertexShader.vert", "assets/spriteFragmentShader.frag");

    float vertices[] = {
            // positions                          // colors                     // texture coords
            1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    unsigned int indices[] =
            {
            0,1,3 ,//First triangle
            1,2,3//second triangle
            };

	//VBO means Vertex Buffer Object
	//Vertex Data
	//
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Put Data into currently bound VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Modern Verison of glBuffer Binding
	//glNamedBufferData(VBO, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Position(XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    //Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //load texture
    texture backgroundTexture, spriteTexture;
    unsigned int backgroundTextureName, spriteTextureName;
    backgroundTexture.load2DTexture(backgroundTextureName, "assets/DuckBackground3.png");
    spriteTexture.load2DTexture(spriteTextureName, "assets/DuckSprite.png");

	//Render loop
	while (!glfwWindowShouldClose(window)) {


		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = glfwGetTime();

        backgroundShader.use();
        backgroundShader.setFloat("time", currentTime);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTextureName);
        glUniform1i(glGetUniformLocation(backgroundShader.ID, "backgroundShader"), 0);
        glBindVertexArray(VAO);

        //Draw Call
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        spriteShader.use();
        spriteShader.setFloat("scale", 0.8);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, spriteTextureName);

        glUniform1i(glGetUniformLocation(spriteShader.ID, "spriteShader"), 1);
		glBindVertexArray(VAO);
        //Draw Call
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Drawing happens here!
		glfwSwapBuffers(window);
        glfwPollEvents();
	}
	printf("Shutting down...");
}
