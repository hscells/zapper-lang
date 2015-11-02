(ns "zapper.math")

(let PI 3.141519)

(fn dec (n) (- n 1))
(fn inc (n) (+ n 1))

(fn mod (a b)
  (- a (* b (/ a b))))

(fn pow (n exponent)
  (cond
    ((<= exponent 1) (return n))
    (True (* n (pow n (dec exponent))))))

(fn square (n) (pow n n))

(fn sum (n) (+ n 0))

(fn even? (n)
  (= 0 (mod n 2)))

(fn odd? (n)
  (not (even? n)))

(fn zero? (n)
  (= 0 n))
