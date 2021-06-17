# Operating-System-Course

[中文文档](https://github.com/teamwong111/Operating-System-Course/blob/main/README-cn.md)

It is my term porject repo of Operating System Course(2021 spring) in Tongji University

---

## Contents
- [Introduction](#Introduction)
- [Structure](#Structure)
- [Running](#Running)
- [Defenders](#Defenders)
- [License](#License)

---

## Introduction
This repo contain the term project of Tongji University's Operating System Course in 2021 spring. It is a file management. The supported commands are as follows:

1. man
2. autotest
3. fformat
4. ls
5. cd
6. mkdir
7. fcreat
8. fdelete
9. fopen
10. fclose
11. fwrite
12. fread
13. flseek

---

## Structure
```bash
.
    buffer.cpp
    buffer.h
    disk.cpp
    disk.h
    inode.cpp
    inode.h
    LICENSE
    main.cpp
    super_block.cpp
    super_block.h
```

---

## Running
- OS：Windows10
- Compiler：g++ (i686-win32-dwarf-rev0, Built by MinGW-W64 project) 8.1.0
- Compile command：g++ *.cpp -o D:\\filemanager.exe -g -Wall -static-libgcc -fexec-charset=GBK -std=c++11

---

## Defenders
The repo is currently owned by https://github.com/teamwong111 maintain

If I have infringement or you have any questions, please contact me by email wungjz1@gmail.com

---

## License
[MIT](https://github.com/teamwong111/Operating-System-Course/blob/main/LICENSE)

---