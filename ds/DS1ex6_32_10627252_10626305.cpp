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

};


bool DataInput(int choice, char filename[]) ;
node *createTreeGra(Data *dataPointer, int dataSize) ;
void insertNode(int keyValue, Data *DataPtr, node *currentPtr, node *rootPtr, int &treeHigh) ;
node *createTreeItm(Data *dataPointer, int dataSize) ;
int height(node* anode) ;
void inorderGra(node *root, int value, int &count) ;
void inorderItm(node *root, string schoolNm, int &count, string memo, bool found) ;

int main()
{
    int mode ;
    int choose ;

    cout << endl
         << "******************************" << "\n"
         << "*  1 . Create two trees      *" << "\n"
         << "* Any. Quit                  *" << "\n"
         << "******************************" << "\n"
         << "Choose a mode:" ;
    cin >> mode ;

    while (mode == 1)
    {
        cout << "Please enter file number(ex: 601, 602, xxx):" ;
        cin >> choose ;
        char filename[NAMESIZE] ;

        if (!DataInput(choose, filename))
            cout << "Please enter correct number(ex: 601, 602, xxx)\n" ;

        cout << endl
             << "******************************" << "\n"
             << "*  1 . Create two trees      *" << "\n"
             << "* Any. Quit                  *" << "\n"
             << "******************************" << "\n"
             << "Choose a mode:" ;
        cin >> mode ;

    }



    return 0 ;
}

bool DataInput (int choice, char filename[])
{
    sprintf(filename, "input%d.txt", choice) ;
    FILE *ifptr = fopen( filename, "r" ) ;

    if (!ifptr)
    {
        cout << "can not found!\n" ;
        return false ;
    }

    /// 插入系統選單位置
    int gotline = 0 ;
    char *goal = new char[TMPSIZE] ;

    while (!feof(ifptr)) // 計算行數
    {
        fgets(goal, 200, ifptr) ;
        gotline++ ;
    }
    gotline -= 3 ;

    for( int i = 0; i <= TMPSIZE; i++)
    {
        char *find = strchr(goal, '\n') ;
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

    /// 以下設定編號
    int i = 1 ;
    int mid = (gotline) / 2 + 1 ;

    (dataPtr+0)->itemNum = mid ;
    (dataPtr+1)->itemNum = mid + mid/3 ;
    (dataPtr+2)->itemNum = mid/3 ;
    for ( j = 3; j < gotline; j++)
    {
        if ((dataPtr+j)->schoolName == (dataPtr+j-1)->schoolName)
        {
            (dataPtr+j)->itemNum = (dataPtr+j-1)->itemNum ;
        }
        else if ((dataPtr+j)->schoolName == (dataPtr+0)->schoolName)
        {
            (dataPtr+j)->itemNum = (dataPtr+0)->itemNum ;
        }
        else if ((dataPtr+j)->schoolName == (dataPtr+1)->schoolName)
        {
            (dataPtr+j)->itemNum = (dataPtr+1)->itemNum ;
        }
        else if ((dataPtr+j)->schoolName == (dataPtr+2)->schoolName)
        {
            (dataPtr+j)->itemNum = (dataPtr+2)->itemNum ;
        }
        else
        {
            if (i == (mid + mid/3) || i == mid || i == (mid/3))
            i++ ;

            (dataPtr+j)->itemNum = i ;
            i++ ;
        }

    }
    /// end

    node *treeRootGra ;
    treeRootGra = createTreeGra(dataPtr, gotline) ;

    node *treeRootItm ;
    treeRootItm = createTreeItm(dataPtr, gotline) ;

    /// 以下讀模式
    int mode ;
    int value, count = 0 ;
    string schName ;

    cout << endl
         << "************************************************************" << "\n"
         << "*  [1]. Search greater or equal to key in value.           *" << "\n"
         << "*  [2]. Search school name.                                *" << "\n"
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
            inorderGra(treeRootGra, value, count) ;

            count = 0 ;
        }

        if (mode == 2)
        {
            cout << "Please enter a school name:" ;
            cin >> schName ;
            cout << "Search result:\n" ;
            cout << "      學校名稱\t\t\t科系名稱          日間/進修別   等級別  學生數  教師數  上學年度畢業生人數" << endl ;

            bool found = false ;
            string memo ;
            inorderItm(treeRootGra, schName, count, memo, found) ;

            count = 0 ;
        }

        if (mode == 0)
            break ;

        if (mode == 1 || mode == 2)
        {
            cout << endl
                 << "************************************************************" << "\n"
                 << "*  [1]. Search greater or equal to key in value.           *" << "\n"
                 << "*  [2]. Search school name.                                *" << "\n"
                 << "*  [0]. Quit search                                        *" << "\n"
                 << "************************************************************" << "\n"
                 << "Choose a mode:" ;
            cin >> mode ;
        }

        while (mode != 1 && mode != 2 && mode != 0)
        {
            cout << "please enter a correct selection." ;
            cout << endl
                 << "************************************************************" << "\n"
                 << "*  [1]. Search greater or equal to key in value.           *" << "\n"
                 << "*  [2]. Search school name.                                *" << "\n"
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

node *createTreeItm(Data *dataPointer, int dataSize)
{
    node *root = new node ; // 紀錄根指標
    root->keyNum = dataPointer->itemNum ;
    root->adata = dataPointer ;
    root->left = NULL ;
    root->right = NULL ;

    int treeHigh = 1 ;

    node *cur = root ; // 走訪用指標

    for (int i = 1; i < dataSize; i++) // 插入第一筆外的資料進樹裡
    {
        insertNode((dataPointer+i)->itemNum, (dataPointer+i), cur, root, treeHigh) ;
        cur = root ;
    }

    cout << "{School name} = " << treeHigh << endl ;

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

void inorderGra(node *root, int value, int &count)
{
    node *findPtr = root ;
    if (findPtr != NULL)
    {

        inorderGra(findPtr->right, value, count ) ;

        if (findPtr->keyNum >= value)
        {
            count++ ;
            cout << "[" << std::left << setw(3) << count << "] "
                 << std::left << setw(20) << findPtr->adata->schoolName << "\t" // 印出7個項目
                 << std::left << setw(20) << findPtr->adata->department << "\t"
                 << std::left << setw(5) << findPtr->adata->type << "\t"
                 << findPtr->adata->degree << "\t"
                 << findPtr->adata->stuNum << "\t"
                 << findPtr->adata->teachNum << "\t"
                 << findPtr->adata->graStu << endl ;
        }

        if (findPtr->keyNum >= value)
            inorderGra(findPtr->left, value, count ) ;

    }

}

void inorderItm(node *root, string schoolNm, int &count, string memo, bool found)
{
    node *findPtr = root ;

    if (findPtr != NULL)
    {
        inorderItm(findPtr->right, schoolNm, count, memo, found ) ;

        if (findPtr->adata->schoolName == schoolNm)
        {
            memo = findPtr->adata->schoolName ;
            found = true ;
            count++ ;
            cout << "[" << std::left << setw(3) << count << "] "
                 << std::left << setw(20) << findPtr->adata->schoolName << "\t" // 印出7個項目
                 << std::left << setw(20) << findPtr->adata->department << "\t"
                 << std::left << setw(5) << findPtr->adata->type << "\t"
                 << findPtr->adata->degree << "\t"
                 << findPtr->adata->stuNum << "\t"
                 << findPtr->adata->teachNum << "\t"
                 << findPtr->adata->graStu << endl ;

        }
        if (!found || memo == findPtr->adata->schoolName) ;
            inorderItm(findPtr->left, schoolNm, count, memo, found ) ;

    }
}


