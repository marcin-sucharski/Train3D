#ifndef TRAIN_ENV_FILE_H
#define TRAIN_ENV_FILE_H

#include <fstream>
#include <vector>
#include <string>

namespace train {
    namespace env {
        class File {
            std::ifstream file;
            std::size_t size;

        public:
            explicit File(const std::string &filename);

            inline bool isValid() const {
                return file.good();
            }

            inline std::size_t getSize() const {
                return size;
            }

            std::vector<char> readBinaryToEnd();
            std::string readTextToEnd();
        };
    }
}

#endif
