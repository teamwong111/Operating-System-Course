#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include "buffer.h"
#include "disk.h"
#include "inode.h"
#include "super_block.h"
using namespace std;

Buffer_manager buffer_manager;
Inode_manager inode_manager;
Sb_manager sb_manager;
Disk_manager disk_manager;

map<string, int> cmd_switch = {
    {"man", 1}, {"autotest", 2}, {"fformat", 3}, {"exit", 4}, {"ls", 5}, {"cd", 6}, {"mkdir", 7}, {"fcreat", 8}, {"fdelete", 9}, {"fopen", 10}, {"fclose", 11}, {"fwrite", 12}, {"fread", 13}, {"flseek", 14}};

void man(string command)
{
    static string man =
        "全部命令如下:\n"
        "man : 手册\n"
        "autotest : 自动测试\n"
        "fformat : 格式化\n"
        "exit : 正确退出\n"
        "ls : 列出目录及文件\n"
        "cd : 改变目录\n"
        "mkdir : 新建目录\n"
        "fcreat : 新建文件\n"
        "fdelete : 删除文件\n"
        "fopen : 打开文件\n"
        "fclose : 关闭文件\n"
        "fwrite : 写入文件\n"
        "fread : 读取文件\n"
        "flseek : 移动读写指针\n"
        "示例 : man mkdir, 即查看mkdir的手册\n";

    static string autotest =
        "autotest : 自动测试\n"
        "示例 : autotest, 即开始自动测试\n";

    static string fformat =
        "fformat : 格式化\n"
        "示例 : fformat, 即格式化\n";

    static string exit =
        "exit : 退出\n"
        "示例 : exit, 即退出\n";

    static string ls =
        "ls : 列出目录及文件\n"
        "示例 : ls, 即列出目录及文件\n";

    static string cd =
        "cd : 改变目录\n"
        "示例 : cd dir1, 即跳转到dir1, 不支持/dir1/dir2这种模式";

    static string mkdir =
        "mkdir : 新建目录\n"
        "示例 : mkdir dir1, 即建立目录dir1, 不支持/dir1/dir2这种模式\n";

    static string fcreat =
        "fcreat : 新建文件\n"
        "示例 : fcreat file1, 即建立file1文件, 不支持/dir1/file1这种模式";

    static string delet =
        "fdelete : 删除文件\n"
        "示例 : fdelete file1, 即删除file1文件, 不支持/dir1/file1这种模式";

    static string fopen =
        "fopen : 打开文件\n"
        "示例 : fopen file1, 即打开file1文件, 不支持/dir1/file1这种模式";

    static string fclose =
        "fclose : 关闭文件\n"
        "示例 : fclose file1, 即关闭file1文件, 不支持/dir1/file1这种模式";

    static string fwrite =
        "fwrite : 写入文件\n"
        "示例 : fwrite file1 D:\\1.txt 800, 即从D:\\1.txt中读入800个字节到file1";

    static string fread =
        "fread : 读取文件\n"
        "示例1 : fread file1 -o D:\\2.txt 800, 即读出file1的800个字节写入D:\\2.txt文件\n"
        "示例2 : fread file1 -f nofile 800, 即读出file1的800个字节到命令行\n";

    static string flseek =
        "flseek : 移动读写指针\n"
        "示例 : flseek file1 0 0, 即调整file1文件读写指针到文件开头, 参数含义除file名外与fseek一致";

    static unordered_map<string, string *> cmd_map({{"man", &man},
                                                    {"autotest", &autotest},
                                                    {"fformat", &fformat},
                                                    {"exit", &exit},
                                                    {"ls", &ls},
                                                    {"cd", &cd},
                                                    {"mkdir", &mkdir},
                                                    {"fcreat", &fcreat},
                                                    {"fdelete", &delet},
                                                    {"fopen", &fopen},
                                                    {"fclose", &fclose},
                                                    {"fwrite", &fwrite},
                                                    {"fread", &fread},
                                                    {"flseek", &flseek}});

    auto it = cmd_map.find(command);
    if (it == cmd_map.end())
    {
        cout << "没有此命令" << endl;
        return;
    }
    cout << *it->second;
}

int auto_test(string &cmd)
{
    static int test_no = 0;
    static vector<string> test_cmds = {
        "mkdir bin",
        "mkdir etc",
        "mkdir home",
        "mkdir dev",
        "fcreat test_del",
        "ls",
        "fdelete test_del",
        "ls",
        "mkdir test",
        "cd test",
        "fcreat jerry",
        "fopen jerry -rw",
        "fwrite jerry D:\\1.txt 800",
        "flseek jerry 500 0",
        "fread jerry -o D:\\1_out.txt 500",
        "fclose jerry",
        "cd ..",
        "cd home",
        "mkdir texts",
        "mkdir reports",
        "mkdir photos",
        "ls",
        "cd texts",
        "fcreat rea",
        "fopen rea -rw",
        "fwrite rea D:\\readme.txt 1018",
        "flseek rea 0 0",
        "fread rea -o D:\\2_out.txt 1018",
        "fclose rea",
        "cd ..",
        "cd reports",
        "fcreat rep",
        "fopen rep -rw",
        "fwrite rep D:\\1.pdf 267265",
        "flseek rep 0 0",
        "fread rep -o D:\\1_out.pdf 267265",
        "fclose rep",
        "cd ..",
        "cd photos",
        "fcreat cat",
        "fopen cat -rw",
        "fwrite cat D:\\1.jpg 26258",
        "flseek cat 0 0",
        "fread cat -o D:\\1_out.jpg 26258",
        "fclose cat"};
    cmd = test_cmds[test_no % test_cmds.size()];
    return ++test_no % (test_cmds.size() + 1);
}

void cmd_to_args(const string &cmd, vector<string> &args)
{
    args.clear();
    for (unsigned int p = 0, q = 0; q < cmd.length(); p = q + 1)
    {
        q = cmd.find_first_of(" \n", p);
        string str = cmd.substr(p, q - p);
        if (!str.empty())
        {
            args.push_back(str);
        }
        if (q == string::npos)
            return;
    }
}

int main()
{
    if (!disk_manager.is_exist())
    {
        sb_manager.format_disk();
    }
    else
    {
        sb_manager.get_superblock();
        inode_manager.get_inode();
    }
    man("man");
    bool at_flag = false;
    int case_num;
    string cmd = "", arg0, arg1, arg2, arg3, arg4;
    vector<string> args;
    while (1)
    {
        cout << inode_manager.get_current_path() << " ]$ ";
        if (at_flag)
        {
            if (auto_test(cmd))
            {
                cout << cmd;
                getchar();
            }
            else
            {
                cout << "自动测试结束" << endl;
                at_flag = false;
                cmd = "";
            }
        }
        else
        {
            getline(cin, cmd);
        }
        if (cmd == "")
        {
            continue;
        }
        cmd_to_args(cmd, args);
        arg0 = args.size() > 0 ? args[0] : "";
        arg1 = args.size() > 1 ? args[1] : "";
        arg2 = args.size() > 2 ? args[2] : "";
        arg3 = args.size() > 3 ? args[3] : "";
        arg4 = args.size() > 4 ? args[4] : "";
        case_num = cmd_switch[arg0];
        switch (case_num)
        {
        case 1:
            man(arg1);
            break;
        case 2:
            at_flag = true;
            break;
        case 3:
            sb_manager.format_disk();
            exit(0);
            break;
        case 4:
            sb_manager.save_superblock();
            inode_manager.save_inode();
            buffer_manager.save_buffer();
            exit(0);
            break;
        case 5:
            inode_manager.ls();
            break;
        case 6:
            inode_manager.cd(arg1);
            break;
        case 7:
            inode_manager.mkdir(arg1);
            break;
        case 8:
            inode_manager.create(arg1);
            break;
        case 9:
            inode_manager.delete_file(arg1);
            break;
        case 10:
            inode_manager.open_file(arg1, arg2);
            break;
        case 11:
            inode_manager.close_file(arg1);
            break;
        case 12:
            inode_manager.write_file(arg1, arg2, arg3);
            break;
        case 13:
            inode_manager.read_file(arg1, arg2, arg3, arg4);
            break;
        case 14:
            inode_manager.seek_file(arg1, arg2, arg3);
            break;
        default:
            cout << "没有此命令" << endl;
            break;
        }
    }
}
