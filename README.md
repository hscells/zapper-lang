# zapper-lang
Zapper is an experimental programming language, mainly for me to learn about programming theory. It is a LISP written partly in C and itself.

At it's heart Zapper aims to be a general purpose language, but also tries to stay functional by not allowing for mutable objects and provides interfaces for functional-style programming.

At the moment, there is a basic structure for the language, and there are examples of actual code in the ```examples/``` folder. If you would like to try it out, it can currently be built on Mac and Linux (untested) systems, as well as possibly Windows (also untested) by running the ```make``` command. Binaries built for Mac are available on [the releases list](https://github.com/hscells/zapper-lang/releases).

If you use the Atom editor, there is a syntax highlighter available [here](https://github.com/hscells/language-zapper).

At the moment the following things still need to be implemented
(in order of what I think is important):

 - ~~Parser & Tokeniser~~
 - ~~Objects~~ & Classes
 - Core Language
 - Package Support
 - ~~REPL~~
 - VM
 - Garbage Collection

## Running Zapper

```zapper``` by itself will invoke the Zapper REPL.

```zapper file.zap``` will attempt to run file.zap
