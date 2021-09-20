-   **[Docs home](../home.md)**

# Introduction

## Overview

### What does it do?

This is a project that aims to give you a basic understanding of:

-   How a graphics API works
-   How to develop game logic in an object-oriented way
-   How algebra and geometry are used in graphics
-   How to use parallel computing via shaders
-   And more!


### What is a graphics API?

It's nothing more than a technical name for a set of functions that allow you to communicate with the graphics hardware of your computer. You do not know, and you should not care, how these functions are actually implemented under the hood. All that's important is that you can call them from your language of choice (C++ in this case). These functions allow you to draw geometric shapes, play with 2D or 3D models, add effects and much more. There are many competing graphics APIs out there, designed by different corporations and manufacturers.


## Technical choices

### Which API are we learning?

Graphics APIs generally fall into two broad categories:

-   Current generation: launched in the 2000s, and widespread adoption is expected to last until the 2020s
    -   OpenGL 3.x/4.x (Windows, Linux, macOS)
    -   OpenGL ES (Android, iOS)
    -   Direct3D 11 (Windows)
    -   WebGL 1.x/2.x (browsers)

-   Next generation: launched in the 2010s, and widespread adoption is expected to begin in the 2020s
    -   Vulkan (Windows, Linux, macOS, Android, iOS, consoles, VR/AR devices)
    -   Direct3D 12 (Windows)
    -   Metal (macOS, iOS)
    -   WebGPU (browsers)

This project uses OpenGL 3.x/4.x, as it's the only one from the current generation that works on all major desktop systems.


### Why not learn a next-generation API?

You could argue that we should be using, for example, Vulkan. After all, it's slowly becoming the graphics industry standard! As with anything involving next-gen technologies, however, things are more complicated than they appear. There are several reasons why we're sticking with OpenGL in the near future:

-   This project does not focus primarily on learning a certain graphics API. It aims to give you a sense of how a basic graphics application works, and to allow you to learn concepts rather than implementation details. ***Going forward, you will be able to apply these concepts with any other graphics API or game engine.***
-   OpenGL is easy to learn. Vulkan is one step lower-level than OpenGL. For example, in 2021, Valve implemented the OpenGL API using Vulkan under the hood (see the [Mesa Zink][ref-zink-article] project). Using Vulkan directly for rendering is generally extremely tedious and not the best tool for education, because, for any practical purposes, one should write an abstraction layer on top of it. There are many open-source abstraction layers being developed, however we currently avoid using any, as they run the risk of becoming unmaintained at some point in the future; should this project switch to Vulkan, we will first need to make sure that we choose the right tools for the job.
-   OpenGL is not going away. Even if a manufacturer decides to deprecate direct support ([such as the road Apple is taking][ref-apple-twitter]), there are currently several industry efforts to maintain support (e.g. Zink). Furthermore, OpenGL has been used for decades and is the primary API for non-gaming graphics applications (e.g. CAD software); the significant time and financial cost of porting these to Vulkan will simply not allow a quick transition.


### What if I still want to experiment with Vulkan?

At this time, you can do so on your own (and preferably after you complete the tasks in this framework!). Below you will find a list of resources to get you started:

- [tutorial](https://github.com/alaingalvan/vulkan-seed) by Alain Galvan
- [practical examples](https://github.com/SaschaWillems/Vulkan) by Sascha Willems
- [C++20 abstraction layer](https://github.com/liblava/liblava) by Lava Block


[ref-zink-article]:         https://www.gamingonlinux.com/2020/11/valve-funds-open-source-developer-to-work-on-zink-the-opengl-on-vulkan-driver
[ref-apple-twitter]:        https://twitter.com/colincornaby/status/1275153748348682240
