-   **[Docs home](../home.md)**

# Updating Windows dependencies

This is a short guide on how to update the third-party code in the `deps` directory.


## :white_check_mark: Prerequisites

-   Visual Studio 2019 or newer
-   `sh` via Git for Windows:
    -   install manually from the [official website](https://git-scm.com/downloads)
    -   ***or,*** via Chocolatey: `choco install -y git`
-   `make` for Windows:
    -   install manually from the [official website](https://packages.msys2.org/package/mingw-w64-x86_64-make)
    -   ***or,*** via Chocolatey: `choco install -y make`
-   Python 3, either vanilla or Anaconda:
    -   vanilla:
        -   install manually from the [official website](https://www.python.org/downloads/)
        -   ***or,*** via Chocolatey: `choco install -y python --pre`
    -   Anaconda:
        -   install manually from the [official website](https://www.anaconda.com/products/individual)
        -   ***or,*** via Chocolatey: `choco install -y anaconda3`


## :label: Current versions

Every time you update a dependency, make sure you also update this list.

| Dependency    | Version
| :---          | :---
| libEGXComp    | private
| [assimp](https://github.com/assimp/assimp)    | 88e5f341 @ master
| [glew](https://github.com/nigels-com/glew)    | 3cdab58d @ master
| [glfw](https://github.com/glfw/glfw)          | 0b9e48fa @ master
| [glm](https://github.com/g-truc/glm)          | 3a25105d @ master
| [stb](https://github.com/nothings/stb)        | b42009b3 @ master
| [spdlog](https://github.com/gabime/spdlog)    | 42c5eb59 @ v1.x


## :bento: Dependencies

### `libEGXComp`

The sources for these binaries are private. If you need builds for a certain architecture, platform etc.,
file a Github issue and let us know. Even better, take a shot at implementing the hidden functionality yourself!


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
    file:   assimp/build-32/lib/Release/assimp.lib  ->      deps/prebuilt/assimp/i686
    file:   assimp/build-32/bin/Release/assimp.dll  ->      deps/prebuilt/assimp/i686
    file:   assimp/build-64/lib/Release/assimp.lib  ->      deps/prebuilt/assimp/x86_64
    file:   assimp/build-64/bin/Release/assimp.dll  ->      deps/prebuilt/assimp/x86_64
    ```


### `glew`

Follow these steps:

-   clone the repo:
    ```
    git clone https://github.com/nigels-com/glew
    cd glew
    ```
-   generate the source files:
    ```
    "C:\Program Files\Git\bin\sh" -c "cd auto && make"
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
    file:   glew/build-32/lib/Release/glew32.lib    ->      deps/prebuilt/glew/i686
    file:   glew/build-32/bin/Release/glew32.dll    ->      deps/prebuilt/glew/i686
    file:   glew/build-64/lib/Release/glew32.lib    ->      deps/prebuilt/glew/x86_64
    file:   glew/build-64/bin/Release/glew32.dll    ->      deps/prebuilt/glew/x86_64
    ```


### `glfw`

Follow these steps:

-   clone the repo:
    ```
    git clone https://github.com/glfw/glfw
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
-   build the 64-bit binaries:
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
    file:   glfw/build-32/src/Release/glfw3dll.lib  ->      deps/prebuilt/GLFW/i686
    file:   glfw/build-32/src/Release/glfw3.dll     ->      deps/prebuilt/GLFW/i686
    file:   glfw/build-64/src/Release/glfw3dll.lib  ->      deps/prebuilt/GLFW/x86_64
    file:   glfw/build-64/src/Release/glfw3.dll     ->      deps/prebuilt/GLFW/x86_64
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
    git clone https://github.com/nothings/stb
    cd stb
    ```
-   copy the API into our project (only the components we need):
    ```
    file:   stb/stb_image.h                         ->      deps/api/stb
    file:   stb/stb_image_write.h                   ->      deps/api/stb
    ```


### `spdlog`

Follow these steps:

-   clone the repo:
    ```
    git clone https://github.com/gabime/spdlog
    cd spdlog
    ```
-   build the 32-bit binaries:
    ```
    mkdir build-32
    cd build-32
    cmake .. -A Win32 -DSPDLOG_BUILD_SHARED=ON
    cmake --build . --config Release
    cd ..
    ```
-   build the 64-bit binaries:
    ```
    mkdir build-64
    cd build-64
    cmake .. -A x64 -DSPDLOG_BUILD_SHARED=ON
    cmake --build . --config Release
    cd ..
    ```
-   copy the API into our project:
    ```
    dir:    spdlog/include/spdlog                   ->      deps/api/spdlog
    ```
-   copy the binaries into our project:
    ```
    file:   spdlog/build-32/Release/spdlog.lib      ->      deps/prebuilt/spdlog/i686
    file:   spdlog/build-32/Release/spdlog.dll      ->      deps/prebuilt/spdlog/i686
    file:   spdlog/build-64/Release/spdlog.lib      ->      deps/prebuilt/spdlog/x86_64
    file:   spdlog/build-64/Release/spdlog.dll      ->      deps/prebuilt/spdlog/x86_64
    ```
