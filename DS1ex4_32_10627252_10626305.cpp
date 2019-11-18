/// 10626305 魏子翔 ,10627252 徐綺柔
/*main @line 55*/
#include <iostream>
#include <cstdio>
#include <windows.h>

#define NAMESIZE 13
#define QUEUESIZE 3

using namespace std ;


class simulation{

public:
    int id ;
    int arrive ;
    int duration ;
    int timeout ;
    int departure ;
    int abort ;
    int delay ;

};

class queue{

public:
    queue(){backPtr = NULL ; frontPtr = NULL ; nodeCount = 0 ;}
    queue(const queue &newq) ;

    bool isEmpty() ;
    bool isFull() ;
    void enqueue(simulation &newItem) ;
    void dequeue() ;
    // simulation getFront(simulation &queueFront) ;
    simulation getFront() ;

    int nodeCount ; // 紀錄queue內有幾筆資料

private:
    struct QueueNode{

        simulation item ;
        QueueNode *next ;
    };
    QueueNode *backPtr ;
    QueueNode *frontPtr ;
};

bool mission(char filename[], int choose) ; // 執行任務一
void Shellsort(simulation arr[], int num) ; // 比1.到達時間 2.ID大小
void Printlist(simulation *ptr, int size , int choose, double rTime, double sTime) ;
void cpuSimulation(simulation *ptr, int listsize) ; // 執行任務二

int main(){

    cout << "Choose a mode:" << "\n"
         << "1.Execute." << "\n"
         << "AnyPress.Quit." << "\n" ;

    int mode ;
    cin >> mode ;

    while( mode == 1 ){

    char filename[NAMESIZE] ;
    int choose ;

    cout << "please enter a number of file.(401,402,XXX...):" ;
    cin >> choose ;

    if(!(mission(filename, choose)))
        cout << "Not found the file!\n\n" ;

    cout << "Choose a mode:" << "\n"
         << "1.Execute." << "\n"
         << "AnyPress.Quit." << "\n" ;
    cin >> mode ;
    }

    cout << "End." ;
    return 0;
}

bool mission(char filename[], int choose)
{
    LARGE_INTEGER startt, endt, freq ;

    sprintf(filename, "input%d.txt", choose) ;
    FILE *ifp = fopen(filename, "r") ;

    QueryPerformanceFrequency(&freq) ;
    QueryPerformanceCounter(&startt) ; // read time start

    if(!ifp)
      return false ;

    fscanf(ifp, "%*[^\n]") ;
    int gotline = 0 ;
    char goal ;

    while(!feof(ifp)) // 計算行數
    {
		goal = fgetc(ifp) ;
		if(goal == '\n')
            gotline++ ;
	}
    gotline-- ;

    cout << "Input file:\n\n" << "line:" << gotline << "\n\n" ;
    fseek(ifp, 0, SEEK_SET) ; // 重讀一次
    fscanf(ifp, "%*[^\n]") ; // 跳過第一行

    int i ;
    simulation *ptr = new simulation[gotline] ;

    cout << "OID	Arrival		Duration	TimeOut\n" ;

    for( i = 0 ; i < gotline ; i++)
    {
        fscanf(ifp, "%d%d%d%d", &(ptr+i)->id, &(ptr+i)->arrive, &(ptr+i)->duration, &(ptr+i)->timeout) ;
        cout << (ptr+i)->id << "\t"
             << (ptr+i)->arrive << "\t\t"
             << (ptr+i)->duration << "\t\t"
             << (ptr+i)->timeout << "\n" ;
    }

    fclose(ifp);
    QueryPerformanceCounter(&endt) ; // read time end

    double readtime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    QueryPerformanceCounter(&startt) ; // sort time start
    Shellsort(ptr, gotline) ;
    QueryPerformanceCounter(&endt) ; // sort time end

    double sorttime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    Printlist(ptr, gotline, choose, readtime , sorttime) ;

    /// ptr 指向已排序的資料起點
    /// 由此開始執行任務二

    cpuSimulation(ptr, gotline) ;

    delete [] ptr ;
    return true ;
}

void Shellsort(simulation arr[], int num)
{
    int i, j, k ;
    int gap ;
    simulation tmp ;

    for( gap = num / 2  ; gap > 0 ; gap /= 2 )
    {
        for( i = gap ; i < num ; i++ ) // insertion sort
        {
            for(j = i + gap ; j < num ; j += gap)
            {
                if(arr[j].arrive < arr[j - gap].arrive) // 優先條件
                {
                    tmp = arr[j] ;
                    k = j - gap ;
                    while(k >= 0 && arr[k].arrive > tmp.arrive )
                    {
                        arr[k+gap] = arr[k] ;
                        k -= gap ;
                    }
                    arr[k+gap] = tmp ;
                }
                else if(arr[j].arrive == arr[j - gap].arrive) // 次要條件
                {
                    tmp = arr[j] ;
                    k = j - gap ;
                    while(k >= 0 && arr[k].id > tmp.id )
                    {
                        arr[k+gap] = arr[k] ;
                        k -= gap ;
                    }
                    arr[k+gap] = tmp ;
                }
            }
        }
    }

}


void Printlist(simulation *ptr, int size , int choose, double rTime, double sTime)
{
    LARGE_INTEGER startt, endt, freq ;
    QueryPerformanceFrequency(&freq) ;


    char filename[NAMESIZE] ;
    sprintf(filename, "sorted%d.txt", choose) ;
    FILE *ofp = fopen(filename, "w");
    QueryPerformanceCounter(&startt) ; // write time start

    /// <檔案內容
    fprintf(ofp, "OID	Arrival		Duration	TimeOut\n") ;
    for(int i = 0 ; i < size ; i++)
    {
        fprintf(ofp, "%d\t %d\t\t %d\t\t %d\n", (ptr+i)->id, (ptr+i)->arrive, (ptr+i)->duration, (ptr+i)->timeout) ;
    }
    QueryPerformanceCounter(&endt) ; // write time end

    double wTime = (double)(endt.QuadPart - startt.QuadPart) / freq.QuadPart * 1000 ;

    fprintf(ofp, "\n"
                 "Reading data: %.4lf ms\n" // 紀錄時間輸出
                 "Sorting data: %.4lf ms\n"
                 "Writing data: %.4lf ms"
                 , rTime, sTime, wTime) ;
    /// 檔案內容>

    fclose(ofp) ;
    cout << "\n...The file has been successfully written.\n" ;
}

bool queue::isEmpty()
{
    return backPtr == NULL ;
}

bool queue::isFull()
{
    return nodeCount == QUEUESIZE ;
}

void queue::enqueue(simulation &newItem)
{
    QueueNode *newPtr = new QueueNode ;
    newPtr->item = newItem ;
    newPtr->next = NULL ;
    nodeCount++ ;
    if(isEmpty())
        frontPtr = newPtr ;
    else
        backPtr->next = newPtr ;

    backPtr = newPtr ;
}

void queue::dequeue()
{
    QueueNode *tempPtr = frontPtr ;
    if(frontPtr == backPtr)
    {
        frontPtr = NULL ;
        backPtr = NULL ;
    }
    else
        frontPtr = frontPtr->next ;

    tempPtr->next = NULL ;
    delete tempPtr ;
    nodeCount-- ;
}

simulation queue::getFront()
{
    if(isEmpty())
        ;// 任務二更改
    else
        return frontPtr->item ;
}

void cpuSimulation(simulation *ptr, int listsize) // 接收的 ptr 指向已排序的資料起點
{
    queue waitQueue ; // 要處理的清單
    queue donelist ;
    queue abtlist ;
    int timeCount = 0 ; // 時間計數器
    int itemCount = 1 ; // 第幾筆資料
    int biginToDo = 0 ; // 開始執行工作時間

    /// sortedlist.enqueue(*(ptr++)) ; // 新增範例
    /// sortedlist.dequeue(); // 移除範例
    /// (sortedlist.getFront(*(ptr))).id // 取得特定(id,arrive...)資訊


    simulation *workingPtr = ptr ;

    while(itemCount <= listsize)
    {
        if(itemCount == 1)
            biginToDo = ptr->arrive ;

        if(timeCount == ptr->arrive) // 當下時間有工作項目到達
        {
            if(waitQueue.isFull()) // 等待佇列已滿
            {
                ptr->abort = ptr->arrive ;
                ptr->delay = 0 ;
                abtlist.enqueue(*ptr) ; // 加入中止清單
                ptr++ ;                 // 下一筆來源清單工作
                itemCount++ ;
            }
            else if(waitQueue.isEmpty()) // 佇列是空的 就將當前項目交給 CPU
            {
                workingPtr = ptr ;
                ptr++ ;
                itemCount++ ;
            }
            else if(!(waitQueue.isFull())) // 等待佇列仍有項目
            {
                waitQueue.enqueue(*ptr) ;
                ptr++ ;
                itemCount++ ;
            }

        }

        if(timeCount == workingPtr->timeout) // CPU 工作中發現項目逾時 就加入abt list
        {
            workingPtr->abort = workingPtr->timeout ;
            workingPtr->delay = timeCount - workingPtr->arrive ;
            abtlist.enqueue(*workingPtr) ;
            *workingPtr = waitQueue.getFront() ; // 下一筆工作
            waitQueue.dequeue() ;
        }

        if((timeCount == (workingPtr->arrive + workingPtr->duration)) // cpu在工作逾時前完成工作
           && ((workingPtr->arrive + workingPtr->duration) <= workingPtr->timeout))
        {
            workingPtr->departure = biginToDo + workingPtr->duration ;
            workingPtr->delay = biginToDo - workingPtr->arrive ;
            donelist.enqueue(*workingPtr) ;

            if(timeCount > (waitQueue.getFront().timeout)) // 取出新工作時 發現該項目已逾時
            {
                simulation *tmpPtr ;
                *tmpPtr = waitQueue.getFront() ;
                waitQueue.dequeue() ;
                tmpPtr->abort = timeCount ;
                tmpPtr->delay = timeCount - tmpPtr->arrive ;
                abtlist.enqueue(*tmpPtr) ;

            }
            else if(timeCount <= (waitQueue.getFront().timeout)) // 取出尚未逾時的新工作
            {
                *workingPtr = waitQueue.getFront() ;
                waitQueue.dequeue() ;
                biginToDo = timeCount ;
            }
        }

        timeCount++ ; // 秒數更新

    }

    cout << "OID\tAbort\tDelay" << "\n" ;

    for(int i = 1 ; i <= listsize ; i++)
    {
        cout << donelist.getFront().id << "\t"
             << donelist.getFront().abort << "\t"
             << donelist.getFront().delay << "\n" ;

        donelist.dequeue() ;
    }


}
