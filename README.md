# zapper-lang
Zapper is an experimental programming language, mainly for me to learn about programming theory.

I want Zapper to be a lispy language which opts to place the opening brace after the first element in a list. For this
reason, Zapper aims to be as purely functional as possible. It is my hope that the only functions included in the
language which change state are the ```print()``` and ```read()``` functions which are both fairly self explanatory, and
a language can't really avoid I/O. Not only does Zapper aim to be functional, but also take the good parts of object
orientation such as encapsulation, polymorphism and inheritance. Down the line, due to both of these quirks, I hope
for programs written in Zapper to be very easy to parallelize.

At the moment, Zapper is a feint gleam in my eye, there is a basic structure for the language, and there are examples of
how I want it to look in the ```examples/``` folder. At the moment the following things still need to be implemented
(in order of what I think is important):

 - Parser & Tokeniser
 - REPL
 - VM
 - Objects & Classes (The core is there)
 - Core Language
 - Garbage Collection
 - Module Support
