#pragma once

#define DLLPROJECT_EXPORTS 

#ifdef DLLPROJECT_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#include "World.hpp"

extern "C" {
    EXPORT World* createWorld();
    EXPORT void freeWorld(World* instance);
    EXPORT void World_update(World* instance);
}