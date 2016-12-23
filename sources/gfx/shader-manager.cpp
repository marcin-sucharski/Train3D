#include <functional>
#include "shader-manager.h"
#include "../env/file.h"

using namespace std;
using namespace std::placeholders;

namespace train {
    namespace gfx {
        ShaderManager::ShaderManager() : preprocessor(bind(ShaderManager::loadCode, _1)) {
        }

        ShaderProgram &ShaderManager::get(
            const string &vsPath,
            const string &fsPath,
            const res::VertexDefinition &vd
        ) {
            auto key = getPathKey(vsPath, fsPath);
            auto maybeProgram = shaderPrograms.find(key);
            if (maybeProgram != shaderPrograms.end())
                return *maybeProgram->second;
            else
                return *(shaderPrograms[key] = loadShader(vsPath, fsPath, vd));
        }

        std::unique_ptr<ShaderProgram> ShaderManager::loadShader(
            const string &vsPath,
            const string &fsPath,
            const res::VertexDefinition &vd
        ) {
            return std::unique_ptr<ShaderProgram>(new ShaderProgram(ShaderProgram::create(
                preprocessor.preprocess(loadCode(vsPath)),
                preprocessor.preprocess(loadCode(fsPath)),
                vd
            )));
        }

        std::string ShaderManager::getPathKey(const std::string &vsPath, const std::string &fsPath) {
            return vsPath + "|" + fsPath;
        }

        res::ShaderPreprocessor::ShaderCode ShaderManager::loadCode(const string &path) {
            return env::File("data/shaders/" + path).readTextToEnd();
        }
    }
}