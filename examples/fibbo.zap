; demonstration of a recursive function

fn(fibbo,(n){
  if(<=(1,n),n,+(fibbo(-(n,1)), fibbo(-(n,2))))
})

print(fibbo(10))
