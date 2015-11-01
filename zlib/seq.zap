(import "core")

(fn map (f l)
  (cond
    ((empty? l) (list))
    (True (conj (f (first l)) (map f (rest l))))))
