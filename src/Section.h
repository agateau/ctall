#ifndef SECTION_H
#define SECTION_H

// std
#include <vector>

namespace SDL2pp {
class Texture;
class Point;
}

class World;

/**
 * Triggers something when the cell is about to become visible
 */
class Trigger {
public:
    virtual ~Trigger();
    virtual void exec(World& world, const SDL2pp::Point& position) const = 0;
};

struct Section {
    struct Column {
        const std::vector<const SDL2pp::Texture*> images;
        const std::vector<const Trigger*> triggers;
    };
    const std::vector<Column> columns;
};


#endif // SECTION_H
