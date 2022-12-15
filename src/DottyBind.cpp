#include "DottyBind.hpp"

EXPORT World* createWorld(){
    return new World();
}

EXPORT void freeWorld(World* instance){
    delete instance;
}

EXPORT void World_update(World* instance){
    instance->update(); 
}