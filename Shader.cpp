//
// Created by trric on 8/4/2025.
//

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertex_path, const char *fragment_path) {
    // Read files
    std::string vert_src;
    std::string frag_src;
    std::ifstream vert_file;
    std::ifstream frag_file;

    vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vert_file.open(vertex_path);
        frag_file.open(fragment_path);
        std::stringstream vss, fss;
        vss << vert_file.rdbuf();
        fss << frag_file.rdbuf();
        vert_file.close();
        frag_file.close();
        vert_src = vss.str();
        frag_src = fss.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Fail reading shader file!\n" << e.what() << std::endl;
    }

    // Compile shaders
    const char* vert_code = vert_src.c_str();
    const char* frag_code = frag_src.c_str();
    unsigned int vert_id, frag_id;
    int success;
    char info[512];

    vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &vert_code, nullptr);
    glCompileShader(vert_id);
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert_id, sizeof(info), nullptr, info);
        std::cerr << "Fail compiling vertex shader!\n" << info << std::endl;
    }

    frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_id, 1, &frag_code, nullptr);
    glCompileShader(frag_id);
    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_id, sizeof(info), nullptr, info);
        std::cerr << "Fail compiling fragment shader!\n" << info << std::endl;
    }

    // Create program
    const unsigned int program = glCreateProgram();
    glAttachShader(program, vert_id);
    glAttachShader(program, frag_id);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(info), nullptr, info);
        std::cerr << "Fail linking shader program!\n" << info << std::endl;
    }

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    _id = program;
}

Shader::~Shader() {
    glDeleteProgram(_id);
}

unsigned int Shader::id() const {
    return _id;
}

void Shader::use() const {
    glUseProgram(_id);
}

void Shader::set(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}

void Shader::set(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::set(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}
