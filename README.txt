This Program Simulates a Cache.
Programmed by: Jacob Worden
CS3339 HW3 Spring 2022 TXSTATE

This program is limited as it cannot simulate all 
cache types. In this cache each cache block contains
only 1 word and the memory references are specified
as a word address.

In order to run this Program:
1) compile the program: g++ -0 main main.cpp
    * Be sure the code is downloaded to your pc
2) run the program: ./main 4 2 memory_reference_file.txt
    * This program takes command line arguments as follows:
    * ./main num_entries associativity memory_reference_file
3) Check the output file "cache_sim_output" for the results

If you want to experiement with this code you can change the
command line inputs. This code is limited in scope and will
only output what would happen, it does not directly simulate 
an actually cache.

Furthurmore this code is subject to change in the future. At
the time of submission I feel the code is incomplete and there
are many things I would have liked to impliment or change to 
be more accurate and expand in scope to simulate all types of
caches.

Current Bug: Valid Bit is not working as expected, resulting in
HITS that are incorrect