; demonstration of a recursive function
(fn fib (n)
  ((cond
    ((<= n 1) (+ n 0))
    ((= 1 1) (+ (fib (- n 1)) (fib (- n 2)))))))

(println (fib 10))
