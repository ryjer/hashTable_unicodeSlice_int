#ifndef  散列表_UNICODE切片_编码_H
#define  散列表_UNICODE切片_编码_H
    #include "../unicode切片/unicode切片.h"
    #include "../compiler/compiler.h"
    #include <stdint.h>
    #include <stdbool.h>

    // 散列表内部参数定义
    #define 散列表_unicode切片_编码_装载因子
    #define 装载因子(x) ((float)((x)->用量)/((x)->容量))
    #define 散列表_最大装载因子 (0.75)
    #define 散列表_最小装载因子 (0.1)
    #define 散列表_链表_最大长度 (8)

    // 单向链表节点：使用拉链法，发生散列碰撞时延伸链表
    typedef struct 散列表链表节点{
        struct 散列表链表节点 *下一节点指针;
        单词类型               值;
        unicode切片           键;
    } 散列表链表节点;
    
    //拉链法散列表，最大容量2^32个元素，直接数组中存储 散列链表节点的数组
    typedef struct 散列表{
        uint32_t        用量;
        uint32_t        容量;
        散列表链表节点 **首节点指针数组;
    } 散列表;
    
    散列表 *创建散列表(uint32_t 期望容量); //√已测试
    void   散列表_释放(散列表* 散列表指针); //√已测试
    // 切片键

    int    散列表_查询_切片键(散列表* 散列表指针, unicode切片 键); //√已测试
    bool   散列表_插入_切片键(散列表* 散列表指针, unicode切片 键, 单词类型 值); //√已测试
    bool   散列表_删除_切片键(散列表* 散列表指针, unicode切片 键); //√已测试
    bool   散列表_更改_切片键(散列表* 散列表指针, unicode切片 键, 单词类型 值); //√已测试

    void   散列表_扩容(散列表* 旧散列表指针, uint32_t 期望容量); //√已测试
    
    void 散列表_文件序列化打印(散列表 散列表, FILE * 输出文件); //√已测试
#endif