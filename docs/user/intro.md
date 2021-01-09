-   **[Docs home](../docs.md)**

# Introduction

## Overview

### What does it teach?

This is a project that aims to give you a basic understanding of:

-   How a graphics API works
-   How you can develop game logic in an object-oriented way
-   How algebra and geometry is used in graphics
-   How to use parallel computing via shaders
-   And more!


### What is a graphics API?

It's just a technical name for a set of functions that allow you to communicate with the graphics hardware of your computer. You don't know, and you shouldn't care, how these functions are actually implemented under the hood. All that's important is that you can call them from your language of choice. These functions allow you to draw geometric shapes, play with 2D or 3D models, add effects and much more. There are many competing graphics APIs out there, designed by different corporations and manufacturers.


## Technical choices

### Which API are we learning?

Graphics APIs generally fall into two broad categories:

-   Current generation (mid-1990s to mid-2020s)
    -   OpenGL (Windows, Linux, macOS)
    -   OpenGL ES (Android, iOS)
    -   Direct3D 11 (Windows)

-   Next generation (from mid-2020s onwards)
    -   Vulkan (Windows, Linux, macOS, Android, iOS, consoles)
    -   Direct3D 12 (Windows)
    -   Metal (macOS, iOS)

This project uses OpenGL, as it's the only one from the current generation that works on all major desktop systems.

*At least for the time being.* Next-generation standards are slowly but surely replacing the current-generation ones.


### Why not learn a next-generation API?

You could argue that we should be using, for example, Vulkan. After all, it's slowly becoming the graphics industry standard! As with anything involving standards, however, things are more complicated than they appear. There are several reasons why we're sticking with OpenGL in the near future:

-   This project does not focus primarily on learning a certain graphics API. It aims to give you a sense of how a basic graphics application works, and to allow you to learn concepts rather than implementation details. ***Going forward, you will be able to apply these concepts with any other graphics API or game engine.***
-   There's a fair chance your computer might not support it. Vulkan has only been around since mid-2010s, which in terms of standards is very, very new.
-   Vulkan is one step lower-level than OpenGL. For any practical purposes, one should write an abstraction layer on top of it. Using Vulkan directly for rendering is generally extremely tedious and not the best tool for education. In the future, should this project switch to Vulkan, we will take care to write a simple abstraction layer over some of its components.
-   OpenGL is not going away. Even if a manufacturer decides to deprecate direct support ([such as the road Apple is taking][ref-apple-twitter]), there are currently several industry efforts to implement the OpenGL API on top of Vulkan (e.g. [Zink][ref-zink-article]). OpenGL has been used for decades and is the primary API for non-gaming graphics applications (e.g. CAD software); the significant time and financial cost of porting these to Vulkan will simply not allow a transition in the near future.


[ref-zink-article]:         https://www.gamingonlinux.com/2020/11/valve-funds-open-source-developer-to-work-on-zink-the-opengl-on-vulkan-driver
[ref-apple-twitter]:        https://twitter.com/colincornaby/status/1275153748348682240
