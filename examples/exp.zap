(let l (list 1 2 3 4))

(println "before conj:")
(println l)
(println (length l))

(conj l (list 1 2 3 4 5))

(println "after conj:")
(println l)
(println (length l))
