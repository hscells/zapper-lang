(import "math")

(fn fizzbuzz (n) (
  (print n)
  (print " ")
  (cond
    ((= 0 (mod n 3)) (print "Fizz")))
  (cond
    ((= 0 (mod n 5)) (print "Buzz")))
  (println "")
  (cond
    ((< n 100) (fizzbuzz (inc n))))))

(fizzbuzz 1)
