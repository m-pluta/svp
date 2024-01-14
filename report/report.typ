#set align(center)
= Balancing Act: Achieving Time and Memory Efficiency in SVP 
\
Michal
\
Word Count: 750

#set align(left)

== Approach

This task seemed quite daunting at first, however I very quickly became fully immersed in this challenge. My approach stemmed entirely from my initial research into the most common methods of solving SVP. The three main approaches

- *Enumeration*
- *Sieving*
- *Voronoi*

== Chosen Algorithm

While all of the above methods are valid ways of solving SVP they each had their own caveats. 
- Enumeration is 

Focus of exactness


All different combos


== Accuracy

Double vs Long Double

== Time & Memory

Memoisation

Delta

== Overall Success

#bibliography("works.bib")


#figure(
  image("algorithms_comparison.png", width: 120%),
  caption: [
    Accuracy vs. Dimension for various algorithm configurations
  ],
)

#figure(
  image("bytes_dimension_mem.png", width: 100%),
  caption: [
    Effects of memoisation on the number of bytes read from memory 
  ],
)

#figure(
  image("max_absolute_difference_plot.png", width: 100%),
  caption: [
    Maximum Absolute Difference vs. Dimension when using two different variable types. 
  ],
)

#figure(
  image("time_dimension_delta.png", width: 100%),
  caption: [
    Effects of changing LLL's delta value on median run-time of LLL + SE
  ],
)

#figure(
  image("time_dimension_mem.png", width: 100%),
  caption: [
    Effects of memoisation on median run-time of LLL + SE
  ],
)