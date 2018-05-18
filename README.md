# haoml
haoml stands for hao markup language

## What is haoml
haoml is a kind of markup language like [toml](https://github.com/skystrife/cpptoml), but more easier.  

## Feature
* all value type are string
* parser and generator

## Example
```
#this is the declare of this file

#format is: name|age|score
[students]
frank|20|100
evan|21|90
jake|20|88
lucy|19|95

#format is: name=age
[teacher]
#teacher li is 30 years old
li = 30
#wang
 wang=40

[reserved]
```

## Elements
a haoml file is consist of several blocks which are delimited by a new line('\n'), each block 
is one of the three types described below. 

### comment
only full-line comment is supported

### mapp
a key-value pair 

### arrayy
strings delimited by '|'

## How to use this lib
just copy `haoml.cpp` and `haoml.h` to your project, include `haoml.h` in your source code, and add
target in your `Makefile` or any other files that can compile.

there are some examples in [test.cpp](test.cpp)

## Documentation
haoml documentation is generated using [Doxygen](http://www.doxygen.org).

## License
See the [LICENSE](LICENSE) file for details.
