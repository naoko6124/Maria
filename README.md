# maria

simple game engine with lua binding

## how to run

clone the repo and use cmake to build it with your preferred compiler, i've used clang

## features

- 3d rendering
- skeletal animation
- lua binding
- entity component system
- custom shaders

## currently working on

- collision
- gui rendering
- tools for converting the models from fbx to the engine format

## how to use

enter the example folder to see a simple project built with it

### project.yaml

this is the file that the exe expects to execute the game, it has the game name, the icon, the screen resolution and the scenes

### scenes/scene1.yaml

this file is a scene rendered by the game engine, it has a sector to put the shaders used in the scene and a sector to put the entities with it's components

### scripts/knight.lua

the script has 2 functions:
- start: is executed once in the startup
- update: is executed every frame

currently, i didn't make any documentation for the engine binding with lua
but you can see for yourself the binding in the file `libmaria/core/script.h`

### models and textures

the files that the engine reads are custom, so you need the tool to convert your fbx/png to them, i am currently working on making those tools more usable

## libraries used

- [assimp](https://github.com/assimp/assimp)
- [entneko](https://github.com/naoko6124/entneko)
- [glfw](https://github.com/glfw/glfw)
- [glad](https://github.com/Dav1dde/glad)
- [glm](https://github.com/g-truc/glm)
- [lua](https://github.com/lua/lua)
- [stb](https://github.com/nothings/stb)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

## credits

knight model used in the example by [Lucas Coppio](https://scoppio.itch.io/fullplate-armor-knight)
