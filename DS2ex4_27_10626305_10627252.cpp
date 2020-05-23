/*
   team:27 member:10626305 魏子翔 10627252 徐綺柔
   mission: adjacency lists, connection counts
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <list>
#include <queue>

#define MAX_LEN 10

using namespace std ;

class DirectedGraph
{
private:
    typedef struct sP
    {
        char senderId[MAX_LEN] ;
        char receiverId[MAX_LEN] ;
        float weight ;
    } studentPair ;

    typedef struct aLNode
    {
        char receiverId[MAX_LEN] ;
        float weight ;
    } adjListNode ;

    typedef struct aL
    {
        char senderId[MAX_LEN] ;
        list<adjListNode> recv ;
        int inf ;
        vector<string> visitedList ;
    } adjList ;

    string fileNum ;
    vector<adjList> adjL ;
    int totalNode ;

    vector<studentPair> readFile(string &fileNum) // read binary file
    {
        vector<studentPair> sList ;
        fstream binFile ;
        studentPair aRelation ;
        int dataLine = 0 ;

        binFile.open(("pairs" + fileNum + ".bin").c_str(), fstream::in|fstream::binary) ;

        if (!binFile.is_open())
        {
            cout << "pairs" << fileNum << ".bin does not exist!\n" ;
            return sList ;
        }
        else
        {
            binFile.seekg(0, binFile.end) ;
            dataLine = binFile.tellg()/sizeof(aRelation) ;
            binFile.seekg(0, binFile.beg) ;

            sList.reserve(dataLine) ;
            sList.resize(dataLine) ;
            for (int i = 0; i < dataLine; i++)
            {
                binFile.read((char *)&aRelation, sizeof(aRelation)) ;
                sList.at(i) = aRelation ;
            }

            binFile.close() ;
            return sList ;
        }
    }

    // insert sender to main array
    // return sorted static main array(vector won't allocate any more memory)
    vector<adjList> insertMain(vector<studentPair> &sList)
    {
        vector<adjList> tmpMain ;
        bool flag = false ;

        for (int i = 0; i < sList.size(); i++)
        {
            flag = false ;

            for (int j = 0; j < tmpMain.capacity(); j++)
            {
                // main array has a same value
                if (!strcmp(tmpMain[j].senderId, sList.at(i).senderId))
                {
                    flag = true ;
                    break ;
                }
            }

            if (!flag)
            {
                adjList tmp ;
                strcpy(tmp.senderId, sList.at(i).senderId) ;
                tmp.inf = 0 ;
                tmpMain.push_back(tmp) ;
            }
        }

        vector<adjList> (tmpMain).swap(tmpMain) ;
        totalNode = sList.size() ;

        return tmpMain ;
    }

    static bool compareIdForList(const adjListNode &first, const adjListNode &second)
    {
        if(strcmp(first.receiverId, second.receiverId) < 0 )
            return true ;
        else
            return false ;
    }

    // link receiver to sender of main array
    void insertRecv(vector<studentPair> &sList, vector<adjList> &adjL)
    {

        for (int i = 0; i < sList.size(); i++)
        {
            for (int j = 0; j < adjL.size(); j++)
            {
                // main array has a same value
                if (!strcmp(adjL.at(j).senderId, sList.at(i).senderId))
                {
                    adjListNode tmp ;
                    strcpy(tmp.receiverId, sList.at(i).receiverId) ;
                    tmp.weight = sList.at(i).weight ;
                    adjL.at(j).recv.push_back(tmp) ;
                }
            }
        }

        // sort list
        for (int i = 0; i < adjL.size(); i++)
           adjL.at(i).recv.sort(compareIdForList) ;

    }

    static bool compareIdForMain(const adjList &first, const adjList &second)
    {
        if(strcmp(first.senderId, second.senderId) < 0 )
            return true ;
        else
            return false ;
    }

    void mainSort(vector<adjList> &adjL)
    {
        sort(adjL.begin(), adjL.end(), compareIdForMain) ;
    }

    void writeFile(vector<adjList> &adjL, string &fileNum)
    {
        // ready to write file
        ofstream outFile ;
        outFile.open(("pairs" + fileNum + ".adj").c_str(), ios::out) ;

        if (!outFile.is_open())
        {
            cout << "Unknown Error!! can not create output file.\n" ;
            cout << "please try again.\n" << endl ;
            return ;
        }

        cout << "\n<<< There are " << adjL.size() << " IDs in total. >>>\n" << endl ;
        outFile << "<<< There are " << adjL.size() << " IDs in total. >>>" << endl ;

        int endLine = 1 ;
        int index = 1 ;

        for (int i = 0; i < adjL.size(); i++)
        {
            outFile << "[" << i+1 << "] " << adjL.at(i).senderId << ":" << endl ;

            list<adjListNode>::const_iterator itr = adjL.at(i).recv.begin();

            outFile << "    " ;
            while(itr != adjL.at(i).recv.end())
            {
                outFile << "(" << setw(2) << index << ")"
                        << setw(10) << itr->receiverId << ", "
                        << setw(5) << itr->weight << " " ;

                if (endLine == 10)
                {
                    outFile << endl ;
                    outFile << "    " ;
                    endLine = 0 ;
                }

                endLine++ ;
                index++ ;
                itr++ ;
            }

            outFile << "\n" ;
            index = 1 ;
            endLine = 1 ;
        } // end for

        cout << "<<< There are " << totalNode << " nodes in total. >>>\n" << endl ;
        outFile << "<<< There are " << totalNode << " nodes in total. >>>" << endl ;
        outFile.close() ;

        cout << "pairs" << fileNum << ".adj" << " is successfully created.\n" << endl ;

    }

    // if vertex is visited, it will return true
    bool checkVisited(vector<string> &visitedList, string searchId)
    {
        if (binary_search(visitedList.begin(), visitedList.end(), searchId))
            return true ;
        else
            return false ;
    }

    // return the index of new start point
    int findStartPoint(vector<adjList> &adjL, string searchId)
    {
        for (int i = 0; i < adjL.size(); i++)
        {
            if (strcmp(adjL[i].senderId, searchId.c_str()) == 0 )
                return i ;
        }
    }

    void mainSortForCount(vector<adjList> &adjL)
    {
        bool sorted = false ;
        for( int j = 0; j < adjL.size()-1 && !sorted; j++)
        {
            sorted = true ;
            for(int i = 0; i < adjL.size()-j-1; i++)
            {
                if(adjL.at(i).inf < adjL.at(i+1).inf)
                {
                    adjList tmp = adjL.at(i) ;
                    adjL.at(i) = adjL.at(i+1) ;
                    adjL.at(i+1) = tmp ;
                    sorted = false ;
                }
            }
        }
    }

    void breadthFirstSearch(vector<adjList> &adjL)
    {
        queue<string> newStartPt ;
        vector<string> visitedList ; // the first data in list is first point
        visitedList.reserve(adjL.size()) ;

        cout << "executing..." ;

        for (int i = 0; i < adjL.size(); i++)
        {
            string startPt ;
            startPt.assign(adjL.at(i).senderId) ;

            newStartPt.push(startPt) ; // new start point
            visitedList.push_back(startPt) ; // record visited vertex

            while (!newStartPt.empty()) // queue still not empty
            {
                // renew search start point
                int searchSt = findStartPoint(adjL, newStartPt.front()) ;

                list<adjListNode>::iterator itr = adjL.at(searchSt).recv.begin();

                while(itr != adjL[searchSt].recv.end())
                {
                    // all receiver push into newStartPt and record these id
                    // if visited, ignore this id
                    string searchId ;
                    searchId.assign(itr->receiverId) ;

                    if (!checkVisited(visitedList, searchId))
                    {
                        newStartPt.push(searchId) ;
                        visitedList.push_back(searchId) ;
                        sort(visitedList.begin(), visitedList.end()) ;
                    }

                    itr++ ;
                }

                newStartPt.pop() ;
            } // end once task

            adjL.at(i).inf = visitedList.size() - 1 ; // -1 for first sender
            sort(visitedList.begin(), visitedList.end()) ;

            adjL.at(i).visitedList = visitedList ;

            visitedList.clear() ;

            float exeTime = (float) i / adjL.size() * 100.0 ;

            cout << setw(3) << (int)exeTime << "%" ;
            cout << "\b\b\b\b" ;
        } // end all task

        mainSortForCount(adjL) ;
        cout << "100%" << endl ;
    } // end BFS

    void writeCount(vector<adjList> &adjL)
    {
        ofstream outFile ;
        outFile.open(("pairs" + fileNum + ".cnt").c_str(), ios::out) ;

        if (!outFile.is_open())
        {
            cout << "Unknown Error!! can not create output file.\n" ;
            cout << "please try again.\n" << endl ;
            return ;
        }

        cout << "\n<<< There are "<< adjL.size() << " IDs in total. >>>\n" << endl ;
        outFile << "<<< There are "<< adjL.size() << " IDs in total. >>>\n" << endl ;

        for (int i = 0; i < adjL.size(); i++)
        {
            outFile << "[" << setw(3) << i+1 << "] " << adjL[i].senderId
                    << "(" << setw(2) << adjL.at(i).inf << "):" << endl ;

            int count = 0, itemIdx = 1 ;
            outFile << "    " ;

            for (int idIndex = 0; idIndex < adjL.at(i).visitedList.size(); idIndex++)
            {
                if (strcmp(adjL.at(i).senderId, (adjL.at(i).visitedList[idIndex]).c_str()) != 0)
                {
                    count++ ;
                    outFile << "(" << setw(2) << itemIdx << ") "
                            << left << setw(10) << adjL.at(i).visitedList[idIndex] << " " ;

                    if (count == 10)
                    {
                        outFile << endl << "    " ;
                        count = 0 ;
                    }

                    itemIdx++ ;
                }

            }

            outFile << endl ; // end write line
        }

        outFile.close() ;

        cout << "pairs" << fileNum << ".cnt is successfully created.\n" << endl ;
    }

public:
    DirectedGraph() {fileNum = "" ; totalNode = 0 ;}
    DirectedGraph(DirectedGraph &obj) {adjL = obj.adjL ; obj.fileNum = "" ; obj.totalNode = 0 ;}
    ~DirectedGraph() {}

    // successful creation will return true
    bool create(string &fileNum)
    {
        if (fileNum.compare(this->fileNum) == 0 )
            return true ;

        vector<studentPair> sList ;
        sList = readFile(fileNum) ;
        if (sList.empty()) // file doesn't exist
            return false ;

        this->fileNum = fileNum ;
        totalNode = sList.size() ;
        adjL = insertMain(sList) ;
        insertRecv(sList, adjL) ;
        mainSort(adjL) ;

        return true ;
    }

    void output()
    {
        writeFile(adjL, fileNum) ;
    }

    void BFS()
    {
        if (adjL.empty())
        {
            cout << "no graph is created!! expect create execute first!\n" << endl ;
            return ;
        }

        breadthFirstSearch(adjL) ;
        writeCount(adjL) ;

    }

    bool none() // check graph is created or not
    {
        if (adjL.empty())
            return true ;
        else
            return false ;
    }

    void release()
    {
        vector<adjList> ().swap(this->adjL) ;
        fileNum = "" ;
        totalNode = 0 ;

        cout << adjL.capacity() << endl ;
    }
} ; // end class

void modeChoose(char *mode) ;

int main()
{
    char mode[] = "1" ;

    while (strcmp(mode, "0") != 0)
    {
        cout << endl ;
        cout << "**** Graph data manipulation ****\n"
             << "* 0. QUIT                       *\n"
             << "* 1. Build adjacency lists      *\n"
             << "* 2. Compute connection counts  *\n"
             << "*********************************\n"
             << "Input a choice(0, 1, 2):" ;
        cin >> mode ;

        modeChoose(mode) ;
    }

    return 0 ;
}

void modeChoose(char *mode)
{
    static DirectedGraph tmp ;

    if (strcmp(mode, "0") == 0)
    {
        cout << "Quit.\n" ;
    }
    else if (strcmp(mode, "1") == 0) // create adjacency lists
    {
        cout << "Please input a file number:" ;
        string fileNum ;
        cin >> fileNum ;

        DirectedGraph aGraph ;

        bool success = false ;
        success = aGraph.create(fileNum) ;

        if (!success) // file doesn't exist
        {
            cout << "Please input a new file number\n" ;
            aGraph.release() ; // destroy the remains of mission 1 info.
            return ;
        }

        aGraph.output() ; // write file
        tmp = aGraph ;
    }
    else if (strcmp(mode, "2") == 0) // double hashing
    {
        if (tmp.none())
        {
            cout << "### There is no graph and choose 1 first. ###\n" << endl ;

            return ;
        }

        tmp.BFS() ;
    }
    else
    {
        cout << "Illegal input! please enter 0, 1 or 2!\n\n" ;
    }
}
