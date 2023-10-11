#include "shaders.hpp"

std::string shaders::ReadShaderFile(const char* filePath) {
    std::string shaderCode;
    std::ifstream shaderFile;

    // Ensure ifstream objects can throw exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open the file
        shaderFile.open(filePath);
        std::stringstream shaderStream;

        // Read the file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();

        // Close the file
        shaderFile.close();

        // Convert the stream into a string
        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Error reading shader file: " << e.what() << std::endl;
    }
    // printf(shaderCode.c_str());
    return shaderCode;
}

GLuint shaders::genShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
    printf("Compiling Shaders:\n");
    std::string vertexShaderSource = shaders::ReadShaderFile(vertexShaderFile);
    std::string fragmentShaderSource = shaders::ReadShaderFile(fragmentShaderFile);

    const char* vertex = vertexShaderSource.c_str();
    const char* fragment = fragmentShaderSource.c_str();

    // Create and compile the shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        printf("- Vertex Shader Compiled; \n");
    }


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        printf("- Fragment Shader Compiled; \n");
    }

    // Create a shader program and link the shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        // Handle shader program linking error
        GLint logLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            // Retrieve and print the error log
            std::vector<GLchar> log(logLength + 1);
            glGetProgramInfoLog(shaderProgram, logLength, nullptr, log.data());
            // Print or log the error message
            std::cout << "ERROR::SHADER::LINKING_FAILED\n" << log.data() << std::endl;
        }
    } else {
        printf("- Shaders Linked. \n");
    }

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

shaders::Shader::Shader() {}

shaders::Shader::Shader(const char *vertexShader, const char *fragmentShader) {
    mProgram = genShaderProgram(vertexShader, fragmentShader);
}

void shaders::Shader::activate() {
    GLuint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currentProgram);
    if (currentProgram != mProgram) {
        glUseProgram(mProgram);
    }
}

shaders::Shader::~Shader()
{
    glDeleteProgram(mProgram);
}
