[![Build Status](https://drone.io/github.com/yekm/bench/status.png)](https://drone.io/github.com/yekm/bench/latest)

### Description
Simple benchmarking program with fancy graphs. It measures the time which was
taken to process some (varying) amount of data by the algorithm. Then it just
plots the relationship between time and amount of processed data.

This project is written for (self)educational purposes and fun. Some things
may look a bit awkward.

### Sample graphs
http://yekm.name/bench/d3

### Adding algorithms for benchmarking
Adding algorithms for benchmarking is easy (at least I've tried to make it easy).

Decide what data your algorithm process and derive from GenericData<T> (or use
already made RandomData and others).

Derive from Task and implement prepare_data(size_t n). Here you create and return
your data of size n. For example:
```
return std::make_shared<common::RandomData<int>>(n);
```

By default amount of data will be doubled on each run of algorithm. Testing will
be stopped then execution time exeeds 60 sec (by default) or amount of data
exceeds memory capacity (std::bad_alloc thrown). Timeout can be changed by
command line arguments. Data growth can be changed by reimplementing virtual
function get_n().

Derive from Algorithm and implement do_run(). Here you cast passed TaskData
to GenericData<T> which you pick earlier and do some processing. Running time
of this function is measured. For example:
```
std::vector<int> &d = static_cast<GenericData<std::vector<int>>&>(td).get_mutable();
std::sort(d.begin(), d.end());
```

Write a cpp file in which make a static struct. In constructor you should create
Task and all Algorithms. Add Algorithms to Task and add Task to TaskCollection.
Since your struct is static and TaskCollection is a singleton all algorithms and tasks
will be created and registered in TaskCollection automatically at program launch.
And thus there is no need to make any changes in present code. Just create a subfolder
in tasks/, place you code there and re-run cmake.

Quiet messy description. You should look at actual code, it is simple <s>and clear</s>.

### Some useful classes
`TaskCollection` - singleton. Keeps tasks together.

`Task` - keeps set of algorithms which perform one particular
task (sorting, counting, searching, etc). Knows how to generate data,
and validate results.

`Algorithm` - gets TaskData, processes it.

`TaskData` - basic class for data. Interface for cloning.

`GenericData<T>` - holds actual data. Can return constant or mutable reference to it.
If algorithm is not modifying it should ask for const data. In that case next algorithm
will get just the same data. Otherwise modified data discarded and new data
generated, which should be exactly the same as previous (it can depend on actual
implementation).
