; comments are written in traditional lisp style

class(HelloWorld
  ; display `Hello World!` on the screen
  {fn(show(){
    print("Hello World!")
  })}
)

; instantiate the class
new(HelloWorld,hello_world)
; invoke the function
hello_world.show()
