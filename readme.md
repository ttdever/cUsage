
# cUsage

### This program is reading data from "/proc/stat" file and then printing it in %s to the console output.

## Running and testing:
### Run:
Use CC env variable to set a compiler (flags will be added automaticaly):
```bash
export CC=clang
#or
export CC=gcc
```

Then compile and execute the program:
```bash
make
./bin/cUsage
```
### Test:
To test the progarm run:
```bash
make test
```

### Program output and manual testing:
Here is example of loading 4 of 8 cores:
```
  Core Index |   %-Load
core #     0 |        7%
core #     1 |      100%
core #     2 |      100%
core #     3 |        0%
core #     4 |      100%
core #     5 |        0%
core #     6 |      100%
core #     7 |        0%
Avg %-Load =  50%
```

You can load cores of your processor with "stress" in linux:
```bash
sudo apt-get install stress
sudo stress --cpu <CORE_NUM> --timeout <SEC_TIMEOUT>
```


## Spinning frog:
<img src="https://media.tenor.com/tS4120QBuugAAAAC/frog-spin-frog.gif"  />


