# Design

## Ingredient

The core program is written in C2x.  
For properties and settings, we will create a [simple marking language](specs/swm.md).  
For map and modding script, we will create a [script language](specs/sws.md).

## Structure

The core program will provide an essential game engine for rendering, plugin processing.  
It will also provide a C plugin interface for plugins.

The elements of game is provided by plugins, duch as game logics, units and buildings.  

A custom map can also consists of its own script, which is written in the [sws](specs/sws.md) language.
