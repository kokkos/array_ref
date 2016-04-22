A 
          30 x 30
striding   1 x  1
padding    0 x  0

B
           6 x  6
striding   5 x  5 
padding    0 x  0

C
          25 x 25
striding   1 x  1 
padding    5 x  5

C
           5 x  5
striding   5 x  5 
padding    5 x  5

A.size() == 30 * 30
B.size() ==  6 *  6
C.size() == 25 * 25

A.span() == ((30 * 1) + 0) * ((30 * 1) + 0)
B.span() == (( 6 * 5) + 0) * (( 6 * 5) + 0)
C.span() == (( 5 * 5) + 5) * (( 5 * 5) + 5)

A.size() == A.span()
B.size() != B.span()
C.size() != C.span()

span() == ((d[i] * s[i]) + p[i]) * ... 

