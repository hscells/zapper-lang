# zapper-lang
Zapper is an experimental programming language, mainly for me to learn about programming theory.

Zapper aims to be purely functional as possible. Not only does Zapper aim to be functional, but also take the good parts of object
orientation such as encapsulation, polymorphism and inheritance. Down the line, due to both of these quirks, I hope
for programs written in Zapper to be very easy to parallelize. Zapper is written in C, and module support is hopefully
going to both come from not only a large libaray of modules written in the mother language, but extended with C. High
interoperability is key with this language and modules should be able to be written fairly effortlessly. Zapper is
an interpreted language for simplicity, but also because LISP languages should be written with the idea of 'code as data'.

At the moment, there is a basic structure for the language, and there are examples of
how I want it to look in the ```examples/``` folder. At the moment the following things still need to be implemented
(in order of what I think is important):

 - Parser & Tokeniser
 - Objects & Classes (The core is there)
 - Module Support
 - REPL
 - VM
 - Core Language
 - Garbage Collection
