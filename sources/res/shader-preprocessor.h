#ifndef TRAIN_RES_SHADER_PREPROCESSOR_H
#define TRAIN_RES_SHADER_PREPROCESSOR_H

#include <string>
#include <sstream>
#include <functional>
#include <stack>

namespace train {
    namespace res {
        struct ShaderPreprocessor {
            typedef std::string ShaderCode;
            typedef std::string Path;

            explicit ShaderPreprocessor(std::function<ShaderCode(Path)> fileProvider);

            std::string preprocess(const ShaderCode &input);

        private:
            const std::function<ShaderCode(Path)> fileProvider;

            std::stack<int> sourceStringNumbers;
            int sourceStringNumber;

            void handleDirective(const std::string &directive, std::ostringstream &output);
            void handleInclude(const std::string &directive, std::ostringstream &output);
            void handleName(const std::string &directive, std::ostringstream &output);
        };
    }
}

#endif // TRAIN_RES_SHADER_PREPROCESSOR_H
