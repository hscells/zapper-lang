(ns "zapper.seq")

(fn map (f l)
  (map f l (list)))

(fn map (f l r)
  (cond
    ((empty? l) (return r))
    (True (map f (rest l) (conj r (f (first l)))))))

(fn filter (p l)
  (filter p l (list)))

(fn filter (p l r)
  (cond
    ((empty? l) (return r))
    ((p (first l)) (filter p (rest l) (conj r (first l))))
    (True (filter p (rest l) r))))

(fn reduce (f l)
  (reduce f (rest l) (first l)))

(fn reduce (f l r)
  (cond
    ((empty? l) (return r))
    (True (reduce f (rest l) (f r (first l))))))

(fn reverse (lst)
  (reverse lst (length lst) (list)))
(fn reverse (lst count r) (
  (cond
    ((zero? count) (return r))
    (True (reverse lst (dec count) (conj r (nth lst (dec count))))))))

(fn range (stop)
  (range stop 0 (list)))
(fn range (start stop)
  (range stop start (list)))
(fn range (stop c r)
  (cond
    ((zero? stop) (return r))
    (True (range (dec stop) (inc c) (conj r c)))))

(fn zip (la lb)
  (zip la lb (list)))
(fn zip (la lb r)
  (cond
    ((empty? la) (return r))
    (True (zip (rest la) (rest lb) (conj r (conj (conj (list) (first la)) (first lb)))))))

(fn slist (l r)
  (cond
    ((empty? l) (return r))
    (True (slist (rest l) (conj r (eval (first l)))))))
(fn slist (l) (slist l (list)))
