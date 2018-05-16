# haoml
haoml is short for "hao markup language"

## what is haoml
haoml is a kind of markup language like [toml](https://github.com/skystrife/cpptoml), but more easier.  

## example
```conf
#this is the declare of this file

#format is: name|age|score
[students]
frank|20|100
evan|21|90
jake|20|88
lucy|19|95

#format is: name=age
[teacher]
#teacher li and he is 30 years old
li = 30
#wang
 wang=40

[reserved]
```

## elements
a haoml file is consist of several blocks which are delimited by a new line('\n'), echo block 
is one of the three types described below. 

### comment
only full-line comment is supported

### mapp
a key-value pair 

### arrayy
strings delimited by '|'




