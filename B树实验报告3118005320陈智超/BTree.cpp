#include <stdio.h>
#include <malloc.h> 

#define M 4   //��Ϊ4
#define  MAX  M-1
#define MIN M/2
#define N 14  //ѡȡ14���ؼ��ֵ�����Ϊ����
#define ERROR 0
#define SUCCESS 1
#define TRUE 1
#define UNSUCCESS 0
#define OVERFLOW -1
#define FALSE -1
typedef int Status;

typedef int KeyType;//�ؼ�������Ϊ����

/*typedef struct {
} Record;    */



typedef struct BTNode {
    int keynum;                 //��ǰ�ڵ�Ĺؼ�����Ŀ
    KeyType key[M + 1];         //�ؼ������飬key[0]δ��
    struct BTNode *parent;      //˫�׽��ָ��
    struct BTNode *ptr[M + 1];  //���ӽڵ�ָ������
    //Record *recptr[M + 1];    //��¼ָ��������0�ŵ�Ԫδ��
} BTNode, *BTree;               //B���Ľڵ㼰ָ������

//B�����ҵĽ������
typedef struct {
    int tag;                    //1�����ҳɹ���0������ʧ��
    BTree pt;                   //ָ���ҵ��Ľ������
    int index;                  //1 <= index <= M  �ڽڵ��еĹؼ���λ��
} Result;


//�ӿڶ���

//�������������յ�B��t
void InitBTree(BTree &t);

//�������������һ��B��
int MakeBTree(BTree &t, KeyType keyType[], int n);

//��ʼ��������t����
//�������������B��t
Status DestroyBTree(BTree &t);

//��ʼ��������t����
//���������������t����ȣ�������Ҷ�ӽ��㣩
int Depth1(BTree t);

//��ʼ��������t����
//�������������t����ȣ�����Ҷ�ӽ��㣩
int Depth2(BTree t);


//��ʼ��������t����
//�����������p�ڵ��ϲ��ҹؼ��ֵ�λ��
int SearchIndex(BTree p, int k);


//��ʼ��������t���� 
//�����������M��B��t�ϲ��ҹؼ���k, ��r���أ�pt�� index�� tag�� ���ǲ��ҳɹ�����tag = 1�� ָ��ptָ��Ľڵ��е�index���ؼ��ֵ���k������ tag = 0�� ��Ҫ����ؼ���k�� ��Ӧ��λ��pt�ڵ��е� index-1 ���͵� index ��֮��
void SearchBTree(BTree t, KeyType k, Result &r);

//��ʼ������t�Ǽ������ɵ��µĸ��ڵ㣬x���µĸ��ڵ�Ĺؼ��ֵ�ֵ��
//p��ap�Ƿ��ѿ��������ڵ㣬��˫�׽�㶼Ҫ��Ϊ��t��
//��������������µĸ��ڵ� ��

Status NewRoot(BTree &t, BTree p, int x, BTree ap);

//��ʼ������q�ڵ�ؼ�����Ŀ����mʱ�����з��ѣ�s�Ƿ��ѵ�λ�á�
//�����������q�����ѳ�������㣬 ǰһ�뱣����ԭ���Ľڵ㣬��һ  ������ap
Status Split(BTree &q, int s, BTree &ap);

//���������������Ĳ��뺯���ĸ����������ؼ���x ���½ڵ�ָ�� ap �ֱ����q->key[index] �� q->ptr[index]
void Insert(BTree &q, int index, int x, BTree ap);

//��ʼ��������t���ڣ���SearchBTree���ҵ��˲���ڵ�q�Ͳ���λ��index
//�����������B��t�� q����key[index-1]�� key[index] �в���ؼ�k
void InsertBTree(BTree &t, int k, BTree q, int index);

//��ʼ��������t����
//����������ֽ�ɸ��ڵ�����ɸ����ӽڵ㣬�ݹ����B��
void Traverse(BTree t);

//��ʼ��������t����
//��������������йؼ��ָ���sum
void KeysNum(BTree t, int &sum);

//��ʼ������ptr�ڵ����
//����������ҵ�ptr��ǰ���ڵ㣬Ϊɾ����������׼��
BTNode *FindPre(BTNode *ptr);

//��ʼ������ptr�ڵ����
//����������ҵ�ptr�ĺ�̽ڵ㣬Ϊɾ����������׼��
BTNode *FindNext(BTNode *ptr);

//��ʼ������key�����t���еĽڵ㲻����ײ�ķ��ն˽ڵ�
// ����������ҵ�key��ǰ���ؼ���
void FindPreKey(BTree t, int key, Result &result);

//��ʼ������key�����t���еĽڵ㲻����ײ�ķ��ն˽ڵ�
//����������ҵ�key�ĺ�̹ؼ���
void FindNextKey(BTree t, int key, Result &result);


//�������������ת��˫�׽���һ���ؼ��ָ�ptr��Ȼ�����ֵܸ���һ����С�Ĺؼ��ָ�˫�׽�㣩
void RightRotate(BTree &leftbro, BTree &ptr, BTree &parent, int pos);

//�������������ת��˫�׽���һ���ؼ��ָ�ptr��Ȼ�����ֵܸ���һ����С�Ĺؼ��ָ�˫�׽�㣩
void LeftRotate(BTree &rightbro, BTree &ptr, BTree &parent, int pos);

//��ʼ������ɾ���ؼ��ֺ����ڹؼ�������MIN���ֵܽڵ�Ҳû�ж���Ľڵ�
// �����������ϲ�
void LeftMerge(BTree &leftbro, BTree &ptr, BTree &parent, int pos);

//��ʼ������ɾ���ؼ��ֺ����ڹؼ�������MIN���ֵܽڵ�Ҳû�ж���Ľڵ�
//����������Һϲ�
void RightMerge(BTree &ptr, BTree &rightbro, BTree &parent, int pos);


//��ʼ����������С��MIN������ĵ�������
//�������������Ϊ����B�������
BTree  Adjus(BTree &ptr);

//��ʼ������ɾ���Ĺؼ���λ�����²�ķ��ն˽��ptr��pos�ǹؼ���key��ptr�е�λ��
//���������ɾ�����²�ķ��ն˽���еĹؼ��֣��������Remove����������
void DelMostBottomKey(BTree &ptr, int pos);

//��ʼ��������Ϊroot��B�����ڣ�eΪҪɾ���Ĺؼ���
//�����������B����ɾ��e�ؼ���
//��B����ɾ��e�ؼ���
void ReMove(BTree &root, KeyType e);


//��ʼ��һ��B��
void InitBTree(BTree &t) {
    t = NULL;
    printf("��ʼ���ɹ�\n");
}

//��p�ڵ��ϲ��ҹؼ��ֵ�λ��
int SearchIndex(BTree p, int k) {
    int index = 1;
    while (index <= p->keynum && k > p->key[index]) {
        index++;
    }

    return index;
}

/**
 * ��M��B��t�ϲ��ҹؼ���k, ��r���أ�pt�� index�� tag��
 * ���ǲ��ҳɹ�����tag = 1�� ָ��ptָ��Ľڵ��е�i���ؼ��ֵ���k
 * ���� tag = 0�� ��Ҫ����ؼ���k�� ��Ӧ��λ��pt�ڵ��е� index-1 ���͵� index ��֮��
 */
void SearchBTree(BTree t, KeyType k, Result &r) {
    int index = 0, found = 0;
    BTree p = t, q = NULL;      //��ʼ�� pָ����ڵ㣻 p������ָ�����ڵ㣬 qָ����˫�׽��
    while (p != NULL && 0 == found) {
        index = SearchIndex(p, k);
        if (index <= p->keynum && p->key[index] == k)
            found = 1;  //�ҵ�����ؼ���
        else {
            q = p;
            p = p->ptr[index - 1];//ָ������
        }

    }

    if (1 == found) { //���ҳɹ�������k��λ�� p �� index
        r.pt = p;
        r.index = index;
        r.tag = 1;
    } else {        //���Ҳ��ɹ�������k�Ĳ���λ��q��index
        r.pt = q;
        r.index = index;
        r.tag = 0;
    }
}


//�����µĸ��ڵ�
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
    t->parent = NULL;    //�¸���˫���ǿ�ָ��
    return SUCCESS;
}

//����B��
Status DestroyBTree(BTree &t) {
    if (NULL == t) return UNSUCCESS;
    for (int i = 0; i <= t->keynum; i++) {
        DestroyBTree(t->ptr[i]);
    }
    free(t);
    return SUCCESS;
}


//B���Ĳ���
void Insert(BTree &q, int index, int x, BTree ap) {
    //�ؼ���x ���½ڵ�ָ�� ap �ֱ���� q->key[index] �� q->ptr[index]
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

//��q�����ѳ�������㣬 ǰһ�뱣����ԭ���Ľڵ㣬��һ������ap��ָ��Ľڵ�
Status Split(BTree &q, int s, BTree &ap) {
  
    int i, j, n = q->keynum;
    ap = (BTNode *) malloc(sizeof(BTNode));         //�����µĽڵ�
    if (ap == NULL)
        return ERROR;
    ap->ptr[0] = q->ptr[s];
    for (i = s + 1, j = 1; i <= n; i++, j++) {     //��һ������ap��ָ��Ľڵ�
        ap->key[j] = q->key[i];
        ap->ptr[j] = q->ptr[i];
    }
    ap->keynum = n - s;
    ap->parent = q->parent;
    for (i = 0; i <= n - s; i++) {                 //�޸��½ڵ��parent��
        if (ap->ptr[i] != NULL)
            ap->ptr[i]->parent = ap;
    }

    q->keynum = s - 1;              // q�ڵ��ǰһ�뱣�����޸�keynum
    return SUCCESS;
}

/**
  * ��B��t�� q����key[index-1] �� key[index] �в��� �ؼ� k
  * �����ڲ������Ĺؼ�����Ŀ����B���Ľ�M��������˫��ָ�������з��ѣ�ʹ��t��Ȼ��M��B��
  */
void InsertBTree(BTree &t, int k, BTree q, int index) {
    int x, s, finished = 0, needNewRoot = 0;
    BTree ap;
    if (NULL == q) {
        NewRoot(t, NULL, k, NULL);  //�����µĸ��ڵ�
    } else {
        x = k;
        ap = NULL;
        while (0 == needNewRoot && 0 == finished) {
            Insert(q, index, x, ap);    //�ؼ���x ���½ڵ�ָ�� ap �ֱ���� q->key[index] �� q->ptr[index]
            if (q->keynum < M) {
                finished = 1;           //�������
            } else {                    //����q���
                s = (M + 1) / 2;
                Split(q, s, ap);
                x = q->key[s];
                if (q->parent != NULL) {
                    //��˫�׽�����ҵ� x �Ĳ���λ��
                    q = q->parent;
                    index = SearchIndex(q, x);
                } else {
                    needNewRoot = 1;
                }
            }


        }//while
    }

    if (1 == needNewRoot)        //t�ǿ������߸��ڵ��Ѿ����ѳ�q�� qp ���
        NewRoot(t, q, x, ap);    //���ɺ���Ϣ��q��x�� ap�����µĸ��ڵ�

}

//����һ��n���ؼ��ֵ�B�����洢��keyType������
int MakeBTree(BTree &t, KeyType keyType[], int n) {
    Result result;
    for (int i = 0; i < n; ++i) {
        SearchBTree(t, keyType[i], result);
        InsertBTree(t, keyType[i], result.pt, result.index);
    }
    printf("�����ɹ�\n");
}

//����
void Traverse(BTree t) {
    if (NULL == t)
        return;

    //�ֽ�ɸ��ڵ�����ɸ����ӽڵ�
    Traverse(t->ptr[0]);    //��0��������
    for (int i = 1; i <= t->keynum; ++i) {
        //�ݹ������i���ؼ��ֺ͵�i������
        printf("%d ", t->key[i]);
        Traverse(t->ptr[i]);
    }
}


//�����йؼ��ָ���sum
void KeysNum(BTree t, int &sum) {
    if (NULL == t)
        return;
    KeysNum(t->ptr[0], sum);
    for (int i = 1; i <= t->keynum; ++i) {
        sum++;
        KeysNum(t->ptr[i], sum);
    }
}

//�������²���ն˽ڵ�Ĺؼ��ָ���
void MostBottomNodeKeys(BTree t, int &bottomNodeKeys) {
    if (NULL == t)
        return;
    if (NULL == t->ptr[0])
        bottomNodeKeys = bottomNodeKeys + t->keynum;
    for (int i = 0; i <= t->keynum; ++i) {
        MostBottomNodeKeys(t->ptr[i], bottomNodeKeys);
    }
}

//B���ĸ߶ȣ��������յ�Ҷ�ӽ��
int Depth1(BTree t) {
    if (NULL == t)
        return 0;
    //����+2
    return Depth1(t->ptr[0]) + 1;

}
//B���ĸ߶ȣ������ղ��Ҷ�ӽ��
int Depth2(BTree t) {
    return Depth1(t) + 1;
}


//��ǰ���ڵ�
BTNode *FindPre(BTNode *ptr) {
    while (ptr != NULL && ptr->ptr[ptr->keynum] != NULL) {
        ptr = ptr->ptr[ptr->keynum];
    }
    return ptr;
}

//�Һ�̽ڵ�
BTNode *FindNext(BTNode *ptr) {
    while (ptr != NULL && ptr->ptr[0] != NULL) {
        ptr = ptr->ptr[0];
    }
    return ptr;
}

//��ǰ���ؼ���
void FindPreKey(BTree t, int key, Result &result) {
    //��ǰ���ڵ�
    SearchBTree(t, key, result);


    if (result.tag == 0) {
        printf("û������ؼ���\n");
        return;
    }
    BTNode *ptr = result.pt;
    int pos = result.index;

    if (NULL != result.pt->ptr[0]) {
        //�ؼ��ֲ�������ײ���ն˽ڵ㣬����ǰ���ڵ�
        BTNode *pre = FindPre(ptr->ptr[pos - 1]);
        printf("%d��ǰ���ؼ�����%d\n", key, pre->key[pre->keynum]);
    } else {
        printf("�ùؼ������ڵĽڵ�����ײ���ն˽ڵ㣬û��ǰ���ڵ�\n");
    }
}

//������ؼ���
void FindNextKey(BTree t, int key, Result &result) {
    //��ǰ���ڵ�
    SearchBTree(t, key, result);


    if (result.tag == 0) {
        printf("û������ؼ���\n");
        return;
    }
    BTNode *ptr = result.pt;
    int pos = result.index;

    if (NULL != result.pt->ptr[0]) {
        //�ؼ��ֲ�������ײ���ն˽ڵ㣬����ǰ���ڵ�
        BTNode *next = FindNext(ptr->ptr[pos]);
        printf("%d�ĺ�̹ؼ�����%d\n", key, next->key[1]);
    } else {
        printf("�ùؼ������ڵĽڵ�����ײ���ն˽ڵ㣬û�к�̽ڵ�\n");
    }
}


//ɾ���Ĺؼ���λ�����²�ķ��ն˽��
void DelMostBottomKey(BTree &ptr, int pos) {
    for (int i = pos; i < ptr->keynum; i++) {
        ptr->key[i] = ptr->key[i + 1];
        ptr->ptr[i] = ptr->ptr[i + 1];
    }
    ptr->keynum -= 1;
}

//����ת��˫�׽���һ���ؼ��ָ�ptr��Ȼ�����ֵܸ���һ����С�Ĺؼ��ָ�˫�׽���㣩
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

//����ת��˫�׽���һ���ؼ��ָ�ptr��Ȼ�����ֵܸ���һ����С�Ĺؼ��ָ�˫�׽���㣩
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

//����ϲ�
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

//���Һϲ�
void RightMerge(BTree &ptr, BTree &rightbro, BTree &parent, int pos) {
    LeftMerge(ptr, rightbro, parent, pos + 1);
}

//����С��MIN������ĵ�������
BTree Adjus(BTree &ptr) {
    BTree parent = ptr->parent;
    int pos = 0;
    //�ҵ�ptr��˫���еĹؼ���λ��pos
    while (parent->ptr[pos] != ptr) ++pos;

    BTree leftbro = pos - 1 < 0 ? NULL : parent->ptr[pos - 1];        //pos - 1 < 0 ����û�����ֵ�
    BTree rightbro = pos + 1 >= MAX ? NULL : parent->ptr[pos + 1];    //pos + 1 > M��ptrû�����ֵ�

    if (leftbro != NULL && leftbro->keynum > MIN) {
        //���ֵܲ�Ϊ�������ֵܵĹؼ�����Ŀ>=MIN+1, ������ת
        RightRotate(leftbro, ptr, parent, pos);
    } else if (rightbro != NULL && rightbro->keynum > MIN) {
        //���ֵܲ�Ϊ�������ֵܵĹؼ�����Ŀ>=MIN+1, ������ת
        LeftRotate(rightbro, ptr, parent, pos);
    } else if (leftbro != NULL) {
        //�����ֵܶ���Ϊ�գ����������ֵܵĹؼ�����Ŀ��С��MIN+1, ����ϲ�
        LeftMerge(leftbro, ptr, parent, pos);
        ptr = leftbro;
    } else if (rightbro != NULL) {
        //���ֵ�Ϊ�գ����ֵܲ�Ϊ�գ����������ֵܵĹؼ�����Ŀ��С��MIN+1, ���Һϲ�
        RightMerge(ptr, rightbro, parent, pos);
        // ptr = rightbro;
    }
    //��˫�׽��ؼ�������MIN��ʱ�� �ݹ���õ���
    if (parent->parent != NULL && parent->keynum < MIN) {
        return Adjus(parent);
    }
    //���ϲ�֮�������ȼ����ˣ��ͷŵ�ԭ���ɵĸ��ڵ�
    if (parent->parent == NULL && parent->keynum <= 0) {
        free(parent);
        ptr->parent = NULL;
        return ptr;
    }
    return NULL;

}
/**
 * ɾ�������� �������ײ�ķ��ն˽ڵ�ֱ��ɾ����
 * ���ǵĻ�����ǰ�����ߺ�̽�㸲�Ǵ�ɾ���Ľڵ㣬Ȼ���ǰ���ڵ���ߺ�̽ڵ�ɾ��
 */
void ReMove(BTree &root, KeyType e) {
    if (root == NULL)
        return;
    Result res;
    //�������ؼ���e���Ǹ��ڵ㣬��res����
    SearchBTree(root, e, res);
    if (res.pt == NULL || res.tag == 0) { //û������ؼ���
        printf("ɾ��ʧ�ܣ�û������ؼ���\n");
        return;
    }
    //�ҵ��ؼ������ڵĽڵ�    
    BTNode *ptr = res.pt;
    //�ؼ����ڽڵ��λ��
    int pos = res.index;

    if (NULL != res.pt->ptr[0]) {//�ؼ��ֲ�������ײ���ն˽ڵ㣬����������ǰ���ڵ���ߺ�̽ڵ�
        BTNode *pre = FindPre(ptr->ptr[pos - 1]);
        BTNode *next = FindNext(ptr->ptr[pos]);
        if (pre != NULL && pre->keynum > MIN) {//ǰ���ڵ�ؼ��ֹ������Դ�ǰ���ڵ��Ͻ裬�����ǵ�ptr�ڵ�
            ptr->key[pos] = pre->key[pre->keynum];
            ptr = pre;
            pos = pre->keynum;
        } else if (next != NULL && next->keynum > MIN) {//ǰ�����������Ǻ�̽ڹؼ��ֹ������ԴӺ�̽ڵ��Ͻ裬�����ǵ�ptr�ڵ�
            ptr->key[pos] = next->key[1];
            ptr = next;
            pos = 1;
        } else if (pre != NULL) {//ǰ���ڵ��̽ڵ㲻��������ǰ���ڵ㣬�ȸ��ǵ�ptr�ڵ㣬ɾ�������µ���
            ptr->key[pos] = pre->key[pre->keynum];
            ptr = pre;
            pos = pre->keynum;
        } else if (next != NULL) { //ǰ���ڵ�Ϊ�գ���̽ڵ�ؼ���Ҳ�������ú�̽ڵ㸲�ǣ�ɾ�������µ���
            ptr->key[pos] = next->key[1];
            ptr = next;
            pos = 1;
        }
    }
    
    //������������������ɾ���Ĺؼ���һ��λ�����²�ķ��ն˽ڵ�
    DelMostBottomKey(ptr, pos);


   //����B��
    if (ptr->parent != NULL && ptr->keynum < MIN) {//��ײ㲻�Ǹ��ڵ�
        //����Adjust�����ټ���Ƿ���Ҫ����
        BTree newroot = Adjus(ptr);
        if (newroot != NULL) {
            root = newroot;
        }
    } else if (ptr->parent == NULL && ptr->keynum <= 0) {//��ײ��Ǹ��ڵ㣨������ֻ�и��ڵ㣩����ɾ��֮���ɿ���
        free(root);
        root = NULL;
    }
    //��ײ��Ǹ��ڵ㣨������ֻ�и��ڵ㣩����ɾ��֮��������ǿ�������ɾ��֮���ǿ���Ҳ�������
    printf("ɾ���ɹ�\n");
}
//���溯��
int menu() {
    int input;
    printf("\n");
    printf("\n");
    printf("\n");
    printf("********************************************\n");
    printf("*****������������:B��***********************\n");
    printf("********************************************\n");
    printf("input 1 ����B��\n");
    printf("input 2 ����ĳһ���ؼ���\n");
    printf("input 3 ��ǰ��\n");
    printf("input 4 �Һ��\n");
    printf("input 5 ����B���Ĺؼ�����Ŀ\n");
    printf("input 6 B������ȣ������ղ��Ҷ�ӽ�㣩\n");
    printf("input 7 ���������²���ն˽ڵ�Ĺؼ��ָ���\n");
    printf("input 8 ����ĳһ���ؼ���\n");
    printf("input 9 ɾ��ĳһ���ؼ���\n");
    printf("input 10 ����B��\n");
    printf("������input��ֵ: ");

    scanf("%d", &input);
    printf("\n");

    return input;
}


/******************************main����******************************/
int main() {
    KeyType keyType[N] = {122, 136, 22, 24, 28, 1, 6, 10, 0, 108, 11, 17, 16, 5};
    BTree t;
    Result result;
    int keysnum;//�ؼ�����Ŀ
    int dep;   //���

    printf("��ʼ��һ��B��\n");
    InitBTree(t);

    printf("����һ��B��\n");
    MakeBTree(t, keyType, N);

    int bj;
    bj:{
        while (t != NULL) {
            switch (menu()) {
                case 1:
                    printf("input 1 ����B��\n");
                    Traverse(t);
                    printf("\n");
                    break;

                case 2:
                    printf("input 2 ����ĳһ���ؼ���\n");
                    int key, tag;
                    printf("��������Ҫ�����Ĺؼ��֣�\n");
                    scanf("%d", &key);
                    SearchBTree(t, key, result);
                    tag = result.tag;
                    if (1 == tag) {
                        printf("���ҳɹ�\n");
                        printf("�ؼ������ڵĽڵ��ַΪ��%d, �ؼ����ڸýڵ��λ��Ϊ��%d\n", result.pt, result.index);
                    } else
                        printf("û������ؼ��֣�����ʧ��\n");
                    break;
                case 3:
                    printf("input 3 ��ǰ��\n");
                    int key2;
                    printf("��������Ҫ�����Ĺؼ��֣�\n");
                    scanf("%d", &key2);
                    FindPreKey(t, key2, result);
                    break;
                case 4:
                    printf("input 4 �Һ��\n");
                    int key3;
                    printf("��������Ҫ�����Ĺؼ��֣�\n");
                    scanf("%d", &key3);
                    FindNextKey(t, key3, result);
                    break;
                case 5:
                    keysnum = 0;
                    printf("input 5 ����B���Ĺؼ�����Ŀ\n");
                    KeysNum(t, keysnum);
                    printf("����B���Ĺؼ�����ĿΪ%d\n", keysnum);
                    break;
                case 6:
                    printf("input 6 B������ȣ������ղ��Ҷ�ӽ�㣩\n");
                    dep = Depth2(t);
                    printf("B�������Ϊ%d(�����ղ��Ҷ�ӽ��)\n", dep);
                    break;

                case 7:
                    int key4;
                    printf("input 7 ���²�ķ��ն˽ڵ�Ĺؼ��ָ���Ϊ\n");
                    MostBottomNodeKeys(t, key4);
                    printf("���²�ķ��ն˽ڵ�Ĺؼ��ָ���Ϊ%d\n", key4);
                    break;
                case 8:
                    printf("input 8 ����ĳһ���ؼ���\n");
                    int key5;
                    printf("��������Ҫ����Ĺؼ��֣�\n");
                    scanf("%d", &key5);
                    SearchBTree(t, key5, result);
                    if (result.tag == 0) {
                        InsertBTree(t, key5, result.pt, result.index);
                        printf("����ɹ��� �������Ϊ��\n");
                        Traverse(t);
                        printf("\n");
                    } else {
                        printf("�ؼ����Ѿ����ڣ�����ʧ�ܣ��������Ϊ��\n");
                        Traverse(t);
                        printf("\n");
                    }
                    break;
                case 9:
                    printf("input 9 ɾ��ĳһ���ؼ���\n");
                    int key6;
                    printf("��������Ҫɾ���Ĺؼ��֣�\n");
                    scanf("%d", &key6);
                    ReMove(t, key6);
                    printf("���±������Ϊ��\n");
                    Traverse(t);
                    printf("\n");
                    break;
                case 10:
                    printf("input 10 ����B��\n");
                    if (SUCCESS == DestroyBTree(t)) {
                        printf("���ٳɹ����˳�\n");
                    } else {
                        printf("������Ϊ�գ�����ʧ��\n");
                    }

                    return 0;
            } // end switch

        }  // end while

    }

    if (t == NULL){ 
        
        printf("���Ѿ��ǿ�����");
        printf("�����Զ�����һ��B��\n");
        printf("��ʼ��һ��B��\n");
        InitBTree(t);            
        MakeBTree(t, keyType, N);
        goto bj;
    }
    return 0;
}   // end main

/******************************����********************************/






