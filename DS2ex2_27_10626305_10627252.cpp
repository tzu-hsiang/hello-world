// 請助教下載GitHub版本評分
// 10626305 魏子翔 10627252 徐綺柔
#include<iostream>
#include<cstdio>
#include<windows.h>
#include<string>
#include<vector>
#include<stack>

#define NAMESIZE 20
#define TMPSIZE 200
#define PTR_NUM 3
#define KEY_NUM 2

using namespace std ;

class Data{

public:
    int serialNum ;
    string schoolName ;
    string department ;
    string type ;
    string degree ;
    string stuNum ;
    int teachNum ;
    int graStu ;

    Data *SaveData(char *choice, char *filename, int &dataNum) ;

private:
    void CalculateDataNum(FILE *ptr, int &dataNum) ;
} ;

typedef struct slotT
{
    vector<Data> rSet ;
    string key ;
} slotType ;

typedef struct nT
{
    slotType data[KEY_NUM] ;
    struct nT *link[PTR_NUM] ;
    struct nT *parent ;
} nodeType ;

typedef struct pointT
{
    nodeType *pNode ; // record searching node

} pointType ;

typedef struct bT
{
    slotType slot ;
    nodeType *link ;
} blockType ;

class avlTNode
{
public:
    vector<Data> sameKey ;
    Data *item ;
    avlTNode *left ;
    avlTNode *right ;
    int height ;
};

void ModeChoose(int mode) ;

void Insert23Tree(Data newRid, string newKey, nodeType *&root, int &count) ;
nodeType *CreateNode(nodeType *left, nodeType *mid, nodeType *right, nodeType *pNode, slotType newSlot, int &count) ;
void SearchDown(nodeType *cur, string newKey, stack<pointType> &aPath) ;
void InsertLeaf(slotType newSlot, pointType aLeaf) ;
void SplitLeaf(slotType newSlot, pointType aLeaf, blockType &aBlock, int &count) ;
nodeType *CreateRoot(nodeType *newLeft, nodeType *newRight, slotType oneSlot, int &count) ;
void InsertNonLeaf(blockType &aBlock, pointType aNonLeaf, int &count) ;
void SplitNonLeaf(blockType &aBlock, pointType aNonLeaf, int &count) ;
int twoThreeHeight(nodeType *aNode) ;
void judgeSwapRoot23tree(nodeType *root) ;
void print23TreeRoot(nodeType *root) ;

void avlFree(avlTNode *cur) ;
avlTNode* insert(Data *newData, avlTNode *&cur) ;
avlTNode* RightRotate(avlTNode *&root) ;
avlTNode* LeftRotate(avlTNode *&root) ;
int height(avlTNode *anode) ;
void inorder(avlTNode *cur, int &count) ;
void swapRootAvlTree(avlTNode *root) ;
void printAvlTreeRoot(avlTNode *root) ;

int main()
{
    int mode ;

    cout << endl
         << "******************************" << "\n"
         << "*  1 . Create 2-3 tree       *" << "\n"
         << "*  2 . Create AVL tree       *" << "\n"
         << "* Any. Quit                  *" << "\n"
         << "******************************" << "\n"
         << "Choose a mode:" ;
    cin >> mode ;

    while (mode == 1 || mode == 2)
    {
        ModeChoose(mode) ;
        cout << endl
             << "******************************" << "\n"
             << "*  1 . Create 2-3 tree       *" << "\n"
             << "*  2 . Create AVL tree       *" << "\n"
             << "* Any. Quit                  *" << "\n"
             << "******************************" << "\n"
             << "Choose a mode:" ;
        cin >> mode ;
    }

    return 0 ;
}

void ModeChoose(int mode)
{
    if (mode == 1)
    {
        char *choose = new char[3] ;
        cout << "Please enter file number(ex: 201, 202, xxx):" ;
        cin >> choose ;

        char *filename = new char[NAMESIZE] ;
        int dataNum = 0 ;
        Data *dataPtr = dataPtr->SaveData(choose, filename, dataNum) ;

        delete [] filename ;


        if (dataPtr == NULL)
        {
            cout << "Please enter correct number(ex: 201, 202, xxx)\n" ;
            delete [] dataPtr ;
        }
        else
        {
            nodeType *root = NULL ;
            int count = 0 ;

            for (int i = 0; i < dataNum ; i++)
            {
                Insert23Tree(dataPtr[i], dataPtr[i].schoolName, root, count) ;
            }

            cout << "Tree height = " << twoThreeHeight(root) << endl ;
            cout << "Number of nodes = " << count << endl ;

            judgeSwapRoot23tree(root) ;
            print23TreeRoot(root) ;

            delete [] dataPtr ;

        }

    }

    if (mode == 2)
    {
        char *choose = new char[3] ;
        cout << "Please enter file number(ex: 201, 202, xxx):" ;
        cin >> choose ;

        char *filename = new char[NAMESIZE] ;
        int dataNum = 0 ;
        Data *dataPtr = dataPtr->SaveData(choose, filename, dataNum) ;

        delete [] filename ;

        if (dataPtr == NULL)
        {
            cout << "Please enter correct number(ex: 201, 202, xxx)\n" ;
            delete [] dataPtr ;
        }
        else
        {
            avlTNode *root = NULL ;

            for (int i = 0; i < dataNum; i++)
            {
                insert(dataPtr+i, root) ;
            }

            int count = 0 ;
            inorder(root, count) ;

            cout << "Tree height = " << height(root) << endl ;
            cout << "Number of nodes = " << count << endl ;

            swapRootAvlTree(root) ;
            printAvlTreeRoot(root) ;

            avlFree(root) ;
            delete [] dataPtr ;
        }


    }


}

void Insert23Tree(Data newRid, string newKey, nodeType *&root, int &count)
{
    // create a slot to insert into a node
    slotType newSlot ;
    newSlot.rSet.push_back(newRid) ;
    newSlot.key = newKey ;

    if (root == NULL)
    {
        // set newSlot's left, right, mid, parent
        root = CreateNode(NULL, NULL, NULL, NULL, newSlot, count) ;
    }
    else
    {
        stack<pointType> aPath ;

        pointType curP ; // now position
        blockType blockUp ; // after split, get a block

        SearchDown(root, newKey, aPath) ;

        if (aPath.empty() == 0)
        {
            curP = aPath.top() ; // first times, the top is leaf or root(only a node)
                                 // equal case, the top is a node, who's slot has same key

            if ((curP.pNode->data[0].rSet.empty() == 0) // duplicate key at left slot
                && (newKey.compare(curP.pNode->data[0].key)) == 0)
            {
                curP.pNode->data[0].rSet.push_back(newRid) ;
            }
            else if ((curP.pNode->data[1].rSet.empty() == 0) // duplicate key at right slot
                     && (newKey.compare(curP.pNode->data[1].key)) == 0)
            {
                curP.pNode->data[1].rSet.push_back(newRid) ;
            }
            else if (curP.pNode->data[0].rSet.empty() == 0  // leaf has unused slot
                     && curP.pNode->data[1].rSet.empty() == 1
                     && (curP.pNode->link[0] == NULL && curP.pNode->link[2] == NULL ))
            {
                InsertLeaf(newSlot, curP) ;
            }
            else // leaf is full, need to split
            {
                SplitLeaf(newSlot, curP, blockUp, count) ;

                if (curP.pNode->parent == NULL) // split root
                {
                    root = CreateRoot(curP.pNode, blockUp.link, blockUp.slot, count) ;
                }
                else // split non-leaf
                {
                    while (1)
                    {

                        aPath.pop() ;

                        curP = aPath.top() ;

                        if (curP.pNode->data[1].rSet.empty() == 1) // node's right slot is empty
                        {
                            InsertNonLeaf(blockUp, curP, count) ;

                            break ;
                        }
                        else // split a full non-leaf
                        {
                            SplitNonLeaf(blockUp, curP, count) ;

                            if (curP.pNode->parent == NULL) // split root, create new root
                            {
                                root = CreateRoot(curP.pNode, blockUp.link, blockUp.slot, count) ;

                                break ;
                            }
                        }
                    } // end while ()
                }
            }
        }
    } // end outer else

}

nodeType *CreateNode(nodeType *left, nodeType *mid, nodeType *right, nodeType *pNode, slotType newSlot, int &count)
{
    // set newSlot into node's left slot
    nodeType *newNode = new nodeType ;
    newNode->data[0].rSet = newSlot.rSet ;
    newNode->data[1].rSet.clear() ;
    newNode->data[0].key = newSlot.key ;
    newNode->data[1].key = "" ;
    newNode->parent = pNode ;
    newNode->link[0] = left ;
    newNode->link[1] = mid ;
    newNode->link[2] = right ;

    count++ ;
    return newNode ;
}

void SearchDown(nodeType *cur, string newKey, stack<pointType> &aPath)
{
    pointType oneP ;
    int i = 1 ;

    while (i)
    {
        oneP.pNode = cur ;

        if (cur->data[0].rSet.empty() == 0 && cur->data[1].rSet.empty() == 0
            && cur->link[1] != NULL)
        { // 節點存在兩個slot, 且中間child存在(non-leaf)

            if (newKey.compare(cur->data[0].key) == 0
               || newKey.compare(cur->data[1].key) == 0)
            {
                // 1.newKey 等於 data[0]或data[1] 直接return
                aPath.push(oneP) ;
                return ;
            }
            else if (newKey.compare(cur->data[0].key) < 0)
            {
                // 2.newKey 小於 data[0]走左邊
                aPath.push(oneP) ;
                cur = cur->link[0] ;
            }
            else if (newKey.compare(cur->data[1].key) > 0)
            {
                // 3.newKey 大於 data[1]走右邊
                aPath.push(oneP) ;
                cur = cur->link[2] ;
            }
            else
            {
                // 4.newKey 介於 data[0] && data[1]走中間
                aPath.push(oneP) ;
                cur = cur->link[1] ;
            }
        }
        else if (cur->data[0].rSet.empty() == 0 && cur->data[1].rSet.empty() == 0
                 && cur->link[1] == NULL)
        { // 節點存在兩個slot, 但中間child不存在(it's leaf)

            aPath.push(oneP) ;
            return ;
        }
        else if (cur->data[0].rSet.empty() == 0 && cur->data[1].rSet.empty() == 1
                 && (cur->link[0] != NULL || cur->link[2] != NULL))
        { // 節點只有一個slot 且 non-leaf

            if (newKey.compare(cur->data[0].key) == 0)
            {
                // 3.newKey 等於 data[0] 存入堆疊 結束
                aPath.push(oneP) ;
                return ;
            }
            else if (newKey.compare(cur->data[0].key) < 0)
            {
                // 1.newKey 小於 data[0]走左邊
                aPath.push(oneP) ;
                cur = cur->link[0] ;
            }
            else if (newKey.compare(cur->data[0].key) > 0)
            {
                // 2.newKey 大於 data[0]走右邊
                aPath.push(oneP) ;
                cur = cur->link[2] ;
            }

        }
        else if (cur->data[0].rSet.empty() == 0 && cur->data[1].rSet.empty() == 1
                 && (cur->link[0] == NULL && cur->link[2] == NULL))
        { // 節點只有一個slot 但 it's leaf
            aPath.push(oneP) ;
            return ;
        }

        if (i > 10)
        {
            cout << "error!!! Infinite loop\n" ;
            system("PAUSE") ;
        }

        i++ ;
    } // end while ()

}

void InsertLeaf(slotType newSlot, pointType aLeaf)
{
    // 插入並排序
    // 1. new slot的key 小於 leaf的左slot的key
    // 2. new slot的key 大於 leaf的左slot的key
    if ((newSlot.key).compare(aLeaf.pNode->data[0].key) < 0)
    {
        aLeaf.pNode->data[1] = aLeaf.pNode->data[0] ;
        aLeaf.pNode->data[0] = newSlot ;
    }
    else if ((newSlot.key).compare(aLeaf.pNode->data[0].key) > 0)
    {
        aLeaf.pNode->data[1] = newSlot ;
    }

}

void SplitLeaf(slotType newSlot, pointType aLeaf, blockType &aBlock, int &count)
{
    slotType buffer[3] ;

    // sort slot and newSlot into buffer by key
    // 1. 新插入的slot的key < Leaf的左slot的key
    // 2. (新插入的slot的key > Leaf的左slot的key)
    //    && (新插入的slot的key < Leaf的右slot的key)
    // 3. 新插入的slot的key > Leaf的右slot的key
    if ((newSlot.key).compare(aLeaf.pNode->data[0].key) < 0)
    {
        buffer[2] = aLeaf.pNode->data[1] ;
        buffer[1] = aLeaf.pNode->data[0] ;
        buffer[0] = newSlot ;
    }
    else if (((newSlot.key).compare(aLeaf.pNode->data[0].key) > 0)
             && (newSlot.key).compare(aLeaf.pNode->data[1].key) < 0)
    {
        buffer[2] = aLeaf.pNode->data[1] ;
        buffer[1] = newSlot ;
        buffer[0] = aLeaf.pNode->data[0] ;
    }
    else if ((newSlot.key).compare(aLeaf.pNode->data[1].key) > 0)
    {
        buffer[2] = newSlot ;
        buffer[1] = aLeaf.pNode->data[1] ;
        buffer[0] = aLeaf.pNode->data[0] ;
    }

    aLeaf.pNode->data[0] = buffer[0] ;

    aLeaf.pNode->data[1].rSet.clear() ; // clear the right slot

    aBlock.link = CreateNode(NULL, NULL, NULL, aLeaf.pNode->parent, buffer[2], count) ;

    aBlock.slot = buffer[1] ;

}

nodeType *CreateRoot(nodeType *newLeft, nodeType *newRight, slotType oneSlot, int &count)
{
    nodeType *newRoot = CreateNode(newLeft, NULL, newRight, NULL, oneSlot, count) ;

    newLeft->parent = newRoot ;
    newRight->parent = newRoot ;
    return newRoot ;
}

void InsertNonLeaf(blockType &aBlock, pointType aNonLeaf, int &count)
{
    // sort, than insert newSlot
    if ((aBlock.slot.key).compare(aNonLeaf.pNode->data[0].key) < 0)
    {
        aNonLeaf.pNode->data[1] = aNonLeaf.pNode->data[0] ;
        aNonLeaf.pNode->data[0] = aBlock.slot ;
    }
    else
    {
        aNonLeaf.pNode->data[1] = aBlock.slot ;
    }

    nodeType *ptr[3] ;
    ptr[0] = aNonLeaf.pNode->link[0] ;
    ptr[1] = aNonLeaf.pNode->link[2] ;
    ptr[2] = aBlock.link ;

    for (int i = 0; i < 2; i++) // sort point
    {
        for (int j = 0; j < 3 - 1 - i; j++)
        {
            if ((ptr[j]->data[0].key).compare(ptr[j+1]->data[0].key) > 0)
            {
                nodeType *tmp = ptr[j] ;
                ptr[j] = ptr[j+1] ;
                ptr[j+1] = tmp ;
            }
        }
    }

    aNonLeaf.pNode->link[0] = ptr[0] ;
    aNonLeaf.pNode->link[1] = ptr[1] ;
    aNonLeaf.pNode->link[2] = ptr[2] ;

}

void SplitNonLeaf(blockType &aBlock, pointType aNonLeaf, int &count)
{
    // 傳入的是 節點分裂帶上來的ablock(包含block->link 右節點)
    // 以及 往上track的節點(curP)
    slotType buffer[3] ;
    nodeType *ptr[4] ;
    int index = 0 ;

    if ((aBlock.slot.key).compare(aNonLeaf.pNode->data[0].key) < 0)
    {
        buffer[2] = aNonLeaf.pNode->data[1] ;
        buffer[1] = aNonLeaf.pNode->data[0] ;
        buffer[0] = aBlock.slot ;
    }
    else if (((aBlock.slot.key).compare(aNonLeaf.pNode->data[0].key) > 0)
             && (aBlock.slot.key).compare(aNonLeaf.pNode->data[1].key) < 0)
    {
        buffer[2] = aNonLeaf.pNode->data[1] ;
        buffer[1] = aBlock.slot ;
        buffer[0] = aNonLeaf.pNode->data[0] ;
    }
    else if ((aBlock.slot.key).compare(aNonLeaf.pNode->data[1].key) > 0)
    {
        buffer[2] = aBlock.slot ;
        buffer[1] = aNonLeaf.pNode->data[1] ;
        buffer[0] = aNonLeaf.pNode->data[0] ;
    }

    ptr[0] = aNonLeaf.pNode->link[0] ;
    ptr[1] = aNonLeaf.pNode->link[1] ;
    ptr[2] = aNonLeaf.pNode->link[2] ;
    ptr[3] = aBlock.link ;

    for (int i = 0; i < 3; i++) // sort point
    {
        for (int j = 0; j < 4 - 1 - i; j++)
        {
            if ((ptr[j]->data[0].key).compare(ptr[j+1]->data[0].key) > 0)
            {
                nodeType *tmp = ptr[j] ;
                ptr[j] = ptr[j+1] ;
                ptr[j+1] = tmp ;
            }
        }
    }

    aNonLeaf.pNode->link[0] = ptr[0] ;
    aNonLeaf.pNode->link[2] = ptr[1] ;

    aNonLeaf.pNode->data[0] = buffer[0] ;
    aNonLeaf.pNode->data[1].rSet.clear() ;

    aBlock.slot = buffer[1] ;

    aBlock.link = CreateNode(ptr[2],NULL, ptr[3], aNonLeaf.pNode->parent, buffer[2], count) ;

}

int twoThreeHeight(nodeType *aNode)
{
    int height = 0 ;
    while (aNode != NULL)
    {
        height++ ;
        aNode = aNode->link[0] ;
    }

    return height ;
}

void judgeSwapRoot23tree(nodeType *root)
{
    if (root->data[0].rSet.size() > 1)
    {
        bool flag = false ;
        for (int i = 0; i < root->data[0].rSet.size() && !flag; i++) // sort by serial number
        {
            flag = true ;
            for (int j = 0; j < root->data[0].rSet.size() - 1 - i; j++)
            {
                if (root->data[0].rSet[j].serialNum > root->data[0].rSet[j+1].serialNum )
                {
                    Data temp = root->data[0].rSet[j+1] ;
                    root->data[0].rSet[j+1] = root->data[0].rSet[j] ;
                    root->data[0].rSet[j] = temp ;
                    flag = false ;
                }
            }
        }
    }

    if (root->data[1].rSet.size() > 1)
    {
        bool flag = false ;
        for (int i = 0; i < root->data[1].rSet.size() && !flag; i++) // sort by serial number
        {
            flag = true ;
            for (int j = 0; j < root->data[1].rSet.size() - 1 - i; j++)
            {
                if (root->data[1].rSet[j].serialNum > root->data[1].rSet[j+1].serialNum )
                {
                    Data temp = root->data[1].rSet[j+1] ;
                    root->data[1].rSet[j+1] = root->data[0].rSet[j] ;
                    root->data[1].rSet[j] = temp ;
                    flag = false ;
                }
            }
        }
    }
}

void print23TreeRoot(nodeType *root)
{
    if (root->data[1].rSet.empty() == 0)
    {
        if (root->data[0].rSet[0].serialNum < root->data[1].rSet[1].serialNum)
        {
            for (int n = 0; n < root->data[0].rSet.size(); n++ )
            {
                cout << n + 1 << ":[" << root->data[0].rSet[n].serialNum << "] "
                    << root->data[0].rSet[n].schoolName << ", "
                    << root->data[0].rSet[n].department << ", "
                    << root->data[0].rSet[n].type << ", "
                    << root->data[0].rSet[n].degree << ", "
                    << root->data[0].rSet[n].graStu << endl ;
            }

            for (int n = 0; n < root->data[1].rSet.size(); n++ )
            {
                cout << n + 1 << ":[" << root->data[1].rSet[n].serialNum << "] "
                    << root->data[1].rSet[n].schoolName << ", "
                    << root->data[1].rSet[n].department << ", "
                    << root->data[1].rSet[n].type << ", "
                    << root->data[1].rSet[n].degree << ", "
                    << root->data[1].rSet[n].graStu << endl ;
            }
        }
        else
        {
            for (int n = 0; n < root->data[1].rSet.size(); n++ )
            {
                cout << n + 1 << ":[" << root->data[1].rSet[n].serialNum << "] "
                    << root->data[1].rSet[n].schoolName << ", "
                    << root->data[1].rSet[n].department << ", "
                    << root->data[1].rSet[n].type << ", "
                    << root->data[1].rSet[n].degree << ", "
                    << root->data[1].rSet[n].graStu << endl ;
            }

            for (int n = 0; n < root->data[0].rSet.size(); n++ )
            {
                cout << n + 1 << ":[" << root->data[0].rSet[n].serialNum << "] "
                    << root->data[0].rSet[n].schoolName << ", "
                    << root->data[0].rSet[n].department << ", "
                    << root->data[0].rSet[n].type << ", "
                    << root->data[0].rSet[n].degree << ", "
                    << root->data[0].rSet[n].graStu << endl ;
            }
        }
    }
    else
    {
        for (int n = 0; n < root->data[0].rSet.size(); n++ )
        {
            cout << n + 1 << ":[" << root->data[0].rSet[n].serialNum << "] "
                << root->data[0].rSet[n].schoolName << ", "
                << root->data[0].rSet[n].department << ", "
                << root->data[0].rSet[n].type << ", "
                << root->data[0].rSet[n].degree << ", "
                << root->data[0].rSet[n].graStu << endl ;
        }
    }
}

void Data::CalculateDataNum(FILE *ptr, int &dataNum)
{
    if (!ptr)
    {
        cout << "can not found!\n" ;
        dataNum = -1 ;
        return ;
    }

    // start to calculate data number
    char *goal = new char[TMPSIZE] ;

    while (!feof(ptr)) // count line
    {
        fgets(goal, 200, ptr) ;
        dataNum++ ;

        if (*goal == ' ' || *goal == '\t' || *goal == '\n' )
            break ;

    }
    dataNum -= 3 ;

    for( int i = 0; i <= TMPSIZE; i++)
    {
        char *find = strchr(goal, '\n') ;
        if (find)
        {
            dataNum-- ;
            break ;
        }
    }
    delete [] goal ;
}

Data *Data::SaveData(char *choice, char *filename, int &dataNum)
{
    sprintf(filename, "input%s.txt", choice) ;
    delete [] choice ;

    FILE *ifptr = fopen(filename, "r") ;
    Data *start = new Data ;
    start->CalculateDataNum(ifptr, dataNum) ;
    delete start ;

    if (dataNum == -1)
        return NULL ;

    // start to save data
    Data *dataPtr = new Data[dataNum] ;
    fseek(ifptr, 100, SEEK_SET) ;
    fscanf(ifptr, "%*[^\n]") ;

    char garbage ;
    char ch ;
    int tabNum = 0 ;
    int j ;
    char *tmp = new char[TMPSIZE] ;

    for ( int i = 0 ; i < dataNum ; i++)
    {
        (dataPtr+i)->serialNum = i + 1 ;

        while ((garbage = fgetc(ifptr)) != '\t')
            ; // skip school code and '\t'

        fscanf(ifptr, "%s", tmp) ;
        ((dataPtr+i)->schoolName).assign(tmp) ;

        while (tabNum < 2) // 跳過科系代碼
        {
            garbage = fgetc(ifptr) ;
            if (garbage == '\t')
                tabNum++ ;
        }

        fscanf(ifptr, "%s", tmp) ;
        ((dataPtr+i)->department).assign(tmp) ; // (*char->string)

        garbage = fgetc(ifptr) ;

        ch = fgetc(ifptr) ;
        for ( j = 0; ch != '\t'; j++)
        {
            *(tmp+j) = ch ;
            ch = fgetc(ifptr) ;

        }
        *(tmp+j) = '\0' ;
        ((dataPtr+i)->type).assign(tmp) ;

        ch = fgetc(ifptr) ; // read grade
        for ( j = 0; ch != '\t'; j++)
        {
            *(tmp+j) = ch ;
            ch = fgetc(ifptr) ;
        }
        *(tmp+j) = '\0' ;
        ((dataPtr+i)->degree).assign(tmp) ; // save grade

        ch = fgetc(ifptr) ; // read stuNum
        for ( j = 0; ch != '\t'; j++)
        {
            *(tmp+j) = ch ;
            ch = fgetc(ifptr) ;
        }
        *(tmp+j) = '\0' ;
        ((dataPtr+i)->stuNum).assign(tmp) ; // save stuNum

        fscanf(ifptr, "%d", &(dataPtr+i)->teachNum) ;
        fscanf(ifptr, "%d", &(dataPtr+i)->graStu) ;

        fscanf(ifptr, "%*[^\n]") ; // end this data line
        tabNum = 0 ; // initialize

    }

    delete [] tmp ;
    fclose(ifptr) ;

    return dataPtr ;

}

int height(avlTNode *anode)
{
    if (anode == NULL)
        return 0 ;
    else
    {
        int lLevel = height(anode->left) ;
        int rLevel = height(anode->right) ;

        if (lLevel > rLevel)
            return (lLevel + 1) ;
        else
            return (rLevel + 1) ;
    }
}

avlTNode *insert(Data *newData, avlTNode *&cur)
{
    if (cur == NULL)
    {
        cur = new avlTNode ;
        cur->item = newData ;
        cur->height = 0 ;
        cur->left = NULL ;
        cur->right = NULL ;
    }
    else if (newData->graStu < cur->item->graStu) // insert to left
    {
        cur->left = insert(newData, cur->left) ;
        if (height(cur->left) - height(cur->right) == 2)
        {
            if (newData->graStu < cur->left->item->graStu) // type LL
                cur = RightRotate(cur) ;
            else // type LR
            {
                cur->left = LeftRotate(cur->left) ;
                cur = RightRotate(cur) ;
            }
        }
    }
    else if (newData->graStu > cur->item->graStu) // inset to right
    {
        cur->right = insert(newData, cur->right) ;
        if (height(cur->left) - height(cur->right) == -2)
        {
            if (newData->graStu > cur->right->item->graStu) // type RR
                cur = LeftRotate(cur) ;
            else // type RL
            {
                cur->right = RightRotate(cur->right) ;
                cur = LeftRotate(cur) ;
            }
        }
    }
    else  // equal, insert to same key node
    {
        cur->sameKey.push_back(*newData) ;
    }

    cur->height = max(height(cur->left), height(cur->right)) + 1 ;
    return cur ;
}

avlTNode* RightRotate(avlTNode *&root)
{
    avlTNode *newRoot = root->left ;
    root->left = newRoot->right ;
    newRoot->right = root ;
    root->height = max(height(root->left), height(root->right)) + 1 ;
    newRoot->height = max(height(newRoot->left), root->height) + 1 ;

    return newRoot ;
}

avlTNode* LeftRotate(avlTNode *&root)
{
    avlTNode* newRoot = root->right ;
    root->right = newRoot->left ;
    newRoot->left = root ;
    root->height = max(height(root->left), height(root->right)) + 1 ;
    newRoot->height = max(height(root->right), root->height) + 1 ;

    return newRoot ;
}

void inorder(avlTNode *cur, int &count)
{
    if (cur == NULL)
        return ;
    inorder(cur->left, count) ;
    count++ ;
    inorder(cur->right, count) ;
}

void swapRootAvlTree(avlTNode *root)
{
    root->sameKey.push_back(*(root->item)) ;

    if (root->sameKey.size() > 1)
    {
        bool flag = false ;
        for (int i = 0; i < root->sameKey.size() - 1 && !flag; i++) // sort by serial number
        {
            flag = true ;
            for (int j = 0; j < root->sameKey.size() - 1 - i; j++)
            {
                if (root->sameKey[j].serialNum > root->sameKey[j+1].serialNum )
                {
                    Data temp = root->sameKey[j+1] ;
                    root->sameKey[j+1] = root->sameKey[j] ;
                    root->sameKey[j] = temp ;
                    flag = false ;
                }
            }
        }
    }
}

void printAvlTreeRoot(avlTNode *root)
{
    for (int n = 0; n < root->sameKey.size(); n++ )
    {
        cout << n + 1 << ":[" << root->sameKey[n].serialNum << "] "
            << root->sameKey[n].schoolName << ", "
            << root->sameKey[n].department << ", "
            << root->sameKey[n].type << ", "
            << root->sameKey[n].degree << ", "
            << root->sameKey[n].graStu << endl ;
    }
}

void avlFree(avlTNode *cur)
{
    if (cur == NULL)
        return ;
    avlFree(cur->left) ;
    avlFree(cur->right) ;

    delete cur ;
}
