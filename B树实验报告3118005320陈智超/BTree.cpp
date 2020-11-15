#include <stdio.h>
#include <malloc.h> 

#define M 4   //阶为4
#define  MAX  M-1
#define MIN M/2
#define N 14  //选取14个关键字的树作为例子
#define ERROR 0
#define SUCCESS 1
#define TRUE 1
#define UNSUCCESS 0
#define OVERFLOW -1
#define FALSE -1
typedef int Status;

typedef int KeyType;//关键字类型为整形

/*typedef struct {
} Record;    */



typedef struct BTNode {
    int keynum;                 //当前节点的关键字数目
    KeyType key[M + 1];         //关键字数组，key[0]未用
    struct BTNode *parent;      //双亲结点指针
    struct BTNode *ptr[M + 1];  //孩子节点指针数组
    //Record *recptr[M + 1];    //记录指针向量，0号单元未用
} BTNode, *BTree;               //B树的节点及指针类型

//B树查找的结果类型
typedef struct {
    int tag;                    //1：查找成功，0：查找失败
    BTree pt;                   //指向找到的结果类型
    int index;                  //1 <= index <= M  在节点中的关键字位序
} Result;


//接口定义

//操作结果：构造空的B树t
void InitBTree(BTree &t);

//操作结果：创建一棵B树
int MakeBTree(BTree &t, KeyType keyType[], int n);

//初始条件：树t存在
//操作结果：销毁B树t
Status DestroyBTree(BTree &t);

//初始条件：树t存在
//操作结果：返回树t的深度（不包括叶子结点层）
int Depth1(BTree t);

//初始条件：树t存在
//操作结果：返回t的深度（包括叶子结点层）
int Depth2(BTree t);


//初始条件：树t存在
//操作结果：在p节点上查找关键字的位置
int SearchIndex(BTree p, int k);


//初始条件：树t存在 
//操作结果：在M阶B树t上查找关键字k, 用r返回（pt， index， tag） 若是查找成功，则tag = 1， 指针pt指向的节点中第index个关键字等于k，否则 tag = 0， 若要插入关键字k， 则应该位于pt节点中第 index-1 个和第 index 个之间
void SearchBTree(BTree t, KeyType k, Result &r);

//初始条件：t是即将生成的新的根节点，x是新的根节点的关键字的值，
//p和ap是分裂开的两个节点，其双亲结点都要设为是t。
//操作结果：生成新的根节点 。

Status NewRoot(BTree &t, BTree p, int x, BTree ap);

//初始条件：q节点关键字数目等于m时，进行分裂，s是分裂的位置。
//操作结果：将q结点分裂成两个结点， 前一半保留在原来的节点，后一  半移入ap
Status Split(BTree &q, int s, BTree &ap);

//操作结果：是下面的插入函数的辅助函数，关键字x 和新节点指针 ap 分别插入q->key[index] 和 q->ptr[index]
void Insert(BTree &q, int index, int x, BTree ap);

//初始条件：树t存在，从SearchBTree中找到了插入节点q和插入位置index
//操作结果：在B树t中 q结点的key[index-1]和 key[index] 中插入关键k
void InsertBTree(BTree &t, int k, BTree q, int index);

//初始条件：树t存在
//操作结果：分解成根节点和若干个孩子节点，递归遍历B树
void Traverse(BTree t);

//初始条件：树t存在
//操作结果：求所有关键字个数sum
void KeysNum(BTree t, int &sum);

//初始条件：ptr节点存在
//操作结果：找到ptr的前驱节点，为删除操作做的准备
BTNode *FindPre(BTNode *ptr);

//初始条件：ptr节点存在
//操作结果：找到ptr的后继节点，为删除操作做的准备
BTNode *FindNext(BTNode *ptr);

//初始条件：key所造的t树中的节点不是最底层的非终端节点
// 操作结果：找到key的前驱关键字
void FindPreKey(BTree t, int key, Result &result);

//初始条件：key所造的t树中的节点不是最底层的非终端节点
//操作结果：找到key的后继关键字
void FindNextKey(BTree t, int key, Result &result);


//操作结果：右旋转（双亲结点借一个关键字给ptr，然后左兄弟给回一个最小的关键字给双亲结点）
void RightRotate(BTree &leftbro, BTree &ptr, BTree &parent, int pos);

//操作结果：左旋转（双亲结点借一个关键字给ptr，然后右兄弟给回一个最小的关键字给双亲结点）
void LeftRotate(BTree &rightbro, BTree &ptr, BTree &parent, int pos);

//初始条件：删除关键字后少于关键字少于MIN，兄弟节点也没有多余的节点
// 操作结果：左合并
void LeftMerge(BTree &leftbro, BTree &ptr, BTree &parent, int pos);

//初始条件：删除关键字后少于关键字少于MIN，兄弟节点也没有多余的节点
//操作结果：右合并
void RightMerge(BTree &ptr, BTree &rightbro, BTree &parent, int pos);


//初始条件：出现小于MIN的情况的调整函数
//操作结果：调整为符合B树的情况
BTree  Adjus(BTree &ptr);

//初始条件：删除的关键字位于最下层的非终端结点ptr，pos是关键字key在ptr中的位置
//操作结果：删除最下层的非终端结点中的关键字，被下面的Remove函数所调用
void DelMostBottomKey(BTree &ptr, int pos);

//初始条件：根为root的B树存在，e为要删除的关键字
//操作结果：从B树中删除e关键字
//从B树中删除e关键字
void ReMove(BTree &root, KeyType e);


//初始化一棵B树
void InitBTree(BTree &t) {
    t = NULL;
    printf("初始化成功\n");
}

//在p节点上查找关键字的位置
int SearchIndex(BTree p, int k) {
    int index = 1;
    while (index <= p->keynum && k > p->key[index]) {
        index++;
    }

    return index;
}

/**
 * 在M阶B树t上查找关键字k, 用r返回（pt， index， tag）
 * 若是查找成功，则tag = 1， 指针pt指向的节点中第i个关键字等于k
 * 否则 tag = 0， 若要插入关键字k， 则应该位于pt节点中第 index-1 个和第 index 个之间
 */
void SearchBTree(BTree t, KeyType k, Result &r) {
    int index = 0, found = 0;
    BTree p = t, q = NULL;      //初始， p指向根节点； p将用于指向待查节点， q指向其双亲结点
    while (p != NULL && 0 == found) {
        index = SearchIndex(p, k);
        if (index <= p->keynum && p->key[index] == k)
            found = 1;  //找到待查关键字
        else {
            q = p;
            p = p->ptr[index - 1];//指针下移
        }

    }

    if (1 == found) { //查找成功，返回k的位置 p 和 index
        r.pt = p;
        r.index = index;
        r.tag = 1;
    } else {        //查找不成功，返回k的插入位置q及index
        r.pt = q;
        r.index = index;
        r.tag = 0;
    }
}


//生成新的根节点
Status NewRoot(BTree &t, BTree p, int x, BTree ap) {
    t = (BTNode *) malloc(sizeof(BTNode));
    if (t == NULL)
        return ERROR;
    t->keynum = 1;
    t->ptr[0] = p;
    t->ptr[1] = ap;
    t->key[1] = x;
    if (p != NULL)
        p->parent = t;
    if (ap != NULL)
        ap->parent = t;
    t->parent = NULL;    //新根的双亲是空指针
    return SUCCESS;
}

//销毁B树
Status DestroyBTree(BTree &t) {
    if (NULL == t) return UNSUCCESS;
    for (int i = 0; i <= t->keynum; i++) {
        DestroyBTree(t->ptr[i]);
    }
    free(t);
    return SUCCESS;
}


//B树的插入
void Insert(BTree &q, int index, int x, BTree ap) {
    //关键字x 和新节点指针 ap 分别插入 q->key[index] 和 q->ptr[index]
    int j, n = q->keynum;
    for (j = n; j >= index; j--) {
        q->key[j + 1] = q->key[j];
        q->ptr[j + 1] = q->ptr[j];
    }
    q->key[index] = x;
    q->ptr[index] = ap;
    if (ap != NULL)
        ap->parent = q;

    q->keynum++;
}

//将q结点分裂成两个结点， 前一半保留在原来的节点，后一半移入ap所指向的节点
Status Split(BTree &q, int s, BTree &ap) {
  
    int i, j, n = q->keynum;
    ap = (BTNode *) malloc(sizeof(BTNode));         //生成新的节点
    if (ap == NULL)
        return ERROR;
    ap->ptr[0] = q->ptr[s];
    for (i = s + 1, j = 1; i <= n; i++, j++) {     //后一半移入ap所指向的节点
        ap->key[j] = q->key[i];
        ap->ptr[j] = q->ptr[i];
    }
    ap->keynum = n - s;
    ap->parent = q->parent;
    for (i = 0; i <= n - s; i++) {                 //修改新节点的parent域
        if (ap->ptr[i] != NULL)
            ap->ptr[i]->parent = ap;
    }

    q->keynum = s - 1;              // q节点的前一半保留，修改keynum
    return SUCCESS;
}

/**
  * 在B树t中 q结点的key[index-1] 和 key[index] 中插入 关键 k
  * 若是在插入后结点的关键字数目等于B树的阶M，就沿着双亲指针链进行分裂，使得t仍然是M阶B树
  */
void InsertBTree(BTree &t, int k, BTree q, int index) {
    int x, s, finished = 0, needNewRoot = 0;
    BTree ap;
    if (NULL == q) {
        NewRoot(t, NULL, k, NULL);  //生成新的根节点
    } else {
        x = k;
        ap = NULL;
        while (0 == needNewRoot && 0 == finished) {
            Insert(q, index, x, ap);    //关键字x 和新节点指针 ap 分别插入 q->key[index] 和 q->ptr[index]
            if (q->keynum < M) {
                finished = 1;           //插入完成
            } else {                    //分裂q结点
                s = (M + 1) / 2;
                Split(q, s, ap);
                x = q->key[s];
                if (q->parent != NULL) {
                    //在双亲结点中找到 x 的插入位置
                    q = q->parent;
                    index = SearchIndex(q, x);
                } else {
                    needNewRoot = 1;
                }
            }


        }//while
    }

    if (1 == needNewRoot)        //t是空树或者根节点已经分裂成q和 qp 结点
        NewRoot(t, q, x, ap);    //生成含信息（q，x， ap）的新的根节点

}

//创建一棵n个关键字的B树，存储在keyType数组中
int MakeBTree(BTree &t, KeyType keyType[], int n) {
    Result result;
    for (int i = 0; i < n; ++i) {
        SearchBTree(t, keyType[i], result);
        InsertBTree(t, keyType[i], result.pt, result.index);
    }
    printf("创建成功\n");
}

//遍历
void Traverse(BTree t) {
    if (NULL == t)
        return;

    //分解成根节点和若干个孩子节点
    Traverse(t->ptr[0]);    //第0个个孩子
    for (int i = 1; i <= t->keynum; ++i) {
        //递归遍历第i个关键字和第i个孩子
        printf("%d ", t->key[i]);
        Traverse(t->ptr[i]);
    }
}


//求所有关键字个数sum
void KeysNum(BTree t, int &sum) {
    if (NULL == t)
        return;
    KeysNum(t->ptr[0], sum);
    for (int i = 1; i <= t->keynum; ++i) {
        sum++;
        KeysNum(t->ptr[i], sum);
    }
}

//所有最下层非终端节点的关键字个数
void MostBottomNodeKeys(BTree t, int &bottomNodeKeys) {
    if (NULL == t)
        return;
    if (NULL == t->ptr[0])
        bottomNodeKeys = bottomNodeKeys + t->keynum;
    for (int i = 0; i <= t->keynum; ++i) {
        MostBottomNodeKeys(t->ptr[i], bottomNodeKeys);
    }
}

//B树的高度，不包括空的叶子结点
int Depth1(BTree t) {
    if (NULL == t)
        return 0;
    //子树+2
    return Depth1(t->ptr[0]) + 1;

}
//B树的高度，包括空层的叶子结点
int Depth2(BTree t) {
    return Depth1(t) + 1;
}


//找前驱节点
BTNode *FindPre(BTNode *ptr) {
    while (ptr != NULL && ptr->ptr[ptr->keynum] != NULL) {
        ptr = ptr->ptr[ptr->keynum];
    }
    return ptr;
}

//找后继节点
BTNode *FindNext(BTNode *ptr) {
    while (ptr != NULL && ptr->ptr[0] != NULL) {
        ptr = ptr->ptr[0];
    }
    return ptr;
}

//找前驱关键字
void FindPreKey(BTree t, int key, Result &result) {
    //找前驱节点
    SearchBTree(t, key, result);


    if (result.tag == 0) {
        printf("没有这个关键字\n");
        return;
    }
    BTNode *ptr = result.pt;
    int pos = result.index;

    if (NULL != result.pt->ptr[0]) {
        //关键字不是在最底层非终端节点，则有前驱节点
        BTNode *pre = FindPre(ptr->ptr[pos - 1]);
        printf("%d的前驱关键字是%d\n", key, pre->key[pre->keynum]);
    } else {
        printf("该关键字所在的节点是最底层非终端节点，没有前驱节点\n");
    }
}

//后继驱关键字
void FindNextKey(BTree t, int key, Result &result) {
    //找前驱节点
    SearchBTree(t, key, result);


    if (result.tag == 0) {
        printf("没有这个关键字\n");
        return;
    }
    BTNode *ptr = result.pt;
    int pos = result.index;

    if (NULL != result.pt->ptr[0]) {
        //关键字不是在最底层非终端节点，则有前驱节点
        BTNode *next = FindNext(ptr->ptr[pos]);
        printf("%d的后继关键字是%d\n", key, next->key[1]);
    } else {
        printf("该关键字所在的节点是最底层非终端节点，没有后继节点\n");
    }
}


//删除的关键字位于最下层的非终端结点
void DelMostBottomKey(BTree &ptr, int pos) {
    for (int i = pos; i < ptr->keynum; i++) {
        ptr->key[i] = ptr->key[i + 1];
        ptr->ptr[i] = ptr->ptr[i + 1];
    }
    ptr->keynum -= 1;
}

//右旋转（双亲结点借一个关键字给ptr，然后左兄弟给回一个最小的关键字给双亲结点你）
void RightRotate(BTree &leftbro, BTree &ptr, BTree &parent, int pos) {

    ptr->key[0] = parent->key[pos];
    for (int i = ptr->keynum; i >= 0; i--) {
        ptr->key[i + 1] = ptr->key[i];
        ptr->ptr[i + 1] = ptr->ptr[i];
    }
    ptr->keynum += 1;
    ptr->ptr[0] = leftbro->ptr[leftbro->keynum];
    if (ptr->ptr[0] != NULL)//
    {
        ptr->ptr[0]->parent = ptr;
    }
    parent->key[pos] = leftbro->key[leftbro->keynum];
    leftbro->keynum -= 1;
}

//左旋转（双亲结点借一个关键字给ptr，然后右兄弟给回一个最小的关键字给双亲结点你）
void LeftRotate(BTree &rightbro, BTree &ptr, BTree &parent, int pos) {
    ptr->key[ptr->keynum + 1] = parent->key[pos + 1];
    ptr->ptr[ptr->keynum + 1] = rightbro->ptr[0];
    if (ptr->ptr[ptr->keynum + 1] != NULL) {
        ptr->ptr[ptr->keynum + 1]->parent = ptr;
    }
    ptr->keynum += 1;
    parent->key[pos + 1] = rightbro->key[1];

    for (int i = 0; i < rightbro->keynum; i++) {
        rightbro->key[i] = rightbro->key[i + 1];
        rightbro->ptr[i] = rightbro->ptr[i + 1];
    }
    rightbro->keynum -= 1;

}

//向左合并
void LeftMerge(BTree &leftbro, BTree &ptr, BTree &parent, int pos) {
    ptr->key[0] = parent->key[pos];
    for (int i = 0, j = leftbro->keynum + 1; i <= ptr->keynum; i++, j++) {
        leftbro->key[j] = ptr->key[i];
        leftbro->ptr[j] = ptr->ptr[i];
        if (leftbro->ptr[j] != NULL) {
            leftbro->ptr[j]->parent = leftbro;
        }
    }
    leftbro->keynum = leftbro->keynum + ptr->keynum + 1;
    free(ptr);
    DelMostBottomKey(parent, pos);

}

//向右合并
void RightMerge(BTree &ptr, BTree &rightbro, BTree &parent, int pos) {
    LeftMerge(ptr, rightbro, parent, pos + 1);
}

//出现小于MIN的情况的调整函数
BTree Adjus(BTree &ptr) {
    BTree parent = ptr->parent;
    int pos = 0;
    //找到ptr在双亲中的关键字位置pos
    while (parent->ptr[pos] != ptr) ++pos;

    BTree leftbro = pos - 1 < 0 ? NULL : parent->ptr[pos - 1];        //pos - 1 < 0 ，则没有左兄弟
    BTree rightbro = pos + 1 >= MAX ? NULL : parent->ptr[pos + 1];    //pos + 1 > M则ptr没有右兄弟

    if (leftbro != NULL && leftbro->keynum > MIN) {
        //左兄弟不为空且左兄弟的关键字数目>=MIN+1, 则右旋转
        RightRotate(leftbro, ptr, parent, pos);
    } else if (rightbro != NULL && rightbro->keynum > MIN) {
        //左兄弟不为空且右兄弟的关键字数目>=MIN+1, 则左旋转
        LeftRotate(rightbro, ptr, parent, pos);
    } else if (leftbro != NULL) {
        //左右兄弟都不为空，而且左右兄弟的关键字数目都小于MIN+1, 向左合并
        LeftMerge(leftbro, ptr, parent, pos);
        ptr = leftbro;
    } else if (rightbro != NULL) {
        //左兄弟为空，右兄弟不为空，而且左右兄弟的关键字数目都小于MIN+1, 向右合并
        RightMerge(ptr, rightbro, parent, pos);
        // ptr = rightbro;
    }
    //当双亲结点关键字少于MIN的时候， 递归调用调整
    if (parent->parent != NULL && parent->keynum < MIN) {
        return Adjus(parent);
    }
    //当合并之后，如果深度减少了，释放掉原来旧的根节点
    if (parent->parent == NULL && parent->keynum <= 0) {
        free(parent);
        ptr->parent = NULL;
        return ptr;
    }
    return NULL;

}
/**
 * 删除函数， 如果是最底层的非终端节点直接删除，
 * 不是的话，用前驱或者后继结点覆盖待删除的节点，然后把前驱节点或者后继节点删除
 */
void ReMove(BTree &root, KeyType e) {
    if (root == NULL)
        return;
    Result res;
    //先搜索关键字e在那个节点，用res返回
    SearchBTree(root, e, res);
    if (res.pt == NULL || res.tag == 0) { //没有这个关键字
        printf("删除失败，没有这个关键字\n");
        return;
    }
    //找到关键字所在的节点    
    BTNode *ptr = res.pt;
    //关键字在节点的位置
    int pos = res.index;

    if (NULL != res.pt->ptr[0]) {//关键字不是在最底层非终端节点，先做处理，找前驱节点或者后继节点
        BTNode *pre = FindPre(ptr->ptr[pos - 1]);
        BTNode *next = FindNext(ptr->ptr[pos]);
        if (pre != NULL && pre->keynum > MIN) {//前驱节点关键字够，可以从前驱节点上借，并覆盖掉ptr节点
            ptr->key[pos] = pre->key[pre->keynum];
            ptr = pre;
            pos = pre->keynum;
        } else if (next != NULL && next->keynum > MIN) {//前驱不够，但是后继节关键字够，可以从后继节点上借，并覆盖掉ptr节点
            ptr->key[pos] = next->key[1];
            ptr = next;
            pos = 1;
        } else if (pre != NULL) {//前驱节点后继节点不够，但有前驱节点，先覆盖掉ptr节点，删除后，重新调整
            ptr->key[pos] = pre->key[pre->keynum];
            ptr = pre;
            pos = pre->keynum;
        } else if (next != NULL) { //前驱节点为空，后继节点关键字也不够，用后继节点覆盖，删除后，重新调整
            ptr->key[pos] = next->key[1];
            ptr = next;
            pos = 1;
        }
    }
    
    //经过上述操作，最终删除的关键字一定位于最下层的非终端节点
    DelMostBottomKey(ptr, pos);


   //调整B树
    if (ptr->parent != NULL && ptr->keynum < MIN) {//最底层不是根节点
        //进入Adjust函数再检测是否需要调整
        BTree newroot = Adjus(ptr);
        if (newroot != NULL) {
            root = newroot;
        }
    } else if (ptr->parent == NULL && ptr->keynum <= 0) {//最底层是根节点（整棵树只有根节点），且删除之后变成空树
        free(root);
        root = NULL;
    }
    //最底层是根节点（整棵树只有根节点），且删除之后如果不是空树，则删除之后不是空树也无需调整
    printf("删除成功\n");
}
//界面函数
int menu() {
    int input;
    printf("\n");
    printf("\n");
    printf("\n");
    printf("********************************************\n");
    printf("*****抽象数据类型:B树***********************\n");
    printf("********************************************\n");
    printf("input 1 遍历B树\n");
    printf("input 2 查找某一个关键字\n");
    printf("input 3 找前驱\n");
    printf("input 4 找后继\n");
    printf("input 5 整棵B树的关键字数目\n");
    printf("input 6 B树的深度（包括空层的叶子结点）\n");
    printf("input 7 求所有最下层非终端节点的关键字个数\n");
    printf("input 8 插入某一个关键字\n");
    printf("input 9 删除某一个关键字\n");
    printf("input 10 销毁B树\n");
    printf("请输入input的值: ");

    scanf("%d", &input);
    printf("\n");

    return input;
}


/******************************main函数******************************/
int main() {
    KeyType keyType[N] = {122, 136, 22, 24, 28, 1, 6, 10, 0, 108, 11, 17, 16, 5};
    BTree t;
    Result result;
    int keysnum;//关键字数目
    int dep;   //深度

    printf("初始化一棵B树\n");
    InitBTree(t);

    printf("创建一棵B树\n");
    MakeBTree(t, keyType, N);

    int bj;
    bj:{
        while (t != NULL) {
            switch (menu()) {
                case 1:
                    printf("input 1 遍历B树\n");
                    Traverse(t);
                    printf("\n");
                    break;

                case 2:
                    printf("input 2 查找某一个关键字\n");
                    int key, tag;
                    printf("请输入需要搜索的关键字：\n");
                    scanf("%d", &key);
                    SearchBTree(t, key, result);
                    tag = result.tag;
                    if (1 == tag) {
                        printf("查找成功\n");
                        printf("关键字所在的节点地址为：%d, 关键字在该节点的位置为：%d\n", result.pt, result.index);
                    } else
                        printf("没有这个关键字，查找失败\n");
                    break;
                case 3:
                    printf("input 3 找前驱\n");
                    int key2;
                    printf("请输入需要搜索的关键字：\n");
                    scanf("%d", &key2);
                    FindPreKey(t, key2, result);
                    break;
                case 4:
                    printf("input 4 找后继\n");
                    int key3;
                    printf("请输入需要搜索的关键字：\n");
                    scanf("%d", &key3);
                    FindNextKey(t, key3, result);
                    break;
                case 5:
                    keysnum = 0;
                    printf("input 5 整棵B树的关键字数目\n");
                    KeysNum(t, keysnum);
                    printf("整棵B树的关键字数目为%d\n", keysnum);
                    break;
                case 6:
                    printf("input 6 B树的深度（包括空层的叶子结点）\n");
                    dep = Depth2(t);
                    printf("B树的深度为%d(包括空层的叶子结点)\n", dep);
                    break;

                case 7:
                    int key4;
                    printf("input 7 最下层的非终端节点的关键字个数为\n");
                    MostBottomNodeKeys(t, key4);
                    printf("最下层的非终端节点的关键字个数为%d\n", key4);
                    break;
                case 8:
                    printf("input 8 插入某一个关键字\n");
                    int key5;
                    printf("请输入您要插入的关键字：\n");
                    scanf("%d", &key5);
                    SearchBTree(t, key5, result);
                    if (result.tag == 0) {
                        InsertBTree(t, key5, result.pt, result.index);
                        printf("插入成功， 遍历结果为：\n");
                        Traverse(t);
                        printf("\n");
                    } else {
                        printf("关键字已经存在，插入失败，遍历结果为：\n");
                        Traverse(t);
                        printf("\n");
                    }
                    break;
                case 9:
                    printf("input 9 删除某一个关键字\n");
                    int key6;
                    printf("请输入您要删除的关键字：\n");
                    scanf("%d", &key6);
                    ReMove(t, key6);
                    printf("最新遍历结果为：\n");
                    Traverse(t);
                    printf("\n");
                    break;
                case 10:
                    printf("input 10 销毁B树\n");
                    if (SUCCESS == DestroyBTree(t)) {
                        printf("销毁成功并退出\n");
                    } else {
                        printf("树本身为空，销毁失败\n");
                    }

                    return 0;
            } // end switch

        }  // end while

    }

    if (t == NULL){ 
        
        printf("树已经是空树了");
        printf("正在自动创建一棵B树\n");
        printf("初始化一棵B树\n");
        InitBTree(t);            
        MakeBTree(t, keyType, N);
        goto bj;
    }
    return 0;
}   // end main

/******************************结束********************************/






