# Makig
make git ignore file by command line

this is super simple program that will generate gitignore file for you.

gitignores are available in : github.com/github/gitignore/

# Use

it's very simple , just open program in command line and your destination folder , then select your option and BOOM!!

program will get giignore file from stated repo with curl, if ignore file exist , it will ask question to rewrite file.

and DONE, you got your ignore file

# Building
dependencies :
- libcurl

build :
```
git clone https://github.com/parsa011/makig
cd makig
make
```
