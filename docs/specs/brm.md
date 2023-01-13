# BRM Format

BRM stands for _**B**lue Ale**R**t **M**arking Language_.  
It's an easy-to-use string data format.  
I said it's easy-to-use because it is easy-to-use(?)...  
BRM is parsed in a top-to-bottom basis, latter values will override former ones.

## Identifier rules

Any continuous UTF-8 string is an identifier. Use spaces to devide them.  
You may not use certain characters for identifiers (except some for the _raw string_ rule below).

|Character|Usage|
|:-:|:-:|
|=|Assignment|
|[|Block identifier left|
|]|Block identifier right|
|@|_See **Escape characters**_|
|"|_See **Raw Strings**_|
|#|Comment|
|/|Comment|

## \r

`\r` is always ignored under any circumstance.

## Comments

    # This is a comment
    // This is also a comment

Use # or // to comment. See below if you want to use these characters as values.  
Comments last for a line. _There is no multiline comment._

## Flags

    enable_world_espace

Such a single word without any decroation will become a flag. It does not have any value
associated with, just exists.

## Pairs

    world = does_not_exist

A normal key-value pair. Spaces are not allowed in pairs, e.g.

    food = hamburger fries gasoline

will be parsed as

    food = hamburger
    fries
    gasoline

Sublayers are provided.

    world.seed = 12c329dd
    world.seed.use = less

Do not use names like

    world.gen... = ..13.49.2fj..
    .f4nvv.riv

Sublayer mark in value of pairs is allowed.

    ip_address = 192.168.233.233

## Blocks

A support for sublayers, helps gathering information in the same category.

    [unit_1345]
    hp = 555
    x = 13
    y = 27

is the same as

    unit_1345.hp = 555
    unit_1345.x = 13
    unit_1345.y = 27

Sublayer of blocks are allowed.

    [block.return]
    ok

If you want to end a region of a block and enter global field again, use

    []

## Escape characters

Use @ to escape a single character.  
This is the only way to use `"`(double quote) for values.  

    dj.speech = "He said"@ @""number of varaibles...
    is very interesting"@"

## Raw Strings

Use `"` to group a raw string. Anything, including line separators and comment characters will be
treated as part of the raw string.

    speech = "yoU kNow i aLwaYs wA
    n //0---------251--14
                t to uSe That # as a goo0
                Character but
              it just DOESN'T?! 1425"

    "btw this is a raw string flag"

If comment characters and `"` appear in the same line, the former one will override the latter one.  
_Line separators will not be parsed as spaces; lines are simply connected._

    talk = "He said: "I want that.""

will becomes

    talk = "He said: "
    I
    want
    that.

And,

    saying = "I think we actually can use " as value for some point."

will results in trash (the last `"` is unresolved) unless there's another `"` below it.
