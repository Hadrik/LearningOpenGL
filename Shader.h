//
// Created by trric on 8/4/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>



class Shader {
public:
    explicit Shader(const char* vertex_path, const char* fragment_path);
    ~Shader();

    [[nodiscard]] unsigned int id() const;
    void use() const;
    void set(const std::string& name, bool value) const;
    void set(const std::string& name, int value) const;
    void set(const std::string& name, float value) const;

private:
    unsigned int _id;
};



#endif //SHADER_H
