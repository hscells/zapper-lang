(let PI 3.141519)

(fn dec (n) (- n 1))
(fn inc (n) (+ n 1))

(fn pow (n exponent)
  (cond
    ((<= exponent 1) (return n))
    (True (* n (pow n (dec exponent)))))
