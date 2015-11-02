(import "seq")

(ns "zapper.str")

(fn concat (strings)
  (reduce + (map add-space strings)))

(fn add-space (string)
  (+ string " "))

(fn reverse (string)
  (reduce + (zapper.seq.reverse string)))
