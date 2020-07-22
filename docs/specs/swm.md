# SWM Format

SWM stands for _**S**tate of **W**ar: Remastered **M**arking_.  
It's an easy-to-use one-to-one string data format.  
I said it's easy-to-use because it is easy-to-use(?)...  
SWM is parsed in a top-to-bottom case, latter values will override former ones.

## Identifier rules

A continuous UTF-8 string itself is an identifier. Use spaces to devide them.  
There are certain characters you may not use for identifiers (except some for the _raw string_ rule below).

|Character|Usage|
|:-:|:-:|
|=|Assignment|
|.|Sublayer mark|
|[|Block identifier left|
|]|Block identifier right|
|"|_See **raw string**_|
|#|Comment|
|$|Reserved for future use|
|<|Reserved for future use|
|>|Reserved for future use|
|&#124;|Reserved for future use|

## Escape characters

Do not exist.

## Comments

    # This is a comment
    // This is also a comment

Use # or // to comment. See below if you want to use these characters as values.
Comments last for a line. There is no multiline comment.

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

    world.seed = 12c329dd19f
    world.seed.use = less

Do not use names like

    world.gen... = 13492fj
    .f4nvv.riv

## Blocks

A support for sublayers, helps gathering.

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

If you want to end a region of a block to enter global field again, use

    []

Spaces are not allowed in block names. E.g.

    [building 321]

Everything after g and before [ will be discarded.

## Raw Strings

Use `"` to group a raw string. Anything, including line separators and comment characters will be
treated as part of the raw string.
_You cannot use `"` itself as a part of some value._

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
If the starting `"` reaches all the way to the end of file, everything inside the scope of the
`"` will be discarded.  
