(ns "zapper.core")

(let nil (list))

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

(fn true? (b)
  (= b True))

(fn or (a b)
  (cond
    ((true? a) (return True))
    ((true? b) (return True))
    (True (return False))))

(fn and (a b)
  (cond
    ((= (true? a) (true? b)) (return True))
    (True (return False))))

(fn if (predicate result)
  (cond
    ((true? predicate) (result))
    (True (nil))))

(fn if (predicate true false)
  (cond
    ((true? predicate) (true))
    (True (false))))

(fn quit () (exit 1))
