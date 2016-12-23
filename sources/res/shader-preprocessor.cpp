#include <sstream>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "shader-preprocessor.h"

using namespace std;

namespace train {
    namespace res {
        ShaderPreprocessor::ShaderPreprocessor(
            function<ShaderPreprocessor::ShaderCode(ShaderPreprocessor::Path)> fileProvider
        ) : fileProvider(move(fileProvider)), sourceStringNumber(0) {
        }

        std::string ShaderPreprocessor::preprocess(const ShaderCode &input) {
            ostringstream output;
            sourceStringNumbers.push(++sourceStringNumber);

            int lineNumber = 1;
            bool firstLineCharacter = true;
            for (auto it = input.begin(); it != input.end(); ++it) {
                switch (*it) {
                    case '\n':
                        firstLineCharacter = true;
                        ++lineNumber;
                        output << *it;
                        output << "#line " << lineNumber << " " << sourceStringNumbers.top() << endl;
                        break;

                    case '#':
                        if (firstLineCharacter) {
                            auto newline = std::find(it, input.end(), '\n');
                            auto directive = string(it, newline);
                            handleDirective(directive, output);
                            it = newline - 1;
                            break;
                        }

                    default:
                        firstLineCharacter = false;
                        output << *it;
                        break;
                }
            }

            sourceStringNumbers.pop();
            return output.str();
        }

        void ShaderPreprocessor::handleDirective(const string &directive, ostringstream &output) {
            assert(*directive.begin() == '#');

            const auto identifierNameBeg = directive.begin() + 1;
            string::const_iterator identifierNameEnd;
            for (auto it = identifierNameBeg; it != directive.end(); ++it) {
                if (!isalnum(*it)) {
                    identifierNameEnd = it;
                    break;
                }
            }

            string identifier(identifierNameBeg, identifierNameEnd);

            output << "//<BEGIN>[" << directive << "]" << endl;
            if (identifier == "include") {
                handleInclude(directive, output);
            } else if(identifier == "name") {
                handleName(directive, output);
            } else {
                output << directive;
            }
            output << "//<END>[" << directive << "]" << endl;
        }

        void ShaderPreprocessor::handleInclude(const std::string &directive, std::ostringstream &output) {
            auto pathBegin = directive.find_first_of("\"");
            auto pathEnd = directive.find_last_of("\"");
            auto path = directive.substr(pathBegin + 1, pathEnd - pathBegin - 1);

            auto includeCode = preprocess(fileProvider(path));
            output << includeCode << endl;
        }

        void ShaderPreprocessor::handleName(const std::string &directive, std::ostringstream &output) {
            auto nameBegin = directive.find_first_of("\"");
            auto nameEnd = directive.find_last_of("\"");
            auto name = directive.substr(nameBegin + 1, nameEnd - nameBegin - 1);
            cerr << "[info] Source with id=" << sourceStringNumbers.top() << " has name=\"" << name << "\"" << endl;
        }
    }
}