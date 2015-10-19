; demonstration of a recursive function
(fn fib (n)
  (cond
    ((<= n 1) (return n))
    (True (+ (fib (- n 1)) (fib (- n 2))))))

(println (fib 10))
