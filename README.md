[![Build Status](https://drone.io/github.com/yekm/bench/status.png)](https://drone.io/github.com/yekm/bench/latest)

### Description
Simple benchmarking program with fancy graphs. It measures the time which was
taken to process some (varying) amount of data by the algorithm. Then it just
plots the relationship between time and amount of processed data.

This project is written for (self)educational purposes and fun. Some things
may look a bit awkward.

### Usage
#### Compilation:

Dependencies: linux, C++ compiler with C++11 support (tested with g++ 4.8.4, clang 3.6.0),
CUDA (optional).

```
git clone https://github.com/yekm/bench
git submodule update --init
mkdir build
cd build
cmake ..
make
```
#### Running:
Make separate folder and run bench from where
```mkdir tmp && cd tmp```

Listing all tasks and algorithms
```
Task 0: popcnt perfomance
  32bit SWAR popcnt
  Brian Kernighan popcnt
  Simple popcnt
  generalized 32 bit SWAR popcnt
  generalized 64 bit SWAR popcnt
  intrinsics _mm_popcnt_u64 manual asm popcnt
  intrinsics _mm_popcnt_u64 popcnt
  intrinsics _mm_popcnt_u64 unrolled popcnt
  table lookup popcnt
  thrust popcnt
Task 1: sorting algorithms
  Insertion sort n^2
  Introsort std::sort n*log(n)
  Merge sort n*log(n)
  Selection sort n^2
  Shell sort n*log^2(n)
  swenson binary insertion sort n^2
  swenson grail sort
  swenson heapsort n*log(n)
  swenson mergesort n*log(n)
  swenson quiksort n*log(n)
  swenson selection sort n^2
  swenson sell sort n*log^2(n)
  swenson sqrt sort
  swenson timsort n*log(n)
  thrust::sort
Task 2: Sorting algorithms, partially sorted data, 1000000 elements
  Insertion sort n^2
  Introsort std::sort n*log(n)
  Merge sort n*log(n)
  Shell sort n*log^2(n)
  swenson timsort n*log(n)
  thrust::sort
Task 3: Flase sharing
  false sharing, default alignment
  fixed false sharing, 64 bytes alignment
Task 4: 1000000 vector lengths
  handmade unrolling
  loop unrolling
  template unrolling
```

Quick run
```
$ ../bench -t 0.1
```

Skip some tasks. However final listing will include this skipped tasks and old
measured perfomance data will be preserved between runs.
```
$ ../bench -t 1 -s 2
```

Each algorithm runs `a` (by default 3) times with same data to minimize the error.
It is possible to regenerate data and run algorithm `a` times again. The number
of iterations of data regeneration specified by `-b`
```
$ ../bench -t 1 -s 0,2 -a 2 -b 3
```

#### Viewing results:
For now it is quite uncomfortable. You need a web server and a couple of links.
And a modern web browser with js promises support.
```
$ for f in ../../html/d3/*; do ln -s $f $(basename $f); done
$ python3 -m http.server 8082
$ $BROWSER http://localhost:8082
```

### Adding algorithms for benchmarking
Adding algorithms for benchmarking is easy (at least I've tried to make it easy).

Run `./create_task.sh new_task_name_here` from `tasks/template` directory. Explore the new
`tasks/new_task_name_here` directory.

Decide what type of data your algorithm should process. Derive a class from `GenericData<T>`
or use `common::RandomData<T>` and others. `GenericData<T>` has two useful functions
`T & get_mutable()` and `const T & get_const()`.
If algorithm is not modifying it should ask for const data. In this case next algorithm
will get just the same data. Otherwise modified data discarded and new data
generated (which _should_ be exactly the same as previous, but it can depend on actual
implementation).

Derive from `Task` and implement `prepare_data(size_t n)`. Here you create and return
your data of size n. For example:
```
return std::make_shared<common::RandomData<int>>(n);
```

`RandomData<T>` derived from `GenericData<std::vector<T>>`, so `get_mutable()` will
return `std::vector<T>&`.

Amount of data will be doubled on each run of algorithm. Testing will
be stopped then execution time exeeds 60 sec (by default) or amount of data
exceeds memory capacity (std::bad_alloc thrown). Timeout can be changed by
command line arguments. Data growth can be changed by reimplementing virtual
function `Task::get_n()`.

Derive from `Algorithm` and implement `do_run(TaskData & td, std::unique_ptr<AResult> &)`.
Here you cast passed `TaskData` to `GenericData<T>` which you pick earlier and do some
processing. Running time of this function is measured. For example, in case of `RandomData<int>`:
```
std::vector<int> &d = static_cast<GenericData<std::vector<int>>&>(td).get_mutable();
std::sort(d.begin(), d.end());
```

Write a cpp file in which make a static struct.
In constructor you should create
Task and all Algorithms. Add Algorithms to Task and add Task to TaskCollection.
Since your struct is static and TaskCollection is a singleton all algorithms and tasks
will be created and registered in TaskCollection automatically at program launch.
And thus there is no need to make any changes in present code.

Quite messy description. You should look at actual code, it is simple <s>and clear</s>.

### Caveats
Aka don't try this at <s>home</s> work.

- static struct for initialisation/registering objects in collection (`tasks/*/*.cpp`)
- dumb/useless singletons (`taskcollection.hpp`)
- protected data members (`genericdata.hpp`)
- loose JSON handling (`utils/output/jsonoutput.cpp`)
- lack of comments

### LICENSE
MIT.
