#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <type_traits>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace shaders {
    
    std::string ReadShaderFile(const char* filePath);

    GLuint genShaderProgram(const char* vertexShader, const char* fragmentShader);
    GLuint genShaderProgram(const char* vertexShader);

    class Shader {
        public:
        GLuint mProgram;

        Shader();
        Shader(const char* vertexShader, const char* fragmentShader);
        Shader(const char* vertexShader);

        void activate();

        template <typename dataType>
        void setUniformVec(const char *name, dataType &data)
        {
            activate();

            GLuint location = glGetUniformLocation(mProgram, name);

            // todo seperate into if blocks
            if (typeid(dataType) == typeid(glm::mat4)) {
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
            } else if (typeid(dataType) == typeid(glm::vec3)) {
                glUniform3fv(location, 1, glm::value_ptr(data));
            }   
        }

        template <typename dataType>
        void setUniform(const char *name, dataType &data)
        {
            activate();

            GLuint location = glGetUniformLocation(mProgram, name);

            if (typeid(dataType) == typeid(float)) {
                glUniform1f(location, data);
            } else if (typeid(dataType) == typeid(int)) {
                glUniform1i(location, data);
            }   
        }

        ~Shader();
    };

    GLuint genCompProgram(const char *compShaderFile);

    class Compute : public Shader {
        public:
        Compute();
        Compute(const char* computeShader);
    };
}
