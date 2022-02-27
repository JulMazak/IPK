# IPK project 1 

## AUTHOR

Julia Mazakova / xmazak02 / 231254 
February 2022

## SUMMARY

Simple HTTP server in C++ executable in Linux environment able to **receive** and **respond** to 3 types of GET requests:
1. hostname
2. load
3. cpu-name

## COMPILING
This project is compiled with `g++ -g -Wall -Wextra -Werror`. You can compile it by typing:

```
make
```

This will generate an executable **hinfosvc**.


If you want to remove created files after using this server simply type:

```
make clean
```

## Contents

This projects contains these files :
* myserver.cpp --> Implementation of web server in C++.
* Makefile --> For compiling .
* README.md --> Documentation.

## Usage

You can send GET requests using **curl**, **wget** or simply via web browser.

### Examples

#### CURL
```
./hinfosvc 12345 & curl http://localhost:12345/hostname
```
#### WGET
```
./hinfosvc 12345 & wget http://localhost:12345/cpu-name
```
#### WEB BROWSER
First type this into command line:
```
./hinfosvc 12345
```
Then type this into your web browser:
```
localhost:12345/load
```
## Sources
These sources were helpful while developing this project:

https://beej.us/guide/bgnet/html/
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
