#ifndef TRAIN_GFX_SHADER_H
#define TRAIN_GFX_SHADER_H

#include "../ogl.h"
#include <string>
#include "../res/vertex-definition.h"

namespace train {
    namespace gfx {
        class ShaderProgram {
            GLuint vertexShader;
            GLuint fragmentShader;
            GLuint program;

        public:
            ShaderProgram(const ShaderProgram &) = delete;
            ShaderProgram(ShaderProgram &&shaderProgram);
            ~ShaderProgram();

            ShaderProgram &operator=(const ShaderProgram &) = delete;
            ShaderProgram &operator=(ShaderProgram &&shaderProgram);

            bool isValid() const;

            void bind();
            void unbind();

            void setUniform(const std::string &name, const glm::mat4x4 &m);
            void setUniform(const std::string &name, const glm::vec2 &value);
            void setUniform(const std::string &name, const glm::vec3 &value);
            void setUniform(const std::string &name, int value);
            void setUniform(const std::string &name, float value);

            static ShaderProgram create(const std::string &vs,
                                        const std::string &fs,
                                        const res::VertexDefinition &vd);

            static ShaderProgram fromFile(const std::string &vertexShaderPath,
                                          const std::string &fragmentShaderPath,
                                          const res::VertexDefinition &vertexDefinition);

        private:
            ShaderProgram(GLuint vs, GLuint fs, GLuint program);

            static GLuint compile(GLenum shaderType, const std::string &source);
        };
    }
}

#endif
