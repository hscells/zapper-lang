(ns "zapper.core")

(fn empty? (l)
  (cond
    ((= 0 (length l)) (return True))
    (True (return False))))

(fn not (b)
  (cond
    ((= b True) (return False))
    ((= b False) (return True))))

(fn apply (f args) (apply f args (conj (list) f)))
(fn apply (f args r)
  (cond
    ((empty? args) (eval r))
    (True (apply f (rest args) (conj r (first args))))))
