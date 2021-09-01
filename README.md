# QDebug Saver to file (qt)


## Installing:
1. Or clone with submodule
```
git clone --recurse-submodules -j8 <this_repo>
```
2. Or run configure after cloning
```
./configure
```

## How to use:

1. To compile as static library
```
qmake LogSaver.pro
make
```

2. Include the complete code to your project
In the .pro file of your project add the following line (update the path to point to the correct location of QZXing src): 
```
include(../../LogSaver.pri)
```
