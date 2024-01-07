# SpiralKit for Defold
This extension is a complete framework for the Defold game engine. It drastically changes the way you make games with Defold.

Key features:
- Use C++ to script the game. You can also mix Lua if you want.
- Automatic import of image assets. All images added to the `/assets` directory can be automatically imported into image atlases, creating a factory for each image using a special Python script that you can call with an editor script `Edit -> SpiralKit - Import Assets`.
- Autocompletion of the framework API and imported assets, because it's C++, can be used with VS Code.
- Entire game is composed of a hierarchy of `SkObject`s.
- Defold's GUI system is not used, instead all UI elements are created with regular game objects.
  - There are `SkLabel`, `SkSprite`, `SkParticleFx` and `Button` classes.
  - Plus some useful UI functions.
- Scene Manager. Each scene is defined as a C++ class.
- Input Manager. To easily capture mouse or keyboard events.
- Signal Manager. For your own needs.
- Sound Manager. A simple one.
- Timeout. A nice custom dt based timeout class to use instead of regular timers.
- Math functions from the Godot game engine.

# Demo/review of the framework

[![From Lua to C++. My framework for low level Defold - SpiralKit](http://img.youtube.com/vi/Hhq2OScXDsI/0.jpg)](http://www.youtube.com/watch?v=Hhq2OScXDsI "From Lua to C++. My framework for low level Defold - SpiralKit")

# Why C++?
C++ isn't the best option for game development, but it's the only reasonable option for this framework. Because Defold is written in C++ you get access to the lower level Defold API.

But mainly because C++ is a statically typed language and Lua is not. Static typing is crucial in my opinion. It catches errors that would become runtime errors later in your development process and are harder to fix.

I consider myself to be very careful when writing Lua code, but still, in the process of rewriting my Lua code to C++, I've found several bugs thanks to static typing.

There are ways to enable static typing in Lua, but they only work half way.

Another reason is more appropriate types: you get integers, signed/unsigned, precisely packed structs with the memory layout you want. And arrays start at 0 :-)

The only major downside (well, two) is that callbacks are harder to manage in C++ (more boilerplate) and that C++ is not a modern language, which has its own disadvantages.

In the future I hope to rewrite my framework in some other more modern language that is much easier to use. Candidates are Jai, Zig and V.

# Disclaimer
Use at your own risk. Requires a deep understanding of the Defold game engine. There is no documentation, only the code.
You can use this extension as learning material and make something of your own.

I've made this framework because I found it difficult to work with Defold. There is currently a game in development using this framework. In terms of game engine features it's feature complete, so it was a good test for the framework.

Here is a video demo of the game

https://twitter.com/SergeyLerg/status/1643692786699444224/video/1

https://twitter.com/SergeyLerg/status/1637113054671384576/video/1
