#import "@preview/tablex:0.0.8": tablex, rowspanx, colspanx

#set align(center)
= Balancing Act: Achieving Time and Memory Efficiency in SVP 
\
Michal
\
Word Count: 742

#set align(left)

== Approach

My methodology was heavily influenced by my initial research into three of the most known methods of solving SVP:

#set align(center)
#figure(
  table(
  columns: (auto, auto, auto),
  inset: (
    x: 10pt,
    y: 5pt,
  ),
  align: horizon,
  [*Algorithm type*], [*Time complexity*],  [*Space complexity*],
  [Enumeration],      [$n^O(n)$],           [$O(n^2)$],
  [Sieving],          [$O(2^n)$],           [$2^O(n)$],
  [Voronoi],          [$O(2^(2n))$],        [$O(2^n)$],

),
  caption: [Time & Space complexities of varying types of Lattice-based algorithms @Micciancio_2023],
) <Table1>
#set align(left)

While Sieving was conceptually the most intuitive, I found that Enumeration was ideal for this task.

Asymptotically, the time complexity of Enumeration is much worse than Sieving or Voronoi, however, empirical evidence suggests that for low-dimensions Enumeration outperforms them. Additionally, Enumeration has polynomial space complexity which is much better than Sieving and Voronoi. 


== Accuracy

A big worry of this assignment were floating point inaccuracies, hence I initially chose C's built-in `double`.

Arbitrarily, I set an accuracy threshold, $T$ where

$ T=5 dot 10^(-5) $

This meant that if

$ |"Expected result" - "Actual result"| <= T $

then I would consider my result as correct.

I found this to be a better metric than percentage difference as it ensured correct results were closely aligned with the actual answer, and unaffected by the result's magnitude.

I generated test lattices using `latticegen` from the `fplll` library @fplll_2023. It is often referred to as the best lattice-based solver available, hence I trusted its answers. I made multiple bash & python scripts to automate test generation, and focused on uniform & knapsack-like lattices.

#pagebreak()

Lacking prior experience in C, I decided to begin implementing a prototype in Python. 

I found many basis-reduction algorithms, and while BKZ is most commonly used, I struggled to implement it and instead implemented LLL @Bhattacherjee_Hernandez-Castro_Moyler_2023 @Galbraith_2018 @Cohen_1993.

Once I implemented LLL and Schnorr Euchner enumeration @Yasuda_Masaya_2021 according to pseudocode, I began testing different configurations, while varying $delta$.

#figure(
  image("algorithms_comparison.png", width: 120%),
  caption: [
    Accuracy vs. Dimension for various algorithm configurations
  ],
) <Fig1>

This highlighted that:

1. LLL and SE on their own had unsatisfactory accuracy
  - LLL gave approximations from its reduced basis which weren't always correct.
  - SE struggled with knapsack-like lattices (due to accumulating floating-point inaccuracies)

2. Increasing $delta$ led to a higher accuracy. This is because a higher $delta$ yields a better basis reduction @Landsman_Lord_Heckman_1998, therefore, a better approximation.

3. LLL and SE combined exhibit superior performance.

#pagebreak()

To determine whether `long double` was necessary, I tested my implementation using both.

#figure(
  image("max_absolute_difference_plot.png", width: 100%),
  caption: [
    Maximum Absolute Difference vs. Dimension when using double and long double in LLL+SE, $delta$=0.99 
  ],
) <Fig2>

Based on this, and considering the coursework's requirements, I concluded it was unnecessary as the accuracy stayed within the tolerance $T$. Furthermore, I could not justify the extra memory and computation time needed for `long double`.

#pagebreak()

== Time

Upon running `valgrind`'s `callgrind` and feeding the result into `kcachegrind`, it became evident where optimisations would be most beneficial.

#figure(
  image("kcachegrind.png", width: 80%),
  caption: [
    Snippet of function call summary provided by kcachegrind
  ],
) <Fig3>
\

My `schnorr_euchner`, `lll`, and `gram_schmidt` all relied on calculating millions of `inner_product`s.

\
#figure(
  ```c
  double inner_product(const Vector v1, const Vector v2, const int dim) {
      double total = 0;
      for (int i = 0; i < dim; i++) {
          total += v1[i] * v2[i];
      }
      return total;
  }
  ```,
  supplement: [Code Sample],
  caption: [
    My implentation of the Euclidean Inner Product
  ]
) <Code1>
\
The only optimisation here was potentially parallelising using multiple threads, which would only be effective on higher dimensions due to thread overhead.

#pagebreak()

I realised that by memoising/precalculating inner products I could drastically reduce the number of calls to `inner_product`, thereby decreasing the number of operations. 

#figure(
  image("time_dimension_mem.png", width: 90%),
  caption: [
    Effects of memoisation on median run-time of LLL + SE, $delta$=0.99
  ],
) <Fig4>

While the graph isn't perfect, it shows that memoising has a positive impact on performance - this is amplified by the logarithmic y-axis.

#pagebreak()

I decided to investigate the $delta$ parameter more:

#figure(
  image("time_dimension_delta.png", width: 100%),
  caption: [
    Effects of changing LLL's $delta$ value on median run-time of LLL + SE
  ],
) <Fig5>

I found that for both uniform and knapsack lattices, a higher delta resulted in less variance of run-time, evident by times being less scattered (@Fig5).

\

\

#figure(
  tablex(
    columns: 9,
    align: center + horizon,
    inset: (
      x: 12pt,
      y: 8pt,
    ),

    // indicate the first two rows are the header
    // (in case we need to eventually
    // enable repeating the header across pages)
    header-rows: 2,

    /* --- header --- */
    rowspanx(2)[*Delta*] , colspanx(8)[*Dimensions*]     , (),
    ()                      , [*1-5*], [*6-10*], [*11-15*], [*16-20*], [*21-25*], [*26-30*], [*31-35*], [*36-40*],
    /* -------------- */

    [*0.75*], [0.30], [#underline[0.33]], [#underline[0.42]], [#underline[0.59]], [#underline[0.98]], [#underline[2.37]], [9.89], [108.38],
    [*0.99*], [#underline[0.30]], [0.33], [0.47], [0.93], [1.98], [3.96], [#underline[7.92]], [#underline[22.39]],
  ),
  caption: [
    Median speed (in ms) of LLL+SE on each dimension range on *Uniform* Lattices
    \
    Faster delta value #underline[underlined]
  ],
) <Fig6>

\

#figure(
  tablex(
    columns: 9,
    align: center + horizon,
    inset: (
      x: 12pt,
      y: 8pt,
    ),

    // indicate the first two rows are the header
    // (in case we need to eventually
    // enable repeating the header across pages)
    header-rows: 2,

    /* --- header --- */
    rowspanx(2)[*Delta*] , colspanx(8)[*Dimensions*]     , (),
    ()                      , [*1-5*], [*6-10*], [*11-15*], [*16-20*], [*21-25*], [*26-30*], [*31-35*], [*36-40*],
    /* -------------- */

    [*0.75*], [0.30], [#underline[0.34]], [#underline[0.55]], [#underline[1.13]], [#underline[2.78]], [43.54], [227.18], [1330.40],
    [*0.99*], [#underline[0.30]], [0.36], [0.70], [1.92], [5.00], [#underline[11.42]], [#underline[26.48]], [#underline[102.61]],
  ),
  caption: [
    Median speed (in ms) of LLL+SE on each dimension range on *Knapsack* Lattices
    \
    Faster delta value #underline[underlined]
  ],
) <Fig7>

\

For dimensions 10-25/30 (@Fig6 and @Fig7), the run-time using $delta = 0.99$ increases due to more iterations inside LLL. However, asymptotically, $delta=0.99$ was better.

#pagebreak()

== Memory

For memory, I used `valgrind` (tools: `massif` and `dhat`) which helped me address potential memory leaks and segmentation faults.

The `memusage` tool was also useful as it gave me a distribution of memory block sizes, and from this, I could pinpoint inefficiencies in my data structures.

#figure(
  image("bytes_dimension_mem.png", width: 85%),
  caption: [
    Effects of memoisation on the number of bytes read from memory 
  ],
) <Fig8>

\
From @Fig8 it is clear memoisation leads to less memory reads. This is because, by memoising the inner products, the only value being read from memory is the inner product itself, and not the component vectors needed to compute the inner product.

#figure(
  image("total_bytes_dimension.png", width: 45%),
  caption: [
    Peak memory usage of LLL + SE, $delta = 0.99$ 
  ],
) <Fig9>

@Fig9 shows that

$ "Peak memory" prop ("Dimension")^2 $

which aligns with @Table1 and @Micciancio_2023

\

From @Fig3, it was also clear that `malloc` and `free` were called too often.
This was resolved by initialising `GS_Info` at the beginning and then reusing it throughout the program's execution.

#pagebreak()

== Readability

My initial implementation was centered around `struct`s, however these were unnecessary and made my code unreadable.

#figure(
  table(
  columns: (160pt, 160pt),
  stroke: none,
  inset: (
    x: 10pt,
    y: 5pt,
  ),
  align: left + top,
  [*Old implementation*], [*New implementation*],
  [
    ```c
    typedef struct {
        double *e;
    } Vector;
    ```],
  [
    ```c
    typedef double* Vector;
    ```
  ],
  [
    ```c
    typedef struct {
        Vector **v;
        int dimension;
    } Vector2D;
    ```
  ],
  [
    ```c
    typedef Vector* Matrix;
    ```
  ]
  ),
  caption: [
    Changes in implementation of `Vector` and `Vector2D`
  ],
  supplement: [Sample],
) <Code2>

\

#figure(
  table(
  columns: (auto),
  stroke: none,
  inset: (
    x: 10pt,
    y: 5pt,
  ),
  align: left + top,
  [*Old implementation*],
  [
    ```c
    for (int k = 0; k < i; k++) {
        double ip = inner_product(B->v[i], Bs->v[k], dim);
        mu->v[i]->e[k] = ip / inner_products[k];

        for (int j = 0; j < dim; j++) {
            Bs->v[i]->e[j] -= mu->v[i]->e[k] * Bs->v[k]->e[j];
        }
    }
    ```
  ],
  [*New implementation*],
  [
    ```c
    for (int k = 0; k < i; k++) {
        double ip = inner_product(B[i], Bs[k], dim);
        mu[i][k] = ip / inner_products[k];

        for (int j = 0; j < dim; j++) {
            Bs[i][j] -= mu[i][k] * Bs[k][j];
        }
    }
    ```
  ]
  
  ),
  caption: [
    Changes in implementation of projection calculation in Gram Schmidt
  ],
  supplement: [Sample],
) <Code3>

\

This led to much better readability, which @Code3 is a great example of.

\
\
\
\
\
\
\


== Conclusion

Overall, I believe my implementation is quite fast, and accurate to a high number of dimensions. I would like to try this challenge again with a Domain-Specific Language and with more advanced methods to improve upon the performance.

#pagebreak()

#bibliography(
  "works.bib",
  full: true
)
