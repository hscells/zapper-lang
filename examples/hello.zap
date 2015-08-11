-- comments are written in SQL style

class(HelloWorld{
  -- display `Hello World!` on the screen
  fn(show(){
    print("Hello World!")
  })
})

-- instantiate the class
new(HelloWorld,hello_world)
-- invoke the function
hello_world.show()
