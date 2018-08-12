# Two-phase Fast lossy Gaussian Filter
These code demonstrate a two-phase Gaussian filter with only integer calculation and high memory locality usage or less cache miss and only penalty is some lossy edges.


Note: in main.cpp it will need opencv to demonstrate the image, or you can just get the gaussian_filter.h and gaussian_filter.cpp


It implements 3 Gaussian filter as following

(1) Floating point kernel 

(2) Discrete integer kernel without division, using right-shift bits to replace division to gain performance

(3) 2-phase, edges-lossy, integer kernel, it is super fast, less cache-miss.




===== The result: (in my core-i7 6700HQ) =====

(1) Running floating point gaussian kernel 7x7

 Totally taking 26.82000 secs for 500 image(s) processing, average : 0.05364 sec

(2) Running integer-division-shift gaussian kernel 7x7

 Totally taking 17.54200 secs for 500 image(s) processing, average : 0.03508 sec

(3) Running Integer Fast gaussian kernel 7x7

 Totally taking 2.70800 secs for 500 image(s) processing, average : 0.00542 sec

== Algorithms comparsion ==

Algorithm (2) is 1.5-times faster than (1)

Algorithm (3) is 6.5-times faster than (2)

Algorithm (3) is 9.9-times faster than (1)
