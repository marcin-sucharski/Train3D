#include "shader-program.h"
#include "../env/file.h"
#include <iostream>

namespace train {
    namespace gfx {
        ShaderProgram::ShaderProgram(GLuint vs, GLuint fs, GLuint program) :
            vertexShader(vs), fragmentShader(fs), program(program) {
        }

        ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) {
            (*this) = std::move(shaderProgram);
        }

        ShaderProgram::~ShaderProgram() {
            if (program != 0) {
                glDetachShader(program, vertexShader);
                glDetachShader(program, fragmentShader);
                glDeleteProgram(program);
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
            }
        }

        ShaderProgram &ShaderProgram::operator=(ShaderProgram &&shaderProgram) {
            if (this != &shaderProgram) {
                vertexShader = std::move(shaderProgram.vertexShader);
                fragmentShader = std::move(shaderProgram.fragmentShader);
                program = std::move(shaderProgram.program);

                shaderProgram.vertexShader = 0;
                shaderProgram.fragmentShader = 0;
                shaderProgram.program = 0;
            }

            return *this;
        }

        bool ShaderProgram::isValid() const {
            return program != 0;
        }

        void ShaderProgram::bind() {
            glUseProgram(program);
        }

        void ShaderProgram::unbind() {
            glUseProgram(0);
        }

        void ShaderProgram::setUniform(const std::string &name,
                                       const glm::mat4x4 &m) {
            const auto uniform = glGetUniformLocation(program, name.c_str());
            glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(m));
        }

        void ShaderProgram::setUniform(const std::string &name, int value) {
            const auto uniform = glGetUniformLocation(program, name.c_str());
            glUniform1i(uniform, value);
        }

        void ShaderProgram::setUniform(const std::string &name, float value) {
            const auto uniform = glGetUniformLocation(program, name.c_str());
            glUniform1f(uniform, value);
        }

        ShaderProgram ShaderProgram::create(const std::string &vs,
                                            const std::string &fs,
                                            const res::VertexDefinition &vd) {
            GLuint vertexShader = compile(GL_VERTEX_SHADER, vs);
            GLuint fragmentShader = compile(GL_FRAGMENT_SHADER, fs);

            GLuint program = glCreateProgram();

            if (program == 0) {
                glDeleteShader(fragmentShader);
                glDeleteShader(vertexShader);

                return ShaderProgram(0, 0, 0);
            }

            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);

            for (const auto &e : vd.getElements())
                glBindAttribLocation(program, e.index, e.name.c_str());

            glLinkProgram(program);

            GLint isLinked;
            glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
            if (isLinked != GL_TRUE) {
                GLint len;
                std::string infoLog;

                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
                infoLog.resize(static_cast<std::size_t>(len));
                glGetProgramInfoLog(program, len, &len, &infoLog[0]);

                glDeleteProgram(program);
                glDeleteShader(fragmentShader);
                glDeleteShader(vertexShader);

                std::cerr << "Shader linking error: " << infoLog.c_str() << std::endl;
                throw std::exception();
            }

            return ShaderProgram(vertexShader, fragmentShader, program);
        }

        GLuint ShaderProgram::compile(GLenum shaderType,
                                      const std::string &source) {
            GLuint shader = glCreateShader(shaderType);

            if (shader == 0)
                return 0;

            const GLchar *src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint isCompiled;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

            if (isCompiled != GL_TRUE) {
                GLint len;
                std::string infoLog;

                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
                infoLog.resize(len);
                glGetShaderInfoLog(shader, len, &len, &infoLog[0]);

                glDeleteShader(shader);
                std::cerr << "Shader compilation error: " << infoLog.c_str() << std::endl;
                throw std::exception();
            }

            return shader;
        }

        ShaderProgram ShaderProgram::fromFile(const std::string &vertexShaderPath,
                                              const std::string &fragmentShaderPath,
                                              const res::VertexDefinition &vertexDefinition) {
            env::File vsFile(vertexShaderPath), fsFile(fragmentShaderPath);
            assert(vsFile.isValid() && fsFile.isValid());
            return create(vsFile.readTextToEnd(), fsFile.readTextToEnd(), vertexDefinition);
        }
    }
}