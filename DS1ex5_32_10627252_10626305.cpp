// 10626305 魏子翔 10627252徐綺柔
#include<iostream>
#include<cstdio>
#include<string>
#include<windows.h>

#define NAMESIZE 13
#define TMPSIZE 200

using namespace std ;

class Data{

public:
    string lineData ;
    int graStu ;


    void bubbleSort(Data arr[], int n) ;
    void selectionSort(Data arr[], int n) ;
    int indexOfSmallest(Data arr[], int size) ;
    void mergeSort(Data arr[], int first, int last) ;
    void merge(Data arr[], int first, int mid, int last) ;
    void quickSort(Data arr[], int first, int last) ;
    void partition(Data arr[], int first, int last, int &pivotIndex) ;
    void radixSort(Data arr[], int first, int last) ;

};

bool DataInput (int choice, char filename[]) ;

int main()
{
    int mode ;
    int choose ;

    cout << "****************************" << "\n"
         << "*  1 . Execute sort        *" << "\n"
         << "* Any. Quit                *" << "\n"
         << "****************************" << "\n"
         << "Choose a mode:" ;
    cin >> mode ;

    while (mode == 1)
    {
        cout << "Please enter file number(ex: 501, 502, xxx):" ;
        cin >> choose ;
        char filename[NAMESIZE] ;

        if (!DataInput(choose, filename))
            cout << "Please enter correct number(ex: 501, 502, xxx)\n" ;

        cout << "****************************" << "\n"
             << "*  1 . Execute sort        *" << "\n"
             << "* Any. Quit                *" << "\n"
             << "****************************" << "\n"
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

    fseek(ifptr, 0, SEEK_SET) ;
    fgets(tmp, 200, ifptr) ;
    fgets(tmp, 200, ifptr) ;
    fgets(tmp, 200, ifptr) ;

    for ( int i = 0 ; i < gotline ; i++)
    {
        fgets(tmp, 200, ifptr) ;

        char *find = strchr(tmp, '\n') ; // 找換行字元
        if(find)
            *find = '\0'; // 刪除換行字元

        ((dataPtr+i)->lineData).assign(tmp) ; // 轉換*char->string
    }
    cout << "\n" ;


    delete [] tmp ;

    fseek(ifptr, 100, SEEK_SET) ;
    fscanf(ifptr, "%*[^\n]") ;

    char garbage ;
    int tabNum = 0 ;

    for ( int i = 0 ; i < gotline ; i++)
    {
        while (tabNum <= 7) // 跳過前7筆資料
        {
            garbage = fgetc(ifptr) ;
            if (garbage == '\t')
                tabNum++ ;
        }

        fscanf(ifptr, "%d", &(dataPtr+i)->graStu) ; // 存入數字

        fscanf(ifptr, "%*[^\n]") ; // 跳過該行
        tabNum = 0 ;
    }

    fclose(ifptr) ;

    /// 宣告複製指標
    Data *bubPtr = new Data[gotline] ;
    for(int i = 0; i < gotline; i++)
    {
         (bubPtr+i)->graStu = (dataPtr+i)->graStu ;
         (bubPtr+i)->lineData = (dataPtr+i)->lineData ;
    }

    Data *selePtr = new Data[gotline] ;
    for(int i = 0; i < gotline; i++)
    {
         (selePtr+i)->graStu = (dataPtr+i)->graStu ;
         (selePtr+i)->lineData = (dataPtr+i)->lineData ;
    }

    Data *mergePtr = new Data[gotline] ;
    for(int i = 0; i < gotline; i++)
    {
         (mergePtr+i)->graStu = (dataPtr+i)->graStu ;
         (mergePtr+i)->lineData = (dataPtr+i)->lineData ;
    }

    Data *quickPtr = new Data[gotline] ;
    for(int i = 0; i < gotline; i++)
    {
         (quickPtr+i)->graStu = (dataPtr+i)->graStu ;
         (quickPtr+i)->lineData = (dataPtr+i)->lineData ;
    }

    Data *radixPtr = new Data[gotline] ;
    for(int i = 0; i < gotline; i++)
    {
         (radixPtr+i)->graStu = (dataPtr+i)->graStu ;
         (radixPtr+i)->lineData = (dataPtr+i)->lineData ;
    }

    /// 時間量測參數與函數
    LARGE_INTEGER startt, endt, freq ;
    QueryPerformanceFrequency(&freq) ;

    /// 泡泡排序
    QueryPerformanceCounter(&startt) ; // bubbleSort sort time start
    bubPtr->bubbleSort(bubPtr, gotline - 1) ;
    QueryPerformanceCounter(&endt) ; // sort time end
    double sorttime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    /// 以下寫檔
    char fileout[20] ;
    sprintf(fileout, "bubble_sort%d.txt", choice) ;
    FILE *bofp = fopen(fileout, "w");

    for ( int i = 0 ; i < gotline ; i++)
    {
        const char *S = (bubPtr+i)->lineData.c_str() ;
        fprintf(bofp, "%s\n", S ) ;
    }
    fprintf(bofp, "\nBubble Sort:%.4lf ms", sorttime ) ;

    fclose(bofp) ;
    delete [] bubPtr ;
    /// 結束寫檔

    ///===============================================================================
    /// 選擇排序
    QueryPerformanceCounter(&startt) ; // selectionSort sort time start
    selePtr->selectionSort(selePtr, gotline ) ;
    QueryPerformanceCounter(&endt) ; // sort time end
    sorttime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    /// 以下寫檔
    fileout[20] ;
    sprintf(fileout, "select_sort%d.txt", choice) ;
    FILE *sofp = fopen(fileout, "w");

    for ( int i = 0 ; i < gotline ; i++)
    {
        const char *S = (selePtr+i)->lineData.c_str() ;
        fprintf(sofp, "%s\n", S ) ;
    }
    fprintf(sofp, "\nSelection Sort:%.4lf ms", sorttime ) ;

    fclose(sofp) ;
    delete [] selePtr ;
    /// 結束寫檔

    ///===============================================================================
    /// 合併排序
    QueryPerformanceCounter(&startt) ; // mergeSort sort time start
    mergePtr->mergeSort(mergePtr, 0, gotline - 1) ;
    QueryPerformanceCounter(&endt) ; // sort time end
    sorttime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    /// 以下寫檔
    fileout[20] ;
    sprintf(fileout, "merge_sort%d.txt", choice) ;
    FILE *mofp = fopen(fileout, "w");

    for ( int i = 0 ; i < gotline ; i++)
    {
        const char *S = (mergePtr+i)->lineData.c_str() ;
        fprintf(mofp, "%s\n", S ) ;
    }
    fprintf(mofp, "\nmerge Sort:%.4lf ms", sorttime ) ;

    fclose(mofp) ;
    delete [] mergePtr ;
    /// 結束寫檔
    ///===============================================================================

    /// 快速排序
    QueryPerformanceCounter(&startt) ; // quickSort sort time start
    quickPtr->quickSort(quickPtr, 0, gotline - 1) ;
    QueryPerformanceCounter(&endt) ; // sort time end
    sorttime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    /// 以下寫檔
    fileout[20] ;
    sprintf(fileout, "quick_sort%d.txt", choice) ;
    FILE *qofp = fopen(fileout, "w");

    for ( int i = 0 ; i < gotline ; i++)
    {
        const char *S = (quickPtr+i)->lineData.c_str() ;
        fprintf(mofp, "%s\n", S ) ;
    }
    fprintf(qofp, "\nquick Sort:%.4lf ms", sorttime ) ;

    fclose(qofp) ;
    delete [] quickPtr ;
    /// 結束寫檔
    ///===============================================================================

    /// 基數排序
    QueryPerformanceCounter(&startt) ; // radixSort sort time start
    radixPtr->radixSort(radixPtr, 0, gotline) ;
    QueryPerformanceCounter(&endt) ; // sort time end
    sorttime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    /// 以下寫檔
    fileout[20] ;
    sprintf(fileout, "radix_sort%d.txt", choice) ;
    FILE *rofp = fopen(fileout, "w");

    for ( int i = 0 ; i < gotline ; i++)
    {
        const char *S = (radixPtr+i)->lineData.c_str() ;
        fprintf(rofp, "%s\n", S ) ;
    }
    fprintf(rofp, "\nradix Sort:%.4lf ms", sorttime ) ;

    fclose(rofp) ;
    delete [] radixPtr ;
    /// 結束寫檔
    ///===============================================================================

    delete [] dataPtr ;
    cout << "the file has been sorted...\n" << endl ;
    return true ;

}

void Data::bubbleSort(Data arr[], int n)
{
    bool sorted = false ;
    for( int j = 0; j < n && !sorted; j++)
    {
        sorted = true ;
        for(int i = 0; i < n-j; i++)
        {
            if(arr[i].graStu < arr[i+1].graStu) // 確認已排序就提前結束
            {
                Data tmp = arr[i] ;
                arr[i] = arr[i+1] ;
                arr[i+1] = tmp ;
                sorted = false ;
            }
        }
    }
}

void Data::selectionSort(Data arr[], int n)
{
    int last ;
    for(last = n-1; last > 0; last--)
    {
        int samllest = indexOfSmallest(arr, last+1) ;
        if ( samllest != last ) // 未排序最小的值的index位置與目前的最後一個位置交換
        {
            Data tmp = arr[samllest] ;
            arr[samllest] = arr[last] ;
            arr[last] = tmp ;
        }
    }
}

int Data::indexOfSmallest(Data arr[], int size) // 找到Data中 未排序中最小的graStu值
{
    int indexSoFar = 0 ;
    for (int index = 1; index < size; index++)
    {
        if (arr[indexSoFar].graStu >= arr[index].graStu)
            indexSoFar = index ;
    }

    return indexSoFar ;
}

void Data::mergeSort(Data arr[], int first, int last)
{
    if(first < last)
    {
        int mid = (first + last ) / 2 ;
        mergeSort(arr, first, mid) ; // 分組 左側遞迴
        mergeSort(arr, mid+1, last) ; // 分組 右側遞迴
        merge(arr, first, mid, last) ; // 合併元素
    }

}

void Data::merge(Data arr[], int first, int mid, int last)
{

    int lenA = mid - first + 1 ;
    int lenB = last - (mid + 1) + 1 ;
    Data tmpA[lenA] ;
    Data tmpB[lenB] ;

    int i, j, k ; // i for tmpA, j for tmpB

    for ( i = 0; i < lenA; i++)
        tmpA[i] = arr[first+i] ;

    for ( j = 0; j < lenB; j++)
        tmpB[j] = arr[mid+1+j] ;

    i = 0, j = 0 ;
    k = first ;

    while (i < lenA && j < lenB)
    {
        if (tmpA[i].graStu >= tmpB[j].graStu)
        {
            arr[k] = tmpA[i] ;
            i++ ;
        }
        else
        {
            arr[k] = tmpB[j] ;
            j++ ;
        }

        k++ ;
    }

    while (i < lenA) // 左側還有剩 將左側陣列直接串在以排序的後面
        arr[k++] = tmpA[i++] ;

    while (j < lenB) // 右側還有剩 將右側陣列直接串在以排序的後面
        arr[k++] = tmpB[j++] ;


}

void Data::partition(Data arr[], int first, int last, int &pivotIndex) // 分組
{
    Data pivot = arr[first] ;
    Data tmp ;
    int lastS1 = first ;
    int firstUnknown = first + 1 ;

    while (firstUnknown <= last )
    {
        if (arr[firstUnknown].graStu > pivot.graStu) // 只要當前的arr[firstUnknown].graStu
        {                                            // 大於當前的pivot 就將這個值將入到S
            lastS1++ ;
            tmp = arr[firstUnknown] ;
            arr[firstUnknown] = arr[lastS1] ;
            arr[lastS1] = tmp ;
        }
        firstUnknown++ ;

    }

    tmp = arr[first] ;
    arr[first] = arr[lastS1] ;
    arr[lastS1] = tmp ;
    pivotIndex = lastS1 ;

}

void Data::quickSort(Data arr[], int first, int last)
{
    int pivotIndex ;
    if (first < last)
    {
        partition(arr, first, last, pivotIndex) ; // 找出pivot 比他大的數就移到左邊
        quickSort(arr, first, pivotIndex-1) ; // 左側遞迴
        quickSort(arr, pivotIndex+1, last) ; // 右側遞迴
    }
}

void Data::radixSort(Data arr[], int first, int last)
{
    int i, base = 1 ;
    Data temp[last+1] ;
    for (i = 0; i < last; i++)
        if (arr[i].graStu > first)
            first = arr[i].graStu ;
    while ( first/base > 0)
    {
        int bucket[10] = {0} ;

        for (i = 0; i < last; i++) // counting
            bucket[9-arr[i].graStu / base%10]++ ;

        for (i = 1; i < 10; i++) // the start of each group
            bucket[i] += bucket[i-1] ;

        for (i = last-1; i >= 0; i--)
            temp[--bucket[9-arr[i].graStu / base%10]] = arr[i] ;

        for (i = 0; i < last; i++)
            arr[i] = temp[i] ;

        base *= 10 ;
    }
}
