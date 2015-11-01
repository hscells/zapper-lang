(import "core")

(fn -map (f l r)
  (cond
    ((empty? l) (return r))
    (True (-map f (rest l) (conj r (f (first l)))))))

(fn map (f l)
  (-map f l (list))
