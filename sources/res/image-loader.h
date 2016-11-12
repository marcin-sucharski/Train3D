#ifndef TRAIN_RES_IMAGE_LOADER_H
#define TRAIN_RES_IMAGE_LOADER_H

#include <vector>
#include "image-data.h"
#include "../env/file.h"

namespace train {
    namespace res {
        struct ImageLoader {
            static ImageData load(env::File &source);
        };
    }
}

#endif
