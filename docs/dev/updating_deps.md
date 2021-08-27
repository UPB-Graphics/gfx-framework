-   **[Docs home](../home.md)**

# Updating dependencies

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

| Dependency                                    | Version           | Needed on
| :---                                          | :---              | :---
| GFXComponents                                 | private           | Windows
| [assimp](https://github.com/assimp/assimp)    | 525213a @ master | Windows
| [glew](https://github.com/nigels-com/glew)    | 3cdab58 @ master | Windows
| [glfw](https://github.com/glfw/glfw)          | 0b9e48f @ master | Windows
| [glm](https://github.com/g-truc/glm)          | 3a25105 @ master | Windows
| [stb](https://github.com/nothings/stb)        | b42009b @ master | Windows
| [spdlog](https://github.com/gabime/spdlog)    | c1af0a3 @ v1.x   | Windows
| [freetype](https://gitlab.freedesktop.org/freetype/freetype)  | 7227aab @ master | Windows
| [pfd](https://github.com/samhocevar/portable-file-dialogs)    | 0049adb @ master | all


## :bento: Dependencies

### `GFXComponents`

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
    cmake -B build-32 -S . -A Win32 -D LIBRARY_SUFFIX="" -D CMAKE_DEBUG_POSTFIX="" -D ASSIMP_BUILD_ZLIB=ON
    cmake --build build-32 --config Release
    ```
-   build the 64-bit binaries:
    ```
    cmake -B build-64 -S . -A x64 -D LIBRARY_SUFFIX="" -D CMAKE_DEBUG_POSTFIX="" -D ASSIMP_BUILD_ZLIB=ON
    cmake --build build-64 --config Release
    ```
-   copy the API into our project:
    ```
    dir:    include/assimp                      ->      deps/api
    file:   build-64/include/assimp/config.h    ->      deps/api/assimp
    ```
-   copy the binaries into our project:
    ```
    file:   build-32/lib/Release/assimp.lib     ->      deps/prebuilt/assimp/i686
    file:   build-32/bin/Release/assimp.dll     ->      deps/prebuilt/assimp/i686
    file:   build-64/lib/Release/assimp.lib     ->      deps/prebuilt/assimp/x86_64
    file:   build-64/bin/Release/assimp.dll     ->      deps/prebuilt/assimp/x86_64
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
    cmake -B build-32 -S build\cmake -A Win32
    cmake --build build-32 --config Release
    ```
-   build the 64-bit binaries:
    ```
    cmake -B build-64 -S build\cmake -A x64
    cmake --build build-64 --config Release
    ```
-   copy the API into our project:
    ```
    dir:    include/GL                          ->      deps/api
    ```
-   copy the binaries into our project:
    ```
    file:   build-32/lib/Release/glew32.lib     ->      deps/prebuilt/glew/i686
    file:   build-32/bin/Release/glew32.dll     ->      deps/prebuilt/glew/i686
    file:   build-64/lib/Release/glew32.lib     ->      deps/prebuilt/glew/x86_64
    file:   build-64/bin/Release/glew32.dll     ->      deps/prebuilt/glew/x86_64
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
    cmake -B build-32 -S . -A Win32 -D BUILD_SHARED_LIBS=ON
    cmake --build build-32 --config Release
    ```
-   build the 64-bit binaries:
    ```
    cmake -B build-64 -S . -A x64 -D BUILD_SHARED_LIBS=ON
    cmake --build build-64 --config Release
    ```
-   copy the API into our project:
    ```
    dir:    include/GLFW                        ->      deps/api
    ```
-   copy the binaries into our project:
    ```
    file:   build-32/src/Release/glfw3dll.lib   ->      deps/prebuilt/GLFW/i686
    file:   build-32/src/Release/glfw3.dll      ->      deps/prebuilt/GLFW/i686
    file:   build-64/src/Release/glfw3dll.lib   ->      deps/prebuilt/GLFW/x86_64
    file:   build-64/src/Release/glfw3.dll      ->      deps/prebuilt/GLFW/x86_64
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
    dir:    glm                                 ->      deps/api
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
    file:   stb_image.h                         ->      deps/api/stb
    file:   stb_image_write.h                   ->      deps/api/stb
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
    cmake -B build-32 -S . -A Win32 -D SPDLOG_BUILD_SHARED=ON
    cmake --build build-32 --config Release
    ```
-   build the 64-bit binaries:
    ```
    cmake -B build-64 -S . -A x64 -D SPDLOG_BUILD_SHARED=ON
    cmake --build build-64 --config Release
    ```
-   copy the API into our project:
    ```
    dir:    include/spdlog                      ->      deps/api
    ```
-   copy the binaries into our project:
    ```
    file:   build-32/Release/spdlog.lib         ->      deps/prebuilt/spdlog/i686
    file:   build-32/Release/spdlog.dll         ->      deps/prebuilt/spdlog/i686
    file:   build-64/Release/spdlog.lib         ->      deps/prebuilt/spdlog/x86_64
    file:   build-64/Release/spdlog.dll         ->      deps/prebuilt/spdlog/x86_64
    ```


### `freetype`

Follow these steps:

-   clone the repo:
    ```
    git clone https://gitlab.freedesktop.org/freetype/freetype.git
    cd freetype
    ```
-   build the 32-bit binaries:
    ```
    cmake -B build-32 -S . -A Win32 -D BUILD_SHARED_LIBS=ON -D CMAKE_DEBUG_POSTFIX=""
    cmake --build build-32 --config Release
    ```
-   build the 64-bit binaries:
    ```
    cmake -B build-64 -S . -A x64 -D BUILD_SHARED_LIBS=ON -D CMAKE_DEBUG_POSTFIX=""
    cmake --build build-64 --config Release
    ```
-   copy the API into our project:
    ```
    dir:    include/freetype                    ->      deps/api
    file:   include/ft2build.h                  ->      deps/api
    ```
-   copy the binaries into our project:
    ```
    file:   build-32/Release/freetype.lib       ->      deps/prebuilt/freetype/i686
    file:   build-32/Release/freetype.dll       ->      deps/prebuilt/freetype/i686
    file:   build-64/Release/freetype.lib       ->      deps/prebuilt/freetype/x86_64
    file:   build-64/Release/freetype.dll       ->      deps/prebuilt/freetype/x86_64
    ```


### `pfd`

Follow these steps:

-   clone the repo:
    ```
    git clone https://github.com/samhocevar/portable-file-dialogs
    cd portable-file-dialogs
    ```
-   copy the API into our project:
    ```
    dir:    portable-file-dialogs.h             ->      deps/api/pfd
    ```
