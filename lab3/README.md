# CSVParser

## Description

Class CSVParser implements reading a csv-like file stream (i.e. a file or input stream)
and allows to stream it out row by row by using TuplePrinter class.

## Usage

1. Run `make` in main directory
2. Go to *test* folder and run `./test` to check everything works fine.
3. Include `CSVParser.h` header and use `CSVParser` class

## Features

1. Lazy loading from a file
2. Customizable line-, row-delimiter and escape characters
3. Arbitrary number of rows/columns in csv file
4. Error handling
5. Simple interface
