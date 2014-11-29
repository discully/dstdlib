# dstdlib

An implementation of the C++ Standard Library.

Written as a learning exercise, this is unlikely to be a
good choice to use in your own code!

## Unit Testing

During initial development a custom unit testing class was used.
However this has recently become a limitation and the existing unit tests
are currently being migrated to Google Test (a worthwhile exercise since
several bugs have already been identified).

The directory 'test' contains the original, and currently more substantial
tests. Tests which have already been migrated to Google Test are considered
to be deprecated.

The directory 'gtest' contains the tests which have been migrated to 
Google Test. Running 'make test' will compile and run all tests.
You will need to have Google Test installed and in your library/include
paths.
