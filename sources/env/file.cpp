#include "file.h"

namespace train {
    namespace env {
        File::File(const std::string &filename) : file(filename.c_str()) {
            if (file) {
                file.seekg(0, std::ios::end);
                size = static_cast<std::size_t>(file.tellg());
                file.seekg(0, std::ios::beg);
            }
        }

        std::vector<char> File::readBinaryToEnd() {
            std::vector<char> buffer(getSize());
            file.read(&buffer[0], getSize());
            return std::move(buffer);
        }

        std::string File::readTextToEnd() {
            std::string text(getSize(), ' ');
            file.read(&text[0], getSize());
            return std::move(text);
        }
    }
}