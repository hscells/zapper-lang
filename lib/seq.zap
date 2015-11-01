(fn -map (f l r)
  (cond
    ((empty? l) (return r))
    (True (-map f (rest l) (conj r (f (first l)))))))

(fn map (f l)
  (-map f l (list)))

(fn -filter (p l r)
  (cond
    ((empty? l) (return r))
    ((p (first l)) (-filter p (rest l) (conj r (first l))))
    (True (-filter p (rest l) r))))

(fn filter (p l)
  (-filter p l (list)))

(fn -reduce (f l r)
  (cond
    ((empty? l) (return r))
    (True (-reduce f (rest l) (f r (first l))))))

(fn reduce (f l)
  (-reduce f (rest l) (first l)))
