# Updating Windows dependencies

This is a short guide on how to update the third-party code in the `deps` directory.


## :white_check_mark: Prerequisites

-   Visual Studio 2019 or newer
-   some form of `bash`, for example via WSL2


## :label: Current versions

Every time you update a dependency, make sure you also update this list.

| Dependency    | Version
| :---          | :---
| libEGC        | private
| assimp        | 88e5f341 @ master
| glew          | 3cdab58d @ master
| glfw          | 0b9e48fa @ master
| glm           | 3a25105d @ master
| stb           | b42009b3 @ master


## :bento: Dependencies

### `libEGC`

These sources for these binaries are private. If you need builds for a certain architecture, platform etc., file a Github issue and let us know. Even better, try implementing your own!


### `assimp`

Follow these steps:

-   clone the repo:
    ```
    git clone https://github.com/assimp/assimp
    cd assimp
    ```
-   build the 32-bit binaries:
    ```
    mkdir build-32
    cd build-32
    cmake .. -A Win32 -DLIBRARY_SUFFIX="" -DCMAKE_DEBUG_POSTFIX=""
    cmake --build . --config Release
    cd ..
    ```
-   build the 64-bit binaries:
    ```
    mkdir build-64
    cd build-64
    cmake .. -A x64 -DLIBRARY_SUFFIX="" -DCMAKE_DEBUG_POSTFIX=""
    cmake --build . --config Release
    cd ..
    ```
-   copy the API into our project:
    ```
    dir:    assimp/include/assimp                   ->      deps/api
    file:   assimp/build-64/include/assimp/config.h ->      deps/api/assimp
    ```
-   copy the binaries into our project:
    ```
    file:   assimp/build-32/lib/Release/assimp.lib  ->      deps/prebuilt/assimp/i686/Release
    file:   assimp/build-32/bin/Release/assimp.dll  ->      deps/prebuilt/assimp/i686/Release
    file:   assimp/build-64/lib/Release/assimp.lib  ->      deps/prebuilt/assimp/x86_64/Release
    file:   assimp/build-64/bin/Release/assimp.dll  ->      deps/prebuilt/assimp/x86_64/Release
    ```

### `glew`

Follow these steps:

-   clone the repo:
    ```
    https://github.com/nigels-com/glew
    cd glew
    ```
-   generate the source files:
    ```
    bash -c "cd auto && make"
    ```
-   build the 32-bit binaries:
    ```
    mkdir build-32
    cd build-32
    cmake ..\build\cmake -A Win32
    cmake --build . --config Release
    cd ..
    ```
-   build the 64-bit binaries:
    ```
    mkdir build-64
    cd build-64
    cmake ..\build\cmake -A x64
    cmake --build . --config Release
    cd ..
    ```
-   copy the API into our project:
    ```
    dir:    glew/include/GL                         ->      deps/api
    ```
-   copy the binaries into our project:
    ```
    file:   glew/build-32/lib/Release/glew32.lib    ->      deps/prebuilt/glew/i686/Release
    file:   glew/build-32/bin/Release/glew32.dll    ->      deps/prebuilt/glew/i686/Release
    file:   glew/build-64/lib/Release/glew32.lib    ->      deps/prebuilt/glew/x86_64/Release
    file:   glew/build-64/bin/Release/glew32.dll    ->      deps/prebuilt/glew/x86_64/Release
    ```

### `glfw`

Follow these steps:

-   clone the repo:
    ```
    https://github.com/glfw/glfw
    cd glfw
    ```
-   build the 32-bit binaries:
    ```
    mkdir build-32
    cd build-32
    cmake .. -A Win32 -DBUILD_SHARED_LIBS=ON
    cmake --build . --config Release
    cd ..
    ```
-   build the 32-bit binaries:
    ```
    mkdir build-64
    cd build-64
    cmake .. -A x64 -DBUILD_SHARED_LIBS=ON
    cmake --build . --config Release
    cd ..
    ```
-   copy the API into our project:
    ```
    dir:    glfw/include/GLFW                       ->      deps/api
    ```
-   copy the binaries into our project:
    ```
    file:   glfw/build-32/src/Release/glfw3dll.lib  ->      deps/prebuilt/GLFW/i686/Release
    file:   glfw/build-32/src/Release/glfw3.dll     ->      deps/prebuilt/GLFW/i686/Release
    file:   glfw/build-64/src/Release/glfw3dll.lib  ->      deps/prebuilt/GLFW/x86_64/Release
    file:   glfw/build-64/src/Release/glfw3.dll     ->      deps/prebuilt/GLFW/x86_64/Release
    ```

### `glm`

Follow these steps:

-   clone the repo:
    ```
    git clone https://github.com/g-truc/glm
    cd glm
    ```
-   copy the API into our project:
    ```
    dir:    glm/glm                                 ->      deps/api
    ```

### `stb` components

Follow these steps:

-   clone the repo:
    ```
    https://github.com/nothings/stb
    cd stb
    ```
-   copy the API into our project (only the components we need):
    ```
    file:   stb/stb_image.h                         ->      deps/api/stb
    file:   stb/stb_image_write.h                   ->      deps/api/stb
    ```