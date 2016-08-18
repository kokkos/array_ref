///////////////////////////////////////////////////////////////////////////////
// Layout Left/Column Major/Fortran
///////////////////////////////////////////////////////////////////////////////

d := dims
i := index

index(0) = i[0] + index(1)
index(n) = d[n-1] * (i[n]+index(n+1))

index(0) = i[0] + index(1)
index(n) = d[n-1]*i[n] + d[n-1]*index(n+1)

index(0 -> 3) =                i[0]  
              +           d[0]*i[1]
              +      d[0]*d[1]*i[2] 
              + d[0]*d[1]*d[2]*i[3] 

d := dims
p := padding
s := striding
i := index

index(0) = s[0]*i[0] + index(1)
index(n) = (d[n-1]*s[n-1]+p[n-1]) * (s[n]*i[n]+index(n+1))

index(0) = s[0]*i[0] + index(1)
index(n) = (d[n-1]*s[n-1]+p[n-1])*s[n]*i[n]
         + (d[n-1]*s[n-1]+p[n-1])*index(n+1)

index(0 -> 3)
    =                                                          (s[0]*i[0])
    +                                       (d[0]*s[0]+p[0]) * (s[1]*i[1])
    +                    (d[0]*s[0]+p[0]) * (d[1]*s[1]+p[1]) * (s[2]*i[2])
    + (d[0]*s[0]+p[0]) * (d[1]*s[1]+p[1]) * (d[2]*s[2]+p[2]) * (s[3]*i[3])

(i  , j  ) - s[0] - (i+1, j  ) - s[0] - (n  , j  ) - p[0]

    |                   |                   |

  s[1]                s[1]                s[1]

    |                   |                   |

(i  , j+1) - s[0] - (i+1, j+1) - s[0] - (n  , j+1) - p[0]

    |                   |                   |

  s[1]                s[1]                s[1]

    |                   |                   |

(i  , m  ) - s[0] - (i+1, m  ) - s[0] - (n  , m  ) - p[0]

    |                   |                   |

  p[1]                p[1]                p[1]

///////////////////////////////////////////////////////////////////////////////
// Layout Right/Row Major/C++
///////////////////////////////////////////////////////////////////////////////

d := dims
r := rank
i := index

index(0) = i[r-1] + index(1)
index(n) = d[r-n] * (i[(r-1)-n]+index(n+1))

index(0) = i[r-1] + index(1)
index(n) = d[r-n]*i[(r-1)-n] + d[r-n]*index(n+1)

index(3) = d[4-3]*(i[(4-1)-3]+index(3+1)) = d[1]*(i[0]+index(4))
index(2) = d[4-2]*(i[(4-1)-2]+index(2+1)) = d[2]*(i[1]+index(3))
index(1) = d[4-1]*(i[(4-1)-1]+index(1+1)) = d[3]*(i[2]+index(2))
index(0) = i[4-1] + index(1)              = i[3] + index(1)

index(0 -> 3) = d[1]*d[2]*d[3]*i[0]
              +      d[2]*d[3]*i[1]
              +           d[3]*i[2]
              +                i[3]

d := dims
r := rank
p := padding
s := striding
i := index

0 = r-1
n-1 = r-n
n = (r-1)-n

index(0) = s[r-1]*i[r-1] + index(1)
index(n) = (d[r-n]*s[r-n]+p[r-n]) * (s[(r-1)-n]*i[(r-1)-n]+index(n+1))

index(0) = s[r-1]*i[r-1] + index(1)
index(n) = (d[r-n]*s[r-n]+p[r-n])*s[(r-1)-n]*i[(r-1)-n]
         + (d[r-n]*s[r-n]+p[r-n])*index(n+1)

index(3) = (d[4-3]*s[4-3]+p[4-3]) * (s[(4-1)-3]*i[(4-1)-3]+index(3+1))
         = (d[1]*s[1]+p[1]) * (s[0]*i[0]+index(4))
index(2) = (d[4-2]*s[4-2]+p[4-2]) * (s[(4-1)-2]*i[(4-1)-2]+index(2+1))
         = (d[2]*s[2]+p[2]) * (s[1]*i[1]+index(3))
index(1) = (d[4-1]*s[4-1]+p[4-1]) * (s[(4-1)-1]*i[(4-1)-1]+index(1+1))
         = (d[3]*s[3]+p[3]) * (s[2]*i[2]+index(2))
index(0) = s[4-1]*i[4-1] + index(1)
         = s[3]*i[3] + index(1)

index(0 -> 3)
    = (d[1]*s[1]+p[1]) * (d[2]*s[2]+p[2]) * (d[3]*s[3]+p[3]) * (s[0]*i[0])
    +                    (d[2]*s[2]+p[2]) * (d[3]*s[3]+p[3]) * (s[1]*i[1])
    +                                       (d[3]*s[3]+p[3]) * (s[2]*i[2])
    +                                                          (s[3]*i[3])

///////////////////////////////////////////////////////////////////////////////
// Incremental Indexer:
// * Method to build partial indices
// * Method to get d[] term for a particular index

// StridedIterator:
// * Strides in 1D
// * Used to implement subview
// * Is copying cheap enough?

// What exactly is the issue with iterator paradigms on a multi-dimension array?

// I don't think we actually /want/ a random access iterator (e.g. an iterator
// that can be advanced in any dimension). What we want is an forward iterator
// that moves through the data in a particular pattern. And on top of that, we
// also want to be able to access neighbors in a particular dimension - but
// just in one direction at a time.
//
// As long as we can do operator splitting, we only need to offset in one
// particular dimension at a time. On some platforms, this is desirable because
// our results indicate that a fully split operator (e.g. three inner loops,
// each applying a "1D" stencil to a different dimension) performs better than
// a fused loop. I believe Sam indicated that the facts here are flipped on
// Xeon Phi.
//
// The crucial thing here is that this dimensional random access is independent
// of how we're moving through memory. We (always?) want to move through memory
// in unit stride.

// One of the problems with an iterator for a multi-dimensional array is the
// attempt to abstract away the loop structure. This structure is actually
// crucial to get high-performance indexing, via incremental index construction.

// This is the fundamental loop structure of an explicit finite
// difference/finite volume operator.

auto const lenj = (d.upper[1]-d.lower[1]+1);
auto const leni = (d.upper[0]-d.lower[0]+1);

for (auto k = d.lower[2]; k <= d.upper[2]; ++k) {
    auto idx = lenj*leni*k;

    for (auto j = d.lower[1]; j <= d.upper[1]; ++j) {
        idx += leni*j;
 
        // i-direction 1D stencil
        #pragma simd
        for (auto i = d.lower[0]; i < d.upper[0]; ++i) {
            idx += i;
            auto const stride = 1;

            output[idx] = stencil1d(input, idx, stride);
        }

        // j-direction 1D stencil
        #pragma simd
        for (auto i = d.lower[0]; i < d.upper[0]; ++i) {
            idx += i;
            auto const stride = leni;

            output[idx] += stencil1d(input, idx, stride);
        }

        // k-direction 1D stencil
        #pragma simd
        for (auto i = d.lower[0]; i < d.upper[0]; ++i) {
            idx += i;
            auto const stride = lenj*leni;

            output[idx] += stencil1d(input, idx, stride);
        }
    }
}
