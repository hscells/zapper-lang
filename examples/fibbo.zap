-- demonstration of a recursive function

fn(fibbo(n){

  if (<(n,1)){
    return(0)
  }

  if (=(n,0)){
    return(1)
  }

  return(+(fibbo(-(n-1),fibbo(-(n,2)))))

})

print(fibbo(10))
