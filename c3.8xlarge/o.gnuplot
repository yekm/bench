# Task: popcnt perfomance

set logscale xy 2
set ylabel 'Time (sec)'
set xlabel 'N (number of elements)'
set terminal svg size 700,500 fsize 9
set output 'task0_alg0.svg'
set title '32bit SWAR popcnt'
plot 'task0_alg0.data' using 1:2 with lines title '32bit SWAR popcnt', \
	'task0_alg0.data' using 1:2:3 with errorbars notitle

set output 'task0_alg1.svg'
set title 'Brian Kernighan popcnt'
plot 'task0_alg1.data' using 1:2 with lines title 'Brian Kernighan popcnt', \
	'task0_alg1.data' using 1:2:3 with errorbars notitle

set output 'task0_alg2.svg'
set title 'Simple popcnt'
plot 'task0_alg2.data' using 1:2 with lines title 'Simple popcnt', \
	'task0_alg2.data' using 1:2:3 with errorbars notitle

set output 'task0_alg3.svg'
set title 'generalized 32 bit SWAR popcnt'
plot 'task0_alg3.data' using 1:2 with lines title 'generalized 32 bit SWAR popcnt', \
	'task0_alg3.data' using 1:2:3 with errorbars notitle

set output 'task0_alg4.svg'
set title 'generalized 64 bit SWAR popcnt'
plot 'task0_alg4.data' using 1:2 with lines title 'generalized 64 bit SWAR popcnt', \
	'task0_alg4.data' using 1:2:3 with errorbars notitle

set output 'task0_alg5.svg'
set title 'intrinsics _mm_popcnt_u64 manual asm popcnt'
plot 'task0_alg5.data' using 1:2 with lines title 'intrinsics _mm_popcnt_u64 manual asm popcnt', \
	'task0_alg5.data' using 1:2:3 with errorbars notitle

set output 'task0_alg6.svg'
set title 'intrinsics _mm_popcnt_u64 popcnt'
plot 'task0_alg6.data' using 1:2 with lines title 'intrinsics _mm_popcnt_u64 popcnt', \
	'task0_alg6.data' using 1:2:3 with errorbars notitle

set output 'task0_alg7.svg'
set title 'intrinsics _mm_popcnt_u64 unrolled popcnt'
plot 'task0_alg7.data' using 1:2 with lines title 'intrinsics _mm_popcnt_u64 unrolled popcnt', \
	'task0_alg7.data' using 1:2:3 with errorbars notitle

set output 'task0_alg8.svg'
set title 'table lookup popcnt'
plot 'task0_alg8.data' using 1:2 with lines title 'table lookup popcnt', \
	'task0_alg8.data' using 1:2:3 with errorbars notitle

set terminal svg size 1000,800 fsize 8
set output 'allinone_task0.svg'
set title 'all in one'
plot  'task0_alg0.data' using 1:2 with lines title '32bit SWAR popcnt', \
	'task0_alg0.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg1.data' using 1:2 with lines title 'Brian Kernighan popcnt', \
	'task0_alg1.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg2.data' using 1:2 with lines title 'Simple popcnt', \
	'task0_alg2.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg3.data' using 1:2 with lines title 'generalized 32 bit SWAR popcnt', \
	'task0_alg3.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg4.data' using 1:2 with lines title 'generalized 64 bit SWAR popcnt', \
	'task0_alg4.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg5.data' using 1:2 with lines title 'intrinsics _mm_popcnt_u64 manual asm popcnt', \
	'task0_alg5.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg6.data' using 1:2 with lines title 'intrinsics _mm_popcnt_u64 popcnt', \
	'task0_alg6.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg7.data' using 1:2 with lines title 'intrinsics _mm_popcnt_u64 unrolled popcnt', \
	'task0_alg7.data' using 1:2:3 with errorbars notitle, \
	 'task0_alg8.data' using 1:2 with lines title 'table lookup popcnt', \
	'task0_alg8.data' using 1:2:3 with errorbars notitle


# Task: sorting algorithms

set logscale xy 2
set ylabel 'Time (sec)'
set xlabel 'N (number of elements)'
set terminal svg size 700,500 fsize 9
fit a*x**2+b 'task1_alg0.data' using 1:2:3 via a, b
set output 'task1_alg0.svg'
set title 'Insertion sort n^2'
plot 'task1_alg0.data' using 1:2 with lines title 'Insertion sort n^2', \
	'task1_alg0.data' using 1:2:3 with errorbars notitle, \
	a*x**2+b title sprintf("a*x**2+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task1_alg1.data' using 1:2:3 via a, b
set output 'task1_alg1.svg'
set title 'Introsort std::sort n*log(n)'
plot 'task1_alg1.data' using 1:2 with lines title 'Introsort std::sort n*log(n)', \
	'task1_alg1.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task1_alg2.data' using 1:2:3 via a, b
set output 'task1_alg2.svg'
set title 'Merge sort n*log(n)'
plot 'task1_alg2.data' using 1:2 with lines title 'Merge sort n*log(n)', \
	'task1_alg2.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x**2+b 'task1_alg3.data' using 1:2:3 via a, b
set output 'task1_alg3.svg'
set title 'Selection sort n^2'
plot 'task1_alg3.data' using 1:2 with lines title 'Selection sort n^2', \
	'task1_alg3.data' using 1:2:3 with errorbars notitle, \
	a*x**2+b title sprintf("a*x**2+b\na=%g\nb=%g", a, b)

fit a*x*(log(x))**2+b 'task1_alg4.data' using 1:2:3 via a, b
set output 'task1_alg4.svg'
set title 'Shell sort n*log^2(n)'
plot 'task1_alg4.data' using 1:2 with lines title 'Shell sort n*log^2(n)', \
	'task1_alg4.data' using 1:2:3 with errorbars notitle, \
	a*x*(log(x))**2+b title sprintf("a*x*(log(x))**2+b\na=%g\nb=%g", a, b)

fit a*x*x+b 'task1_alg5.data' using 1:2:3 via a, b
set output 'task1_alg5.svg'
set title 'swenson binary insertion sort n^2'
plot 'task1_alg5.data' using 1:2 with lines title 'swenson binary insertion sort n^2', \
	'task1_alg5.data' using 1:2:3 with errorbars notitle, \
	a*x*x+b title sprintf("a*x*x+b\na=%g\nb=%g", a, b)

set output 'task1_alg6.svg'
set title 'swenson grail sort'
plot 'task1_alg6.data' using 1:2 with lines title 'swenson grail sort', \
	'task1_alg6.data' using 1:2:3 with errorbars notitle

fit a*x*log(x)+b 'task1_alg7.data' using 1:2:3 via a, b
set output 'task1_alg7.svg'
set title 'swenson heapsort n*log(n)'
plot 'task1_alg7.data' using 1:2 with lines title 'swenson heapsort n*log(n)', \
	'task1_alg7.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task1_alg8.data' using 1:2:3 via a, b
set output 'task1_alg8.svg'
set title 'swenson mergesort n*log(n)'
plot 'task1_alg8.data' using 1:2 with lines title 'swenson mergesort n*log(n)', \
	'task1_alg8.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task1_alg9.data' using 1:2:3 via a, b
set output 'task1_alg9.svg'
set title 'swenson quiksort n*log(n)'
plot 'task1_alg9.data' using 1:2 with lines title 'swenson quiksort n*log(n)', \
	'task1_alg9.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x*x+b 'task1_alg10.data' using 1:2:3 via a, b
set output 'task1_alg10.svg'
set title 'swenson selection sort n^2'
plot 'task1_alg10.data' using 1:2 with lines title 'swenson selection sort n^2', \
	'task1_alg10.data' using 1:2:3 with errorbars notitle, \
	a*x*x+b title sprintf("a*x*x+b\na=%g\nb=%g", a, b)

fit a*x*(log(x))**2+b 'task1_alg11.data' using 1:2:3 via a, b
set output 'task1_alg11.svg'
set title 'swenson sell sort n*log^2(n)'
plot 'task1_alg11.data' using 1:2 with lines title 'swenson sell sort n*log^2(n)', \
	'task1_alg11.data' using 1:2:3 with errorbars notitle, \
	a*x*(log(x))**2+b title sprintf("a*x*(log(x))**2+b\na=%g\nb=%g", a, b)

set output 'task1_alg12.svg'
set title 'swenson sqrt sort'
plot 'task1_alg12.data' using 1:2 with lines title 'swenson sqrt sort', \
	'task1_alg12.data' using 1:2:3 with errorbars notitle

fit a*x*log(x)+b 'task1_alg13.data' using 1:2:3 via a, b
set output 'task1_alg13.svg'
set title 'swenson timsort n*log(n)'
plot 'task1_alg13.data' using 1:2 with lines title 'swenson timsort n*log(n)', \
	'task1_alg13.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

set terminal svg size 1000,800 fsize 8
set output 'allinone_task1.svg'
set title 'all in one'
plot  'task1_alg0.data' using 1:2 with lines title 'Insertion sort n^2', \
	'task1_alg0.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg1.data' using 1:2 with lines title 'Introsort std::sort n*log(n)', \
	'task1_alg1.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg2.data' using 1:2 with lines title 'Merge sort n*log(n)', \
	'task1_alg2.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg3.data' using 1:2 with lines title 'Selection sort n^2', \
	'task1_alg3.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg4.data' using 1:2 with lines title 'Shell sort n*log^2(n)', \
	'task1_alg4.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg5.data' using 1:2 with lines title 'swenson binary insertion sort n^2', \
	'task1_alg5.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg6.data' using 1:2 with lines title 'swenson grail sort', \
	'task1_alg6.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg7.data' using 1:2 with lines title 'swenson heapsort n*log(n)', \
	'task1_alg7.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg8.data' using 1:2 with lines title 'swenson mergesort n*log(n)', \
	'task1_alg8.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg9.data' using 1:2 with lines title 'swenson quiksort n*log(n)', \
	'task1_alg9.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg10.data' using 1:2 with lines title 'swenson selection sort n^2', \
	'task1_alg10.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg11.data' using 1:2 with lines title 'swenson sell sort n*log^2(n)', \
	'task1_alg11.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg12.data' using 1:2 with lines title 'swenson sqrt sort', \
	'task1_alg12.data' using 1:2:3 with errorbars notitle, \
	 'task1_alg13.data' using 1:2 with lines title 'swenson timsort n*log(n)', \
	'task1_alg13.data' using 1:2:3 with errorbars notitle


# Task: Sorting algorithms, partially sorted data, 1000000 elements

set logscale xy 2
set ylabel 'Time (sec)'
set xlabel 'number of shuffles'
set terminal svg size 700,500 fsize 9
fit a*x**2+b 'task2_alg0.data' using 1:2:3 via a, b
set output 'task2_alg0.svg'
set title 'Insertion sort n^2'
plot 'task2_alg0.data' using 1:2 with lines title 'Insertion sort n^2', \
	'task2_alg0.data' using 1:2:3 with errorbars notitle, \
	a*x**2+b title sprintf("a*x**2+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task2_alg1.data' using 1:2:3 via a, b
set output 'task2_alg1.svg'
set title 'Introsort std::sort n*log(n)'
plot 'task2_alg1.data' using 1:2 with lines title 'Introsort std::sort n*log(n)', \
	'task2_alg1.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task2_alg2.data' using 1:2:3 via a, b
set output 'task2_alg2.svg'
set title 'Merge sort n*log(n)'
plot 'task2_alg2.data' using 1:2 with lines title 'Merge sort n*log(n)', \
	'task2_alg2.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

fit a*x*(log(x))**2+b 'task2_alg3.data' using 1:2:3 via a, b
set output 'task2_alg3.svg'
set title 'Shell sort n*log^2(n)'
plot 'task2_alg3.data' using 1:2 with lines title 'Shell sort n*log^2(n)', \
	'task2_alg3.data' using 1:2:3 with errorbars notitle, \
	a*x*(log(x))**2+b title sprintf("a*x*(log(x))**2+b\na=%g\nb=%g", a, b)

fit a*x*log(x)+b 'task2_alg4.data' using 1:2:3 via a, b
set output 'task2_alg4.svg'
set title 'swenson timsort n*log(n)'
plot 'task2_alg4.data' using 1:2 with lines title 'swenson timsort n*log(n)', \
	'task2_alg4.data' using 1:2:3 with errorbars notitle, \
	a*x*log(x)+b title sprintf("a*x*log(x)+b\na=%g\nb=%g", a, b)

set terminal svg size 1000,800 fsize 8
set output 'allinone_task2.svg'
set title 'all in one'
plot  'task2_alg0.data' using 1:2 with lines title 'Insertion sort n^2', \
	'task2_alg0.data' using 1:2:3 with errorbars notitle, \
	 'task2_alg1.data' using 1:2 with lines title 'Introsort std::sort n*log(n)', \
	'task2_alg1.data' using 1:2:3 with errorbars notitle, \
	 'task2_alg2.data' using 1:2 with lines title 'Merge sort n*log(n)', \
	'task2_alg2.data' using 1:2:3 with errorbars notitle, \
	 'task2_alg3.data' using 1:2 with lines title 'Shell sort n*log^2(n)', \
	'task2_alg3.data' using 1:2:3 with errorbars notitle, \
	 'task2_alg4.data' using 1:2 with lines title 'swenson timsort n*log(n)', \
	'task2_alg4.data' using 1:2:3 with errorbars notitle


# Task: Flase sharing

unset logscale
set ylabel 'Time (sec)'
set xlabel 'N (number of elements)'
set terminal svg size 700,500 fsize 9
set output 'task3_alg0.svg'
set title 'false sharing, default alignment'
plot 'task3_alg0.data' using 1:2 with lines title 'false sharing, default alignment', \
	'task3_alg0.data' using 1:2:3 with errorbars notitle

set output 'task3_alg1.svg'
set title 'fixed false sharing, 64 bytes alignment'
plot 'task3_alg1.data' using 1:2 with lines title 'fixed false sharing, 64 bytes alignment', \
	'task3_alg1.data' using 1:2:3 with errorbars notitle

set terminal svg size 1000,800 fsize 8
set output 'allinone_task3.svg'
set title 'all in one'
plot  'task3_alg0.data' using 1:2 with lines title 'false sharing, default alignment', \
	'task3_alg0.data' using 1:2:3 with errorbars notitle, \
	 'task3_alg1.data' using 1:2 with lines title 'fixed false sharing, 64 bytes alignment', \
	'task3_alg1.data' using 1:2:3 with errorbars notitle


# Task: 1000000 vector lengths

set logscale x 2
set ylabel 'Time (sec)'
set xlabel 'vector length'
set terminal svg size 700,500 fsize 9
set output 'task4_alg0.svg'
set title 'handmade unrolling'
plot 'task4_alg0.data' using 1:2 with lines title 'handmade unrolling', \
	'task4_alg0.data' using 1:2:3 with errorbars notitle

set output 'task4_alg1.svg'
set title 'loop unrolling'
plot 'task4_alg1.data' using 1:2 with lines title 'loop unrolling', \
	'task4_alg1.data' using 1:2:3 with errorbars notitle

set output 'task4_alg2.svg'
set title 'template unrolling'
plot 'task4_alg2.data' using 1:2 with lines title 'template unrolling', \
	'task4_alg2.data' using 1:2:3 with errorbars notitle

set terminal svg size 1000,800 fsize 8
set output 'allinone_task4.svg'
set title 'all in one'
plot  'task4_alg0.data' using 1:2 with lines title 'handmade unrolling', \
	'task4_alg0.data' using 1:2:3 with errorbars notitle, \
	 'task4_alg1.data' using 1:2 with lines title 'loop unrolling', \
	'task4_alg1.data' using 1:2:3 with errorbars notitle, \
	 'task4_alg2.data' using 1:2 with lines title 'template unrolling', \
	'task4_alg2.data' using 1:2:3 with errorbars notitle


