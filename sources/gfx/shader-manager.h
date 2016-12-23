#ifndef TRAIN_GFX_SHADER_MANAGER_H
#define TRAIN_GFX_SHADER_MANAGER_H

#include <memory>
#include <unordered_map>
#include <string>

#include "../res/shader-preprocessor.h"
#include "../res/vertex-definition.h"
#include "shader-program.h"

namespace train {
    namespace gfx {
        class ShaderManager {
            res::ShaderPreprocessor preprocessor;
            std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shaderPrograms;

        public:
            ShaderManager();

            ShaderProgram& get(
                const std::string &vsPath,
                const std::string &fsPath,
                const res::VertexDefinition &vd
            );

        private:
            std::unique_ptr<ShaderProgram> loadShader(
                const std::string &vsPath,
                const std::string &fsPath,
                const res::VertexDefinition &vd
            );

            static std::string getPathKey(const std::string &vsPath, const std::string &fsPath);

            static res::ShaderPreprocessor::ShaderCode loadCode(const std::string &path);
        };
    }
}

#endif // TRAIN_GFX_SHADER_MANAGER_H
