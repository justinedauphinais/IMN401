#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <gtc/constants.hpp>
#include <chrono>
#include "utils.hpp"

namespace IMN401 {


    void printProgramError(GLuint program)
    {
        GLuint isLinked;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            std::cout << "Error : " + std::string(infoLog.begin(), infoLog.end()) + "\n";

            // We don't need the program anymore.
            glDeleteProgram(program);
            return;
        }
        else
            std::cout << "Shader compilation : OK" << std::endl;


    }

    void printPipelineError(GLuint pipeline)
    {
        GLuint isValid;
        glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, (int*)&isValid);
        if (isValid == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramPipelineiv(pipeline, GL_INFO_LOG_LENGTH, &maxLength);
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramPipelineInfoLog(pipeline, maxLength, &maxLength, &infoLog[0]);

            std::cout << "Error : " + std::string(infoLog.begin(), infoLog.end()) + "\n";
            // We don't need the program anymore.
            glDeleteProgram(pipeline);
            return;
        }
        else
            std::cout << "Pipeline : OK" << std::endl;

    }






    int main()
    {
        // Init GLFW
        glfwInit();

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return EXIT_FAILURE;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        const int width = 800;
        const int height = 800;

        GLFWwindow* window = glfwCreateWindow(width, height, "TP - From Scratch", NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, NULL);


        // Load all OpenGL functions using the glfw loader function
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL context" << std::endl;
            glfwTerminate();
            return EXIT_FAILURE;
        }


        // shaders
        std::string strVS = readFile("shaders/triangle-vs.glsl");
        const GLchar* vsCode = strVS.c_str();
        std::string strFS = readFile("shaders/triangle-fs.glsl");
        const GLchar* fsCode = strFS.c_str();



        // Initialization
        // =====================
        // TODO: init buffers, shaders, etc.
        // cf. https://www.khronos.org/files/opengl46-quick-reference-card.pdf

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        size_t n = 30;
        float radius = 0.5f;

        std::vector<glm::vec3> vertices;
        vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

        std::vector<glm::ivec3> indices;

        for (size_t i = 0; i < n; i++)
        {
            float theta = i * glm::two_pi<float>() / n;
            vertices.push_back(glm::vec3(radius * glm::cos(theta), radius * glm::sin(theta), 0.0f));
        }

        for (size_t i = 1; i < n; i++)
        {
            indices.push_back(glm::ivec3(0, i + 1, i));
        }
        indices.push_back(glm::ivec3(0, 1, n));



        GLuint EBO_indices;
        glCreateBuffers(1, &EBO_indices);
        glNamedBufferData(EBO_indices, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

        GLuint monBuffer;
        glCreateBuffers(1, &monBuffer);
        glNamedBufferData(monBuffer, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        GLuint VA;
        glCreateVertexArrays(1, &VA);
        glEnableVertexArrayAttrib(VA, 0);
        glVertexArrayAttribFormat(VA, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(VA, 0, monBuffer, 0, sizeof(glm::vec3));
        glVertexArrayAttribBinding(VA, 0, 0);

        glVertexArrayElementBuffer(VA, EBO_indices); 

        GLuint vprogram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vsCode);
        GLuint fprogram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fsCode);

        GLuint pipeline;
        glCreateProgramPipelines(1, &pipeline);

        glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vprogram);
        glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fprogram);

        GLint uniformLocation = glGetUniformLocation(fprogram, "color");
        glProgramUniform3f(fprogram, uniformLocation, 1.0f, 0.0f, 0.0f);

        auto start = std::chrono::system_clock::now();

        GLint uniformLocation2 = glGetUniformLocation(vprogram, "time");


        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "OpenGL error" << std::endl;
            return EXIT_FAILURE;
        }
        // ====================


        // Rendering loop
        while (!glfwWindowShouldClose(window))
        {
            // Handle events
            glfwPollEvents();


            // ==================
            // TODO: render here !

            glClear(GL_COLOR_BUFFER_BIT);

            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
            glProgramUniform1f(vprogram, uniformLocation2, now / 1000.0f);

            glBindProgramPipeline(pipeline);

            glBindVertexArray(VA);

            glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

            // ==================

            glfwSwapBuffers(window);
        }

        // Clean up
        glfwTerminate();

        return EXIT_SUCCESS;
    }
}

int main()
{
    return IMN401::main();
}