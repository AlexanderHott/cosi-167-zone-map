# COSI-167A: Advanced Data Systems - ZoneMaps [CPP]

# Questions

- [ ] are range query bounds inclusive or exclusive?

## About

TemplateZonemaps is a simple template for you, the student, to use for the simple implementation project
in COSI-167A. Note that this is a high-level template, it is not extensive, rather it is meant to help
guide you on what we expect when you implement the zone maps. You can use this as a base code or start from
scratch. You may require additional helper functions to satisfy the requirements of the project.

## Zone Map Header
The zonemaps.h header file is a template that defines the basic components of a zone map data structure.
It is required to implement the data structure using this template in a separate file named
"zonemaps.cpp". You are free to modify certain components of the template to improve the performance of the
data structure; however, the core functionality should remain the same. In your implementation, it is
important to comment your code to highlight various design decisions and logic. 

## Testing 
The main.cpp file provides a foundational testing framework expected for this project. Each `workloads/W*` directory 
contains three files (1) `workload.dat` for inserts and (2) `point_queries.txt.` and (3) `range_queries.txt`.
You are required to run the workloads, given in the `workloads` directory (change the `W[number]`), build the zone map, and benchmark point queries and range queries. We have already provided a simple logic for benchmarking the point query. You are required to
implement parsing the range queries from 'range_queries.txt` and benchmarking the range query
performance. The execution times are to be written either into a log file or displayed on the terminal
along with other relevant details about the workload, such as the input workload size, the number of
queries.

Compile all files using the "make" command.
Once the implementation of zone maps is complete, you will be required to fill in the appropriate test
call in the main.cpp file for the point/range queries. 

## Contact

> If you have any questions please feel free to see Subhadeep during office hours, or
email your queries at subhadeep@brandeis.edu.
