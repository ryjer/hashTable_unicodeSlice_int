#include "散列表_unicode切片_编码.h"
#include <stdio.h>

// gcc -g -Wall test.c 散列表_unicode切片_编码.c ../切片_unicode切片/切片_unicode切片.c ../unicode切片/unicode切片.c ../utf8_unicode/utf8_unicode.c && ./a.out
// 测试用例
int main() {
    //配置 输入输出 文件指针
    FILE * 输入文件 = fopen("test.txt", "r");
    FILE * 输出文件 = stdout;
    // 配置切片
    unicode切片 切片;
    unicode切片 键;
    fprintf(输出文件, "######## 创建散列表 ########\n");
    散列表 * 散列表指针 = 创建散列表(0);
    散列表_文件序列化打印(*散列表指针, 输出文件);
    fputc('\n', 输出文件);
    int 值;
    // 初始化切片 切片数组
    unicode切片_初始化(&切片, 8);
    unicode切片_初始化(&键, 8);
// 插入
    fprintf(输出文件, "######## 插入 ########\n一行一对 键-值 对\n");
    // utf8文件读取
    unicode字符 字符; // 读取utf8编码
    do{
        字符 = utf8转unicode(文件_读取utf8字符(输入文件));
        unicode切片_追加(&切片, 字符);
        // 制表 分隔符检查，生成 键
        if (字符 == '\t' || 字符==' ') {
            切片.长度--;
            键 = unicode切片_复制(切片);
            unicode切片_重置(&切片);
        }
        // 行结束检查，生成 值
        if (字符==EOF || 字符=='\n') {
            if (字符=='\n') 切片.长度--;
            值 = unicode切片_转整数(切片, 10);
            unicode切片_重置(&切片);
            // 插入 键值对
            散列表_插入(散列表指针, 键, 值);
        }
    }while(字符!=EOF);
// 序列化查看
    fprintf(输出文件, "######## 序列化文件打印 ########\n");
    fprintf(输出文件, "装载因子=%f\n", 装载因子(散列表指针));
    散列表_文件序列化打印(*散列表指针, 输出文件);
    fputc('\n', 输出文件);
// 查询
    fprintf(输出文件, "######## 查询 ########\n");
    fprintf(输出文件, "\"%s\": %d\n", "身高", 散列表_查询(散列表指针, 原始utf8字符串转unicode切片("身高")));
    fprintf(输出文件, "\"%s\": %d\n", "年龄", 散列表_查询(散列表指针, 原始utf8字符串转unicode切片("年龄")));
    fprintf(输出文件, "\"%s\": %d\n", "abc",  散列表_查询(散列表指针, 原始utf8字符串转unicode切片("abc")));
// 更改
    fprintf(输出文件, "######## 更改 ########\n");
    char * 待更改键字符串; 
    unicode切片 待更改键切片;
    待更改键字符串 = "身高";
    待更改键切片 =  原始utf8字符串转unicode切片(待更改键字符串);
    fprintf(输出文件, "原键值: \"%s\": %d\n", 待更改键字符串, 散列表_查询(散列表指针, 待更改键切片));
    散列表_更改(散列表指针, 待更改键切片, 80);
    fprintf(输出文件, "新键值: \"%s\": %d\n", "身高", 散列表_查询(散列表指针, 原始utf8字符串转unicode切片("身高")));
// 删除
    fprintf(输出文件, "######## 删除 ########\n");
    char *      待删除键字符串;
    unicode切片 待删除键切片;
    // 删除 孤立首节点
    待删除键字符串 = "时长";
    待删除键切片 = 原始utf8字符串转unicode切片(待删除键字符串);
    fprintf(输出文件, "-键: %s\n", 待删除键字符串);
    散列表_删除(散列表指针, 待删除键切片);
    散列表_文件序列化打印(*散列表指针, 输出文件);
    fputc('\n', 输出文件);
    // 删除 中间节点
    待删除键字符串 = "甲乙丙";
    待删除键切片 = 原始utf8字符串转unicode切片(待删除键字符串);
    fprintf(输出文件, "-键: %s\n", 待删除键字符串);
    散列表_删除(散列表指针, 待删除键切片);
    散列表_文件序列化打印(*散列表指针, 输出文件);
    fputc('\n', 输出文件);
    // 删除 尾节点
    待删除键字符串 = "jkun";
    待删除键切片 = 原始utf8字符串转unicode切片(待删除键字符串);
    fprintf(输出文件, "-键: %s\n", 待删除键字符串);
    散列表_删除(散列表指针, 待删除键切片);
    散列表_文件序列化打印(*散列表指针, 输出文件);
    fputc('\n', 输出文件);
// 释放
    fprintf(输出文件, "######## 释放散列表 ########\n");
// 散列表_文件序列化打印(*散列表指针, 输出文件);
    fputc('\n', 输出文件);
    散列表_释放(散列表指针);
    // 注意，下面正常应该会因为 散列表释放发生错误
    // 散列表_文件序列化打印(*散列表指针, 输出文件);
}