#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, const int w, const int h) {
    glViewport(0, 0, w, h);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int compile_shader(const unsigned int shader_type, const char* shader_source) {
    const unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);
    int comp_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &comp_success);
    if (!comp_success) {
        char comp_fail_info[512];
        glGetShaderInfoLog(shader, sizeof(comp_fail_info), nullptr, comp_fail_info);
        std::cout << "Shader compilation fail!\n" << comp_fail_info << std::endl;
    }
    return shader;
}

unsigned int link_shader_program(const unsigned int vertex_shader, const unsigned int fragment_shader) {
    const unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    int link_success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        char link_fail_info[512];
        glGetProgramInfoLog(shader_program, sizeof(link_fail_info), nullptr, link_fail_info);
        std::cout << "Shader link fail!\n" << link_fail_info << std::endl;
    }
    return shader_program;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learning OpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    const char *vert_sh_src = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vert_sh_src);
    const char *frag_sh_src = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
    "}\0";
    unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, frag_sh_src);
    unsigned int shader_program = link_shader_program(vertex_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    float verts[] = {
        -1.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        -0.5, 0.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.5, 1.0, 0.0,
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(.2, .3, .3, 1.);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);
    glfwTerminate();
    return 0;
}