(ns "zapper.str")

(fn concat (strings)
  (reduce + (map add-space strings)))

(fn add-space (string)
  (+ string " "))

(fn reverse (string)
  (reverse string (length string) (list)))
(fn reverse (string count r) (
  (cond
    ((zero? count) (reduce + r))
    (True (reverse string (dec count) (conj r (nth string (dec count))))))))
