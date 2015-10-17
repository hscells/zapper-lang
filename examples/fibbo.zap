; demonstration of a recursive function
(fn fib [n]
  (cond
    (<= 1 n) n
    (true (+ (fib (- n 1)) (fib (- n 2)))))

(println (fib 5))
