(ns "zapper.math")

(let PI 3.141519)

(fn dec (n) (- n 1))
(fn inc (n) (+ n 1))

(fn neg (n)
  (- n n))

(fn mod (a b)
  (- a (* b (/ a b))))

(fn pow (n exponent)
  (cond
    ((<= exponent 1) (return n))
    (True (* n (pow n (dec exponent))))))

(fn square (n) (pow n n))

(fn sum (lst) (reduce + lst))

(fn even? (n)
  (= 0 (mod n 2)))

(fn odd? (n)
  (not (even? n)))

(fn zero? (n)
  (= 0 n))

(fn max (a b)
  (cond
    ((> a b) (return a))
    (True (return b))))

(fn min (a b)
  (cond
    ((> a b) (return b))
    (True (return a))))

(fn min (lst)
  (reduce min lst))

(fn max (lst)
  (reduce max lst))
