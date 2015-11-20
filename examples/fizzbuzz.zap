(import core math)

(fn fizzbuzz (n) (
  (print n)
  (print " ")
  (print (if (= 0 (mod n 3)) "Fizz" ""))
  (print (if (= 0 (mod n 5)) "Buzz" ""))
  (println "")
  (cond
    ((< n 100) (fizzbuzz (inc n)))
    (True (True)))))

(fizzbuzz 1)
