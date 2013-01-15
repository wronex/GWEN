# GWEN

## What is GWEN
A simple GUI system aimed at games.

## What does it look like?
Like this by default

![Screenshot of GWEN](https://raw.github.com/wronex/GWEN/master/gwen/doc/images/DefaultSkin_Preview.png)

The skin is a single texture - easy to edit. Even the text colours are defined in the texture - by reading the colour of the pixels at the bottom. This is the default texture:

![Default skin](https://raw.github.com/wronex/GWEN/master/gwen/skins/DefaultSkin.png)

## Facts
 * Coded in C++
 * Fully Namespaced
 * All standard window controls
 * Behaves like you'd expect
 * Lightweight
   - No XML readers, no font loaders/renderers, no texture loaders - your engine should be doing all this stuff!
 * Easy to integrate (comes with renderers for Windows GDI, Allegro, OpenGL, Direct2D, DirectX and SFML)
 * Totally portable & cross platform
 * Doesn't need RTTI

Released under a "do whatever you want" MIT license. See LICENSE.md