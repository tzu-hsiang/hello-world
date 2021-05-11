// 10626305 魏子翔
#include<iostream>
#include<cstdio>
#include<string>
#include<windows.h>
#include<iomanip>

#define NAMESIZE 13
#define TMPSIZE 200

using namespace std ;

class Data{

public:
    int itemNum ;
    string schoolName ;
    string department ;
    string type ;
    string degree ;
    int stuNum ;
    int teachNum ;
    int graStu ;

};

struct node{

    int keyNum ;
    Data *adata ;
    node *left ;
    node *right ;
    node *parent ;
};


bool DataInput(char choice[], char filename[]) ;
node *createTreeGra(Data *dataPointer, int dataSize) ;
void insertNode(int keyValue, Data *DataPtr, node *currentPtr, node *rootPtr, int &treeHigh) ;
int height(node* anode) ;
void deleteNode(node *root, int value, int &count, bool &esc, bool &empty) ;

int main()
{
    int mode ;
    char *choose = new char[3] ;

    cout << endl
         << "******************************" << "\n"
         << "*  1 . Create trees          *" << "\n"
         << "* Any. Quit                  *" << "\n"
         << "******************************" << "\n"
         << "Choose a mode:" ;
    cin >> mode ;

    while (mode == 1)
    {
        cout << "Please enter file number(ex: 001, 002, xxx):" ;
        cin >> choose ;
        char filename[NAMESIZE] ;

        if (!DataInput(choose, filename))
            cout << "Please enter correct number(ex: 001, 002, xxx)\n" ;

        cout << endl
             << "******************************" << "\n"
             << "*  1 . Create  trees         *" << "\n"
             << "* Any. Quit                  *" << "\n"
             << "******************************" << "\n"
             << "Choose a mode:" ;
        cin >> mode ;

    }



    return 0 ;
}

bool DataInput (char choice[], char filename[])
{
    sprintf(filename, "input%s.txt", choice) ;
    delete [] choice ;

    FILE *ifptr = fopen( filename, "r" ) ;

    /// 以下讀檔
    if (!ifptr)
    {
        cout << "can not found!\n" ;
        return false ;
    }

    int gotline = 0 ;
    char *goal = new char[TMPSIZE] ;

    while (!feof(ifptr)) // 計算行數
    {
        fgets(goal, 200, ifptr) ;
        gotline++ ;

        if (*goal == ' ' || *goal == '\t' || *goal == '\n' )
            break ;

    }
    gotline -= 3 ;

    for( int i = 0; i <= TMPSIZE; i++)
    {
        char *find = strchr(goal, '\n') ; // 最後一行讀入的資料中若有換行 就消掉
        if (find)
        {
            gotline-- ;
            break ;
        }
    }
    delete [] goal ;

    cout << "Input file:\n\n" << "line:" << gotline << "\n\n" ;

    Data *dataPtr = new Data[gotline] ;
    char *tmp = new char[TMPSIZE] ;

    fseek(ifptr, 100, SEEK_SET) ;
    fscanf(ifptr, "%*[^\n]") ;

    char garbage ;
    char ch ;
    int tabNum = 0 ;
    int j ;

    for ( int i = 0 ; i < gotline ; i++)
    {
        while ((garbage = fgetc(ifptr)) != '\t')
            ; // skip school code and '\t'

        fscanf(ifptr, "%s", tmp) ; // 讀入校名 (*char)
        ((dataPtr+i)->schoolName).assign(tmp) ; // 存入校名(轉換*char->string)

        while (tabNum < 2) // 跳過科系代碼
        {
            garbage = fgetc(ifptr) ;
            if (garbage == '\t')
                tabNum++ ;
        }

        fscanf(ifptr, "%s", tmp) ; // 讀入科系(*char)
        ((dataPtr+i)->department).assign(tmp) ; // 存入科系(轉換*char->string)

        garbage = fgetc(ifptr) ;

        ch = fgetc(ifptr) ; // 讀日間進修別 包含'\t'
        for ( j = 0; ch != '\t'; j++)
        {
            *(tmp+j) = ch ;
            ch = fgetc(ifptr) ;

        }
        *(tmp+j) = '\0' ;
        ((dataPtr+i)->type).assign(tmp) ; // 存入日間進修別

        ch = fgetc(ifptr) ; // 讀等級別 包含'\t'
        for ( j = 0; ch != '\t'; j++)
        {
            *(tmp+j) = ch ;
            ch = fgetc(ifptr) ;

        }
        *(tmp+j) = '\0' ;
        ((dataPtr+i)->degree).assign(tmp) ; // 存入等級別

        fscanf(ifptr, "%d", &(dataPtr+i)->stuNum) ; // 讀學生數 之後存入

        fscanf(ifptr, "%d", &(dataPtr+i)->teachNum) ; // 讀教師數 之後存入

        fscanf(ifptr, "%d", &(dataPtr+i)->graStu) ; // 讀畢業生數 之後存入

        cout << "[" << std::left << setw(3) << i+1 << "]" << " "
             << std::left << setw(20) << (dataPtr+i)->schoolName << "\t" // 印出7個項目
             << std::left << setw(20) << (dataPtr+i)->department << "\t"
             << (dataPtr+i)->type << "\t"
             << (dataPtr+i)->degree << "\t"
             << (dataPtr+i)->stuNum << "\t"
             << (dataPtr+i)->teachNum << "\t"
             << (dataPtr+i)->graStu << endl ;

        fscanf(ifptr, "%*[^\n]") ; // 跳過該行
        tabNum = 0 ; // initialize
    }

    cout << "\n" ;
    delete [] tmp ;
    fclose(ifptr) ;

    node *treeRootGra ;
    treeRootGra = createTreeGra(dataPtr, gotline) ;

    /// 以下讀模式
    int mode ;
    int value, count = 0 ;
    string schName ;

    cout << endl
         << "************************************************************" << "\n"
         << "*  [1]. Delete node that smaller or equal                  *" << "\n"
         << "        to key in value.                                   *" << "\n"
         << "*  [0]. Quit search                                        *" << "\n"
         << "************************************************************" << "\n"
         << "Choose a mode:" ;
    cin >> mode ;

    while (1)
    {
        if (mode == 1)
        {
            cout << "Please enter a value:" ;
            cin >> value ;
            cout << "Search result:\n" ;
            cout << "      學校名稱\t\t\t科系名稱          日間/進修別   等級別  學生數  教師數  上學年度畢業生人數" << endl ;

            bool esc = true, empty = false ;
            deleteNode(treeRootGra, value, count, esc, empty ) ;
            while (esc)
            {
                node *temp = treeRootGra ;

                if (treeRootGra->right != NULL)
                    treeRootGra = treeRootGra->right ;
                else if (treeRootGra->right == NULL && treeRootGra->left != NULL)
                    treeRootGra = treeRootGra->left ;
                else if (empty)
                {

                    treeRootGra = NULL ;
                    treeRootGra->parent = NULL ;

                    break ;

                }

                treeRootGra->parent = NULL ;

                temp->adata = NULL ;
                temp->left = NULL ;
                temp->right = NULL ;
                delete temp ;

                deleteNode(treeRootGra, value, count, esc, empty ) ;


            }

            if (empty)
            {
                cout << "The Hights:" << endl ;
                cout << "{Number of graduates} = " << 0 << endl ;
                cout << "the tree is empty!! ESCAPE." ;
                return true ;
            }
            else
            {
                cout << "The Hights:" << endl ;
                cout << "{Number of graduates} = " << height(treeRootGra) << endl ;
            }


            count = 0 ;

        }

        if (mode == 0)
            break ;

        cout << endl
             << "************************************************************" << "\n"
             << "*  [1]. Delete node that smaller or equal                  *" << "\n"
             << "        to key in value.                                   *" << "\n"
             << "*  [0]. Quit search                                        *" << "\n"
             << "************************************************************" << "\n"
             << "Choose a mode:" ;
        cin >> mode ;
        count = 0 ;

        while (!(mode == 1 || mode == 0))
        {
            cout << "please enter a correct selection." ;
            cout << endl
                 << "************************************************************" << "\n"
                 << "*  [1]. Delete node that smaller or equal                  *" << "\n"
                 << "        to key in value.                                   *" << "\n"
                 << "*  [0]. Quit search                                        *" << "\n"
                 << "************************************************************" << "\n"
                 << "Choose a mode:" ;
            cin >> mode ;
        }
    }



    return true ;

}

node *createTreeGra(Data *dataPointer, int dataSize)
{
    node *root = new node ; // 紀錄根指標
    root->keyNum = dataPointer->graStu ;
    root->adata = dataPointer ;
    root->left = NULL ;
    root->right = NULL ;
    root->parent = NULL ;

    int treeHigh = 1 ;

    node *cur = root ; // 走訪用指標

    for (int i = 1; i < dataSize; i++) // 插入第一筆外的資料進樹裡
    {
        insertNode((dataPointer+i)->graStu, (dataPointer+i), cur, root, treeHigh) ;
        cur = root ;
    }

    cout << "The Hights:" << endl ;
    cout << "{Number of graduates} = " << treeHigh << endl ;

    return root ;
}

void insertNode(int keyValue, Data *DataPtr, node *currentPtr, node *rootPtr, int &treeHigh)
{
    node *newNode = new node ; // 先建立一個空節點
    newNode->keyNum = 0 ;
    newNode->left = NULL ;
    newNode->right = NULL ;

    while (1)
    {
        if (keyValue < currentPtr->keyNum && currentPtr->left == NULL ) // 輸入值比當前指標鍵值小 且不存在左子樹
        {
            currentPtr->left = newNode ; // 當前節點的左子節點即為該值正確位置
            newNode->keyNum = keyValue ;
            newNode->adata = DataPtr ;
            newNode->parent = currentPtr ;

            break ;
        }
        else if (keyValue < currentPtr->keyNum && currentPtr->left != NULL ) // 輸入值比當前指標鍵值小 且存在左子樹
        {
            currentPtr = currentPtr->left ; // 繼續往左子樹找空節點
        }
        else if (keyValue >= currentPtr->keyNum && currentPtr->right == NULL) // 輸入值>=當前指標鍵值 且不存在右子樹
        {
            currentPtr->right = newNode ; // 當前節點的右子節點即為該值正確位置
            newNode->keyNum = keyValue ;
            newNode->adata = DataPtr ;
            newNode->parent = currentPtr ;

            break ;
        }
        else if (keyValue >= currentPtr->keyNum && currentPtr->right != NULL) // 輸入值>=當前指標鍵值大 且存在右子樹
        {
            currentPtr = currentPtr->right ; // 繼續往右子樹找空節點
        }
    }

    treeHigh = height(rootPtr) ;
}

int height(node* anode)
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

void deleteNode(node *root, int value, int &count, bool &esc, bool &empty)
{
    node *cur = root ;
    esc = false ;

    if (cur != NULL)
    {
        deleteNode(cur->left, value, count, esc, empty) ;
        deleteNode(cur->right, value, count, esc, empty) ;

        if (cur->keyNum <= value) // 找小於等於輸入值的節點
        {
            count++ ;
            cout << "[" << std::left << setw(3) << count << "]" << " "
                 << std::left << setw(20) << cur->adata->schoolName << "\t" // 印出7個項目
                 << std::left << setw(20) << cur->adata->department << "\t"
                 << cur->adata->type << "\t"
                 << cur->adata->degree << "\t"
                 << cur->adata->stuNum << "\t"
                 << cur->adata->teachNum << "\t"
                 << cur->adata->graStu << endl ;

            /// 以下判斷刪除case

            if (cur->right == NULL && cur->left == NULL)
            { // cur沒有左右節點

                if (cur->parent == NULL) // 刪除的是根 且 已經沒有子樹
                {
                    cur = NULL ;
                    empty = true ;
                    return ;
                }

                if (cur->parent->left == cur)
                {
                    cur->parent->left = NULL ;
                    cur->adata = NULL ;
                    cur->left = NULL ;
                    cur->right = NULL ;
                }
                else
                {
                    cur->parent->right = NULL ;
                    cur->adata = NULL ;
                    cur->left = NULL ;
                    cur->right = NULL ;
                }

                delete cur;

            } // end case 1
            else if ((cur->right == NULL && cur->left != NULL) ||
                    (cur->right != NULL && cur->left == NULL))
            { // cur有一個節點

                if (cur->left != NULL)
                {
                    cur->parent->left = cur->left ;
                    cur->adata = NULL ;
                    cur->left = NULL ;
                    cur->right = NULL ;
                }
                else
                {
                    if(cur->parent != NULL)
                    {
                        cur->parent->left = cur->right ;
                        cur->adata = NULL ;
                        cur->left = NULL ;
                        cur->right = NULL ;
                        delete cur ;
                    }
                    else // 刪除的是根
                    {
                        esc = true ;
                        return ;
                    }
                }

            } // end case 2
            else
            { // cur有兩個節點

                node *tmp = cur->left ;

                if (tmp->right != NULL)
                {
                    while (tmp->right != NULL) // find the most in left tree
                    {
                        tmp = tmp->right ;
                    }

                    cur->parent->left = tmp ;

                    cur->adata = NULL ;
                    cur->left = NULL ;
                    cur->right = NULL ;
                    delete cur ;
                }
                else if ((tmp->left != NULL) && (tmp->right == NULL))
                {
                    tmp = cur->right ;
                    while (tmp->left != NULL) // find the smallest in right tree
                    {
                        tmp = tmp->left ;
                    }

                    cur->parent->right = tmp ;

                    cur->adata = NULL ;
                    cur->left = NULL ;
                    cur->right = NULL ;
                    delete cur ;
                }
                else
                {
                    cur->parent->left = tmp ;
                    cur->adata = NULL ;
                    cur->left = NULL ;
                    cur->right = NULL ;
                    delete cur ;
                }
            } // end case 3


        }

    }


}


