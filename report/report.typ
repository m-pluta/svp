#set align(center)
= Balancing Act: Achieving Time and Memory Efficiency in SVP 
\
Michal
\
Word Count: 750

#set align(left)

== Approach

My methodology was heavily influenced by my initial research into three of the most up-to-date methods of solving SVP:

#set align(center)
#figure(
  table(
  columns: (auto, auto, auto),
  inset: (
    x: 10pt,
    y: 5pt,
  ),
  align: horizon,
  [*Algorithm type*], [*Time complexity*], [*Space complexity*],
  [Enumeration], [$n^O(n)$], [$O(n^2)$],
  [Sieving], [$O(2^n)$], [$2^O(n)$],
  [Voronoi], [$O(2^(2n))$], [$O(2^n)$],

),
  caption: [Time & Space complexities of varying types of Lattice-based algorithms],
)
#set align(left)

I considered all the options and while Sieving was conceptually the most intuitive, I found that Enumeration would be ideal for this task, due to its favourable time complexity.

Asymptotically, the time complexity of Enumeration is much worse than Sieving or Voronoi, however, empirical evidence suggests that for low-dimensions Enumeration outperforms them. Additionally, Enumeration has polynomial space complexity - much better than Sieving and Voronoi. 

Although, Voronoi is a very cool way to solve this problem.

\

Lacking prior experience in C, I initially focused on getting a proof-of-concept working in Python. 

I found there to be many basis-reduction algorithms, and while BKZ is most-commonly used, I struggled to implement it and instead implemented LLL.

\
== Accuracy

A big worry of this assignment were floating point inaccuracies. Until testing, I didn't have an idea of how big of an issue they would be, hence I chose to use C's built-in `double`.

Arbitrarily, I set an accuracy threshold of $T=5 dot 10^(-5)$.

This meant that if

$ |"Expected result" - "Actual result"| <= T $

then I would consider my result as correct.

Experimentally, I found this to be a better metric than percentage difference as it ensured correct results were closely aligned with the actual answer, and were unaffected by the result's magnitude.

For testing, I generated lattices using `latticegen` from the fplll library @fplll_2023. I treated this as my source of truth, since it is commonly referred to as the best lattice-based solver available. I made multiple bash & python scripts to automate this process, and focused my testing on uniform and knapsack-like lattices.

#pagebreak()

The reference I found most useful offered me a deeper understanding of the underlying theory and algorithms, and provided me with pseudocode for both:

*LLL* @Yasuda_Masaya_2021

#figure(
  image("LLL_basic.png", width: 80%),
  caption: [
    *Lenstra-Lenstra-Lovász* Basis-Reduction Algorithm pseudocode
  ],
)

... and *Schnorr-Euchner Enumeration* @Yasuda_Masaya_2021

#figure(
  image("schnorr-euchner.png", width: 80%),
  caption: [
    Basic *Schnorr-Euchner* enumeration algorithm pseudocode
  ],
)

#pagebreak()

Once I implemented both algorithms, I began testing different configurations, while varying $delta$.

#figure(
  image("algorithms_comparison.png", width: 120%),
  caption: [
    Accuracy vs. Dimension for various algorithm configurations
  ],
)

This highlighted that:

1. LLL and SE on their own had unsatisfactory accuracy
  - LLL simply gave approximations from its reduced basis which weren't always correct.
  - SE struggled with knapsack-like lattices (due to accumulating floating-point inaccuracies)
2. Increasing $delta$ led to a higher accuracy. This aligns with other sources @Landsman_Lord_Heckman_1998, and is due to a higher $delta$ yielding a better basis reductionm, which ultimately leads to a better approximation.
3. LLL and SE combined exhibit superior performance.


#pagebreak()

To investigate whether it was necessary to use `long double` instead of `double`, I tested my LLL+SE implementation with both.

#figure(
  image("max_absolute_difference_plot.png", width: 100%),
  caption: [
    Maximum Absolute Difference vs. Dimension when using double and long double in LLL+SE, $delta$=0.99 
  ],
)

Based on these findings, and considering the assignment's requirements, I concluded it was unnecessary as the accuracy stays within the tolerance $T$. Furthermore, I could not justify the extra memory and computation time needed for `long double` in this context.

#pagebreak()

== Time

Upon running `valgrind`'s `callgrind` and feeding the result into `kcachegrind`, it became evident where optimisations would be most beneficial.

#figure(
  image("kcachegrind.png", width: 100%),
  caption: [
    Snippet of function call summary provided by kcachegrind
  ],
)
\

My `schnorr_euchner`, `lll`, and `gram_schmidt` all relied on calculating millions of `inner_product`s.

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
  supplement: [Code sample],
  caption: [
    My implentation of the Euclidean Inner Product
  ]
)

The only optimisation that stood out was potentially parallelising using multiple threads. This however would only be effective on higher dimensions due to overhead.

#pagebreak()

I realised that by memoising/precalculating the inner products I could drastically reduce the number of calls to `inner_product`, thereby decreasing the number of calculations. 

#figure(
  image("time_dimension_mem.png", width: 90%),
  caption: [
    Effects of memoisation on median run-time of LLL + SE, $delta$=0.99
  ],
) <Figure6>

While the graph isn't perfect, it shows that memoising has a clear impact on time - this is amplified by the y-axis being logarithmic.

#pagebreak()

I was intrigued by the $delta$ parameter, and decided to investigate more.

#figure(
  image("time_dimension_delta.png", width: 100%),
  caption: [
    Effects of changing LLL's $delta$ value on median run-time of LLL + SE
  ],
) <Figure7>

What I found was that for both uniform and knapsack lattices, a higher delta resulted in less variance of run-time, evident by the points being less scattered in @Figure7. It is important to note that for dimension 15-30, the run-time did increase due to overheard from computing LLL.

#pagebreak()

== Memory

Another key aspect of optimisation was memory usage. For this, I used `valgrind` (tools: `massif` and `dhat`) which provided valuable insights that helped me address potential memory leaks and segmentation faults. The `memusage` tool was also useful as it gave me a distribution of memory block sizes, and from this, I could pinpoint inefficiencies in my data structures.

#figure(
  image("bytes_dimension_mem.png", width: 100%),
  caption: [
    Effects of memoisation on the number of bytes read from memory 
  ],
)

== Readability

The final aspect I chose to prioritise was readability.

== Overall Success

Overall, I believe my implementation is quite fast, and accurate to a high number of dimensions. In the future, I am planning to attempt this problem again with a Domain-Specific Language and with more advanced methods to improve upon the performance.

#bibliography("works.bib")


1. Struct to typedef
2. malloc optimisation



