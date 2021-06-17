# Operating-System-Course
这是我在同济大学操作系统课程（2021春）的大作业

---

## 目录
- [简介](#简介)
- [项目目录结构](#项目目录结构)
- [运行](#运行)
- [维护者](#维护者)
- [License](#License)

---

## 简介
本仓库为包含同济大学2021年春操作系统课程的大作业，其支持的命令如下：
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

## 项目目录结构
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

## 运行
- 操作系统：Windows10
- 编译器：g++ (i686-win32-dwarf-rev0, Built by MinGW-W64 project) 8.1.0
- 编译命令：g++ *.cpp -o D:\\filemanager.exe -g -Wall -static-libgcc -fexec-charset=GBK -std=c++11

---

## 维护者
该仓库目前的维护者为https://github.com/teamwong111

如有侵权或其他问题请通过邮件联系wungjz1@gmail.com

---

## License
[MIT](https://github.com/teamwong111/Operating-System-Course/blob/main/LICENSE)

---