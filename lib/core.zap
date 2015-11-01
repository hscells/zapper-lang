(fn empty? (l)
  (cond
    ((= 0 (length l)) (return True))
    (True (return False))))

(fn not (b)
  (cond
    ((= b True) (return False))
    ((= b False) (return True))))
