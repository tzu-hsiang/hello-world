// 10626305 魏子翔 10627252 徐綺柔
#include<iostream>
#include<cstdio>
#include<windows.h>
#include<cmath>

#define NAMESIZE 20
#define TMPSIZE 200

using namespace std ;

class Data{

public:
    int itemNum ;
    int graStu ;
};

class Heap{

public:
    Data *adata ;
    int keyValue ;
    Heap *CreateMinHeap(Data *firstDataPtr, int dataNum, int &buttom, int &leftmost) ;

private:
    Heap MinInsertNode(Heap *heapRoot, Data *newItem, int buttom) ;
};

class MinMaxHeap : public Heap{

public:
    int JudgeLevel(int currentNode) ;
    void CheckUpMax(MinMaxHeap *heapRoot, int pos) ;
    void CheckUpMin(MinMaxHeap *heapRoot, int pos) ;
    MinMaxHeap *CreateMinMaxHeap(Data *firstDataPtr, int dataNum, int &buttom, int &leftmost) ;
    MinMaxHeap MinMaxInsertNode(MinMaxHeap *heapRoot, Data *newItem, int buttom) ;
};

void ModeChoose(int mode) ;
Data *SaveData(char *choice, char *filename, int &dataNum) ;
void PrintHeap(int mode, Heap *heapRoot, int buttom, int leftmost) ;
void PrintHeap(int mode, MinMaxHeap *heapRoot, int buttom, int leftmost) ;

int main()
{
    int mode ;

    cout << endl
         << "******************************" << "\n"
         << "*  1 . Create min heap       *" << "\n"
         << "*  2 . Create min-max heap   *" << "\n"
         << "* Any. Quit                  *" << "\n"
         << "******************************" << "\n"
         << "Choose a mode:" ;
    cin >> mode ;

    while (mode == 1 || mode == 2)
    {
        ModeChoose(mode) ;
        cout << endl
             << "******************************" << "\n"
             << "*  1 . Create min heap       *" << "\n"
             << "*  2 . Create min-max heap   *" << "\n"
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
        cout << "Please enter file number(ex: 100, 101, xxx):" ;
        cin >> choose ;

        char *filename = new char[NAMESIZE] ;
        int dataNum = 0 ;
        Data *dataPtr = SaveData(choose, filename, dataNum) ;
        delete [] filename ;

        if (dataPtr == NULL)
        {
            cout << "Please enter correct number(ex: 100, 101, xxx)\n" ;
            delete [] dataPtr ;
        }
        else
        {
            int buttom = 0, leftmost = 0 ;
            Heap *minHeap ;
            minHeap = minHeap->CreateMinHeap(dataPtr, dataNum, buttom, leftmost) ;
            PrintHeap(mode, minHeap, buttom, leftmost) ;
            delete [] minHeap ;
            delete [] dataPtr ;
        }

    }

    if (mode == 2)
    {
        char *choose = new char[3] ;
        cout << "Please enter file number(ex: 100, 101, xxx):" ;
        cin >> choose ;

        char *filename = new char[NAMESIZE] ;
        int dataNum = 0 ;
        Data *dataPtr = SaveData(choose, filename, dataNum) ;
        delete [] filename ;

        if (dataPtr == NULL)
        {
            cout << "Please enter correct number(ex: 100, 101, xxx)\n" ;
            delete [] dataPtr ;
        }
        else
        {
            int buttom = 0, leftmost = 0 ;
            MinMaxHeap *minMaxHeap ;
            minMaxHeap = minMaxHeap->CreateMinMaxHeap(dataPtr, dataNum, buttom, leftmost) ;
            PrintHeap(mode, minMaxHeap, buttom, leftmost) ;
            delete [] minMaxHeap ;
            delete [] dataPtr ;
        }
    }


}

Data *SaveData (char choice[], char filename[], int &dataNum)
{
    sprintf(filename, "input%s.txt", choice) ;
    delete [] choice ;

    FILE *ifptr = fopen(filename, "r") ;

    if (!ifptr)
    {
        cout << "can not found!\n" ;
        return NULL ;
    }

    char *goal = new char[TMPSIZE] ;

    while (!feof(ifptr)) // count line
    {
        fgets(goal, 200, ifptr) ;
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

    Data *dataPtr = new Data[dataNum] ;

    fseek(ifptr, 100, SEEK_SET) ;
    fscanf(ifptr, "%*[^\n]") ;

    char garbage ;
    int tabCount = 0 ;

    for ( int i = 0 ; i < dataNum ; i++)
    {
        (dataPtr + i)->itemNum = i + 1 ; // set serial number

        // skip data and tab before the graduated students
        while (garbage = fgetc(ifptr))
        {
            if (garbage == '\t')
                tabCount++ ;

            if (tabCount == 8)
            {
                tabCount = 0 ; // initialize
                break ;
            }
        }

        fscanf(ifptr, "%d", &(dataPtr+i)->graStu) ;
        fscanf(ifptr, "%*[^\n]") ; // skip [i]th line
    }

    cout << "\n" ;

    fclose(ifptr) ;
    return dataPtr ;

}

Heap *Heap::CreateMinHeap(Data *firstDataPtr, int dataNum, int &buttom, int &leftmost)
{
    int nthData = 0 ;
    Heap *minHeap = new Heap[dataNum] ;

    // one by one insert the node to heap
    while (buttom < dataNum)
    {
        minHeap->MinInsertNode(minHeap, firstDataPtr + nthData, buttom) ;
        nthData++ ;
        buttom++ ;
    }

    buttom-- ;

    // calculate leftmost node
    int root = 0 ;
    leftmost = 2 * root + 1 ;

    while (leftmost <= buttom)
    {
        root = leftmost ;
        leftmost = 2 * root + 1 ;
    }

    leftmost = root ;

    return minHeap ;
}

Heap Heap::MinInsertNode(Heap *heapRoot, Data *newItem, int buttom)
{
    (heapRoot + buttom)->adata = newItem ;
    (heapRoot + buttom)->keyValue = newItem->graStu ;

    int position = buttom ;
    int parent = (position - 1) / 2 ;

    // TODO: swap and compare from child to parent, than renew child and parent.
    while ((parent >= 0) &&
          ((heapRoot + position)->keyValue < (heapRoot + parent)->keyValue))
    {
        Heap temp = *(heapRoot + parent) ;
        heapRoot[parent] = *(heapRoot + position) ;
        heapRoot[position] = temp ;
        position = parent ;
        parent = (position - 1) / 2 ;
    }

}

void PrintHeap(int mode, Heap *heapRoot, int buttom, int leftmost) // for min heap
{
    if (mode == 1)
        cout << "<min heap>\n" ;
    else
        cout << "<min-max heap>\n" ;

    cout << "root: " << "[" << heapRoot->adata->itemNum << "]"
         << heapRoot->keyValue << endl ;

    cout << "bottom: " << "[" << (heapRoot + buttom)->adata->itemNum << "]"
         << (heapRoot + buttom)->keyValue << endl ;

    cout << "leftmost: " << "[" << (heapRoot + leftmost)->adata->itemNum << "]"
         << (heapRoot + leftmost)->keyValue << endl ;

}

void PrintHeap(int mode, MinMaxHeap *heapRoot, int buttom, int leftmost) // for min-max heap
{
    if (mode == 1)
        cout << "<min heap>\n" ;
    else
        cout << "<min-max heap>\n" ;

    cout << "root: " << "[" << heapRoot->adata->itemNum << "]"
         << heapRoot->keyValue << endl ;

    cout << "bottom: " << "[" << (heapRoot + buttom)->adata->itemNum << "]"
         << (heapRoot + buttom)->keyValue << endl ;

    cout << "leftmost: " << "[" << (heapRoot + leftmost)->adata->itemNum << "]"
         << (heapRoot + leftmost)->keyValue << endl ;

}

MinMaxHeap *MinMaxHeap::CreateMinMaxHeap(Data *firstDataPtr, int dataNum, int &buttom, int &leftmost)
{
    int nthData = 0 ;
    MinMaxHeap *minMaxHeap = new MinMaxHeap[dataNum] ;

    // one by one insert the node to heap
    while (buttom < dataNum)
    {
        MinMaxInsertNode(minMaxHeap, firstDataPtr + nthData, buttom) ;
        nthData++ ;
        buttom++ ;
    }

    buttom-- ;

    // calculate leftmost node
    int root = 0 ;
    leftmost = 2 * root + 1 ;

    while (leftmost <= buttom)
    {
        root = leftmost ;
        leftmost = 2 * root + 1 ;
    }

    leftmost = root ;

    return minMaxHeap ;
}

MinMaxHeap MinMaxHeap::MinMaxInsertNode(MinMaxHeap *heapRoot, Data *newItem, int buttom)
{
    (heapRoot + buttom)->adata = newItem ;
    (heapRoot + buttom)->keyValue = newItem->graStu ;

    int position = buttom ;
    int parent = (position - 1) / 2 ;
    int level = heapRoot->JudgeLevel(position + 1) ;

    if (buttom >= 1)
    {
        if (level == 1) // addition at the odd level
        {
            if (heapRoot[position].keyValue > heapRoot[parent].keyValue)
            {
                MinMaxHeap temp = *(heapRoot + parent) ;
                heapRoot[parent] = *(heapRoot + position) ;
                heapRoot[position] = temp ;
                position = parent ;
                heapRoot->CheckUpMax(heapRoot, position) ;
            }
            else
            {
                heapRoot->CheckUpMin(heapRoot, position) ;
            }
        }
        else // addition at the even level
        {
            if (heapRoot[position].keyValue < heapRoot[parent].keyValue)
            {
                MinMaxHeap temp = *(heapRoot + parent) ;
                heapRoot[parent] = *(heapRoot + position) ;
                heapRoot[position] = temp ;
                position = parent ;
                heapRoot->CheckUpMin(heapRoot, position) ;
            }
            else
            {
                heapRoot->CheckUpMax(heapRoot, position) ;
            }
        }
    }

}

void MinMaxHeap::CheckUpMax(MinMaxHeap *heapRoot, int pos)
{
    int parent = (pos - 1) / 2 ;
    int grandparent = (parent - 1) / 2 ;

    if (pos >= 3 && // pos have a grandparent
        heapRoot[pos].keyValue > heapRoot[grandparent].keyValue)
    {
        MinMaxHeap temp = *(heapRoot + grandparent) ;
        heapRoot[grandparent] = *(heapRoot + pos) ;
        heapRoot[pos] = temp ;
        heapRoot->CheckUpMax(heapRoot, grandparent) ;
    }
}

void MinMaxHeap::CheckUpMin(MinMaxHeap *heapRoot, int pos)
{
    int parent = (pos - 1) / 2 ;
    int grandparent = (parent - 1) / 2 ;

    if (pos >= 3 && // pos have a grandparent
        heapRoot[pos].keyValue < heapRoot[grandparent].keyValue)
    {
        MinMaxHeap temp = *(heapRoot + grandparent) ;
        heapRoot[grandparent] = *(heapRoot + pos) ;
        heapRoot[pos] = temp ;
        heapRoot->CheckUpMin(heapRoot, grandparent) ;
    }
}

int MinMaxHeap::JudgeLevel(int currentNode)
{
    double level = log10(currentNode) / log10(2.0) + 1 ;
    int judge = (int)level % 2 ;

    if (judge != 0) // it's odd level
        return 1 ;
    else            // it's even level
        return 2 ;

}
