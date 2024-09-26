#include <iostream>
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Zeben/Shader.h>
#include <ew/external/stb_image.h>

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

    Shader myShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	float Vertices[] = {
              //first triangle
              //positions                   //colors                                        //texture coordinates
            //   X        Y        Z            R        G        B                         S        T
             0.5f,0.5f,0.0f,    1.0f,0.0f,0.0f,                 1.0f,1.0f, //top right
             0.5f,-0.5f,0.0f,   0.0f,1.0f,0.0f,              1.0f,0.0f,// bottom right
             -0.5f,0.5f, 0.0f,  0.0f,0.0f,1.0f,            0.0f,1.0f, // top left
             -0.5f, -0.5f, 0.0f,    1.0f,1.0f,0.0f,       0.0f,0.0f// bottom left
	};

    unsigned int indices[] =
            {
                0, 1, 2, //first triangle
                1, 2, 3 //second triangle
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    //Modern Verison of glBuffer Binding
    //glNamedBufferData(VBO, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //adding a texture
	unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int height, width, nrChannels;

    unsigned char *picture = stbi_load("assets/DuckBackground.png", &width, &height, &nrChannels, 0);
    if (picture)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, picture);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "\nImage height: " << height << " Image Width: " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(picture);

    /*glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/

    /*picture = stbi_load("assets/Duck Sprite for Graphics Assignment2.png", &width, &height, &nrChannels, 0);
    if (picture)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, picture);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Image height: " << height << " Image Width: " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(picture);*/


    myShader.use();
    myShader.setInt("texture1", 0);
    //myShader.setInt("texture2", 1);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);



        /*float currentTime = glfwGetTime();
        myShader.setFloat("time", currentTime);
        myShader.setVec3("offsetPosition", 0.5, 0.5, 0.1);*/

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);

		//Draw Call
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
		//Drawing happens here!
		glfwSwapBuffers(window);
        glfwPollEvents();
	}
	printf("Shutting down...");
}
