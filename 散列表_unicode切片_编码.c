#include "散列表_unicode切片_编码.h"
#define 容量表表项 28
static uint32_t 容量质数表[容量表表项] = { //采用小于 2^n 的最大质数，构成容量质数表
    31, 61, 127,
    251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521,
    131071, 262139, 524287, 1048573, 2097143, 4194301, 8388593, 16777213,
    33554393, 67108859, 134217689, 268435399, 536870909, 1073741789, 2147483647, 4294967291
};
// 内部函数
散列表链表节点 * 创建散列表链表节点(unicode切片 键, 单词类型 值);
散列表链表节点 * 散列表_查询节点(散列表* 散列表指针, unicode切片 键);
void           散列表_节点链表_释放(散列表链表节点* 首节点指针);
void           散列表_不扩容插入节点(散列表* 散列表指针, 散列表链表节点 * 待插入节点指针);
void           散列表链表节点_释放(散列表链表节点 * 待释放节点);

/******************************************************************************
 * 创建一个 空散列表 ，初始化后返回 散列表结构体的指针
 * 期望容量：期望的最小容量，如果为0则使用容量表最小容量
 *****************************************************************************/
散列表 * 创建散列表(uint32_t 期望容量) {
    // 查找容量质数表，确定新散列表容量
    int 实际容量;
    int i=0; // 从头到尾遍历容量表，直到找到足够容纳“期望容量”的最小容量表质数
    while (容量质数表[i] < 期望容量) {
        i++;
    }
    实际容量 = 容量质数表[i];
    // 申请堆内存 散列表对象、指针数组对象
    散列表* 散列表指针 = (散列表*)malloc(sizeof(散列表));
    // 初始化散列表
    散列表指针->用量 = 0;
    散列表指针->容量 = 实际容量;
    散列表指针->首节点指针数组 = (散列表链表节点**)malloc(实际容量*sizeof(散列表链表节点*));
    for (int i=0; i<实际容量; i++) {// 初始化 数据区，节点指针数组全部置为 空 NULL
        散列表指针->首节点指针数组[i] = NULL;
    }
    return 散列表指针;
}
// 创建 散列表链表节点
散列表链表节点 * 创建散列表链表节点(unicode切片 键, 单词类型 值) {
    // 构造新节点
    散列表链表节点 * 新节点指针 = (散列表链表节点*)malloc(sizeof(散列表链表节点));
    新节点指针->键 = 键; //初始化数据域
    新节点指针->值 = 值;
    新节点指针->下一节点指针 = NULL;
    return 新节点指针;
}
/******************************************************************************
 * 释放整个散列表，包括节点数组和链表节点，以及节点下游相关数据区
 * 散列表指针：被释放的 散列表结构体的指针
 *****************************************************************************/
void 散列表_释放(散列表* 散列表指针) {
    // 遍历节点指针数组，释放所有链表
    for (int i=0; i<(散列表指针->容量); i++) {
        if (散列表指针->首节点指针数组[i] != NULL) { //数组中的节点指针不为空，就释放
            散列表_节点链表_释放(散列表指针->首节点指针数组[i]); //释放键同散列值 链表
        }
    }
    free(散列表指针->首节点指针数组); //释放 首节点指针数组
    free(散列表指针); // 释放 散列表结构体
}
// 提供指向首节点的指针，释放整个单向链表的所有节点
void 散列表_节点链表_释放(散列表链表节点* 首节点指针) {
    散列表链表节点* 当前节点指针=首节点指针;
    散列表链表节点* 上一节点指针;
    // 遍历单向链表，释放上一节点
    while (当前节点指针->下一节点指针 != NULL) {
        上一节点指针 = 当前节点指针;
        当前节点指针 = 当前节点指针->下一节点指针;
        // 释放上一节点
        unicode切片_清空(&(上一节点指针->键)); // 先清空 切片 数据区
        free(上一节点指针); //再释放节点
    }
    // 释放尾节点
    unicode切片_清空(&(当前节点指针->键)); // 先清空 切片 数据区
    free(当前节点指针);
} 
/******************************************************************************
 * 查询一个“键”，返回对应的“键-值对”节点地址。失败则返回 NULL
 * 散列表指针：目标散列表结构体的指针
 * 键：查询的 键
 *****************************************************************************/
散列表链表节点 * 散列表_查询节点(散列表* 散列表指针, unicode切片 键) {
    // 计算‘键’的散列值
    uint32_t 键散列值 = unicode切片_计算散列值(键, 散列表指针->容量);
    // 查找 对应节点
    if (散列表指针->首节点指针数组[键散列值] == NULL) {
        return NULL;  //数组内容为 NULL，不存在改键
    } else {
        //有散列值，可能存在改键，在链表中进一步查找
        散列表链表节点 * 当前节点指针;
        散列表链表节点 * 上一节点指针;
        当前节点指针 = 散列表指针->首节点指针数组[键散列值]; //逐个查找链表节点
        while(当前节点指针->下一节点指针 != NULL){
            上一节点指针 = 当前节点指针;
            当前节点指针 = 当前节点指针->下一节点指针;
            if (unicode切片_判等(上一节点指针->键, 键)) { //比对 上一节点
                return 上一节点指针; //查询成功，返回对应值
            }
        }
        if (unicode切片_判等(当前节点指针->键, 键)) { //比对 当前节点 尾节点
                return 当前节点指针; //查询成功，返回对应值
        } else { // 尾节点也比对失败，查询失败
            return NULL;
        }
    }
}
/******************************************************************************
 * 查询一个“键”，返回对应的“值”。如果失败返回 -1
 * 散列表指针：目标散列表结构体的指针
 * 键：查询的 键
 *****************************************************************************/
int 散列表_查询(散列表* 散列表指针, unicode切片 键) {
    散列表链表节点 * 目标节点指针;
    目标节点指针 = 散列表_查询节点(散列表指针, 键);
    if (目标节点指针 == NULL) {
        return -1;
    } else {
        return 目标节点指针->值;
    }
}
/******************************************************************************
 * 插入一个 键-值 对，成功返回0。如果该“键”已存在则发生错误返回-1
 * 键: 映射的 键
 * 值：映射 值
 *****************************************************************************/
bool 散列表_插入(散列表* 散列表指针, unicode切片 键, 单词类型 值) {
    // 检查该 键 是否已存在
    散列表链表节点 * 目标节点指针 = 散列表_查询节点(散列表指针, 键);
    if (目标节点指针 != NULL) { // 键 已存在，失败
        return false;
    }
    // 该键不存在，可以插入新 键-值 对
    // 检查扩容，大于最大装载因子触发扩容
    if (装载因子(散列表指针) >= 散列表_最大装载因子) {
        散列表_扩容(散列表指针, 散列表指针->容量 + 1);
    }
    // 构建 新键值对 节点
    散列表链表节点 * 新节点指针 = 创建散列表链表节点(键, 值);
    // 查询并插入新节点
    uint32_t 键散列值 = unicode切片_计算散列值(键, 散列表指针->容量);
    if (散列表指针->首节点指针数组[键散列值] == NULL) { //键 不存在，作为唯一节点插入
        散列表指针->首节点指针数组[键散列值] = 新节点指针;
    } else { //散列碰撞，作为新首节点插入
        新节点指针->下一节点指针 = 散列表指针->首节点指针数组[键散列值];
        散列表指针->首节点指针数组[键散列值] = 新节点指针;
    }
    // 成功，返回 true
    散列表指针->用量 ++;
    return true;
}
/******************************************************************************
 * 删除一个 键-值 对，如果没有该“键”则失败并返回-1
 * 键: 需要删除的 键
 *****************************************************************************/
bool 散列表_删除(散列表 * 散列表指针, unicode切片 键) { // 暂时不自动缩容
    // 计算‘键’的散列值
    uint32_t 键散列值 = unicode切片_计算散列值(键, 散列表指针->容量);
    // 查找 对应节点
    if (散列表指针->首节点指针数组[键散列值] == NULL) {
        return false;  // 键 不存在
    } else { // 散列碰撞，链表线性搜索
        散列表链表节点 * 上一节点指针 = NULL;
        散列表链表节点 * 当前节点指针;
        当前节点指针 = 散列表指针->首节点指针数组[键散列值]; //初始化为首节点
        // 尝试查找第一个匹配节点
        while(!unicode切片_判等(当前节点指针->键, 键) && 当前节点指针->下一节点指针 != NULL){
            上一节点指针 = 当前节点指针;
            当前节点指针 = 当前节点指针->下一节点指针;
        }
        // 删除节点
        if (unicode切片_判等(当前节点指针->键, 键)) { // 找到
            if (上一节点指针 == NULL) { // 首节点，没有上游节点
                散列表指针->首节点指针数组[键散列值] = 当前节点指针->下一节点指针;
                散列表链表节点_释放(当前节点指针);
            } else { // 非首节点，动用上一节点指针
                上一节点指针->下一节点指针 = 当前节点指针->下一节点指针;
                散列表链表节点_释放(当前节点指针);
            }
            散列表指针->用量 --;
            return true;
        } else { // 没找到
            return false;
        }
    }
}
// 释放拉链链表指定节点，这里不会维护链表结构
void 散列表链表节点_释放(散列表链表节点 * 待释放节点) {
    // 先释放 切片 下游 堆数组
    unicode切片_清空(&(待释放节点->键));
    // 再释放 键值对节点
    free(待释放节点);
}
/******************************************************************************
 * 将一个键对应的值更改为 新值，如果没有该键则失败并返回-1
 * 键: 待更改的 键
 * 值：新的对应 值
 *****************************************************************************/
bool 散列表_更改(散列表* 散列表指针, unicode切片 键, 单词类型 新值) {
    // 查找目标节点
    散列表链表节点 * 目标节点指针 = 散列表_查询节点(散列表指针, 键);
    // 检查该 “键” 是否存在
    if (目标节点指针 == NULL) { // “键” 不存在
        return false;
    } else { // “键” 存在，改为新值
        目标节点指针->值 = 新值; 
        return true;
    }
}
/******************************************************************************
 * 扩容散列表到至少可以容纳 期望容量，返回新散列表地址
 * 期望容量：期望容量表扩容后的最小容量，为0则自动扩容到容量表下一级容量（接近翻倍）
 *****************************************************************************/
void 散列表_扩容(散列表* 散列表指针, uint32_t 期望容量) {
    // 查找容量质数表，确定 新散列表 容量
    int 新容量;
    if (期望容量 == 0) { 期望容量 = 散列表指针->容量 + 1; } //如果为0，则自动扩容到容量表下一项
    int i=0; // 从头到尾遍历容量表，直到找到足够容纳“期望容量”的最小质数
    while (容量质数表[i] < 期望容量) {
        i++;
    }
    新容量 = 容量质数表[i];
    // 创建一个新的临时散列表
    散列表 * 临时散列表指针 = 创建散列表(新容量);
    // 遍历旧散列表首节点指针数组，将各节点 移入"临时散列表"
    散列表链表节点 * 当前节点指针;
    散列表链表节点 * 上一节点指针;
    for (int i=0; i<(散列表指针->容量); i++) {
        if (散列表指针->首节点指针数组[i] != NULL) { //处理非空表项
            // 遍历单向链表，将其插入新散列表
            当前节点指针 = 散列表指针->首节点指针数组[i];
            while(当前节点指针->下一节点指针 != NULL) {
                上一节点指针 = 当前节点指针;
                当前节点指针 = 当前节点指针->下一节点指针;
                散列表_不扩容插入节点(临时散列表指针, 上一节点指针);
            }
            散列表_不扩容插入节点(临时散列表指针, 当前节点指针);
        }
    }
    // 将数据从 临时散列表 移回 本散列表
    散列表指针->用量 = 临时散列表指针->用量;
    散列表指针->容量 = 临时散列表指针->容量;
    free(散列表指针->首节点指针数组); // 释放 旧 首节点指针数组
    散列表指针->首节点指针数组 = 临时散列表指针->首节点指针数组;
    free(临时散列表指针);
}
// 用于散列表变容后的‘链表节点’重新插入，因此假定散列表剩余空间充足，‘不需要扩容’
void 散列表_不扩容插入节点(散列表* 散列表指针, 散列表链表节点 * 待插入节点指针) {
    待插入节点指针->下一节点指针 = NULL; // 将 下一节点指针 置空，防止干扰
    // 计算 散列值
    uint32_t 键散列值 = unicode切片_计算散列值(待插入节点指针->键, 散列表指针->容量);
    // 插入节点检查 链表是否存在
    if (散列表指针->首节点指针数组[键散列值] == NULL) { //首节点不存在，作为首节点插入
        散列表指针->首节点指针数组[键散列值] = 待插入节点指针;
    } else { //首节点存在，作为新首节点插入
        待插入节点指针->下一节点指针 = 散列表指针->首节点指针数组[键散列值]; //待插入节点指向原头部节点
        散列表指针->首节点指针数组[键散列值] = 待插入节点指针; //散列表项 指向待插入节点
    }
    // 更新用量
    散列表指针->用量 ++;
}
/******************************************************************************
 * 序列化输出 {3 31 [0:["k":v "k":v "k":v ] 3:["k":v "k":v]]}
 * 期望容量：期望容量表扩容后的最小容量。如果为0则自动扩容到容量表下一容量
 *****************************************************************************/
void 散列表_文件序列化打印(散列表 散列表, FILE * 输出文件) {
    // 先打印结构体信息 {3 31 [
    fprintf(输出文件, "{%d %d [", 散列表.用量, 散列表.容量);
    // 再打印散列表信息，只打印非空元素及后链表
    散列表链表节点 * 当前节点指针;
    散列表链表节点 * 上一节点指针;
    for (int i=0; i<(散列表.容量); i++) {
        if (散列表.首节点指针数组[i] != NULL) {
            // 遍历单向链表，逐个打印 {3 31 [0:["
            fprintf(输出文件, "%d:[", i); 
            当前节点指针 = 散列表.首节点指针数组[i];
            while(当前节点指针->下一节点指针 != NULL){
                上一节点指针 = 当前节点指针;
                当前节点指针 = 当前节点指针->下一节点指针;
                // 打印上一节点 "{3 31 哈希 [0:[k:v "
                fputc('"', 输出文件);
                unicode切片_文件打印(上一节点指针->键, 输出文件);
                fprintf(输出文件, "\":%d ", 上一节点指针->值);
            }
            // 打印当前接节点 "{3 31 哈希 [0:[k:v k:v k:v] "
            fputc('"', 输出文件);
            unicode切片_文件打印(当前节点指针->键, 输出文件);
            fprintf(输出文件, "\":%d] ", 当前节点指针->值);
        }
    }
    if (散列表.用量 > 0) {
        fputc('\b', 输出文件); //抹去最后的空格
    }
    fprintf(输出文件, "]}");
}