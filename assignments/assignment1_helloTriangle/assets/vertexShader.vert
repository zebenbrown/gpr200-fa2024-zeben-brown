unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Error::Shader::Vertex::Compilation_Failed\n%s", infoLog);
		//std::cout << "Error::Shader::Vertex::Compilation_Failed\n" << infoLog << "\n";
	}