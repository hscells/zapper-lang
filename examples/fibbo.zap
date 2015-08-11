-- demonstration of a recursive function

fn(fibbo(n){

  -- if operates using if (predicate, {expression})
  -- down at the lower level, if is just a function
  if (<(n,1), {
    return(0)
  })

  if (=(n,0), {
    -- same with return, there is nothing hacked on, it is pure
    return(1)
  })

  return(+(fibbo(-(n-1),fibbo(-(n,2)))))

})

print(fibbo(10))
