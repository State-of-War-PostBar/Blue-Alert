# Program Design

## Ingredient

The core program is written in C.  
For properties and settings, we will create a [simple marking language](specs/brm.md).  
For map and modding script, we will create a [script language](specs/brs.md).

## Principle

Procedural & data oriented designed.  
No object oriented trash. Objects are allowed, do not orient the design with them.

## Structure

The core program will provide an essential game engine for logic, rendering, plugin processing etc.
It will also provide a C plugin interface for modders.  
The elements of game is provided by plugins, such as game logics, units and buildings.  
A custom map can also consists of its own script, which can also be written in the [brs](specs/brs.md) language.
