/*
   team:27 member:10626305 魏子翔 10627252 徐綺柔
   mission: connected components, shortest paths
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <stack>

#define MAX_LEN 10
#define GUESS_SIZE 2000
#define GUESS_SUBIDS 10
#define INFINITY 9.9
#define ENDLOOP -2
#define RANGE_CHECK 2

using namespace std ;

class UnDirectedGraph
{
private:
    typedef struct sP
    {
        char id1[MAX_LEN] ;
        char id2[MAX_LEN] ;
        float weight ;
    } studentPair ;

    typedef struct aLNode
    {
        char subId[MAX_LEN] ;
        float weight ;
        aLNode *next ;
        bool usedStartPt ; // has been chosen for start point
    } adjListNode ;

    typedef struct aL
    {
        char mainId[MAX_LEN] ;
        aLNode *head ;
        bool visited ;
        bool joined ; // has been added to sub-graph
        float shortestPath ;
    } adjList ;

    typedef struct group
    {
        vector<string> groupMemberId ;
    } subGraphGroup ;

    string fileNum ;
    vector<adjList> adjL1 ;
    vector<adjList> adjL2 ;
    vector<adjList> totalAdjL ;
    vector<subGraphGroup> subGraphMemo ;
    float threshold ;
    int components ; // record numbers of sub-graph

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

            for (int i = 0; i < dataLine; i++)
            {
                binFile.read((char *)&aRelation, sizeof(aRelation)) ;
                sList.push_back(aRelation) ;
            }

            binFile.close() ;
            return sList ;
        }
    }

    static bool compareIdForMain(const adjList &first, const adjList &second)
    {
        if(strcmp(first.mainId, second.mainId) < 0 )
            return true ;
        else
            return false ;
    }

    void createAdjListMain(vector<studentPair> &sList)
    {
        adjL1.reserve(GUESS_SIZE) ;
        adjL2.reserve(GUESS_SIZE) ;

        bool exist = false ;

        for (int i = 0; i < sList.size(); i++)
        {
            // adjList 1
            for (int j = 0; j < adjL1.size(); j++)
            {
                if (strcmp(adjL1.at(j).mainId, sList.at(i).id1) == 0)
                {
                    exist = true ;
                    break ;
                }
            }

            if (!exist)
            {
                adjList temp ;
                strcpy(temp.mainId, sList.at(i).id1) ;
                temp.head = NULL ;
                temp.visited = false ;
                temp.joined = false ;
                temp.shortestPath = INFINITY ;
                adjL1.push_back(temp) ;
            }

            exist = false ;

            // adjList 2
            for (int j = 0; j < adjL2.size(); j++)
            {
                if (strcmp(adjL2.at(j).mainId, sList.at(i).id2) == 0)
                {
                    exist = true ;
                    break ;
                }
            }

            if (!exist)
            {
                adjList temp ;
                strcpy(temp.mainId, sList.at(i).id2) ;
                temp.head = NULL ;
                temp.visited = false ;
                temp.joined = false ;
                temp.shortestPath = INFINITY ;
                adjL2.push_back(temp) ;
            }

            exist = false ;
        }

        vector<adjList> (adjL1).swap(adjL1) ;
        vector<adjList> (adjL2).swap(adjL2) ;
        sort(adjL1.begin(), adjL1.end(), compareIdForMain) ;
        sort(adjL2.begin(), adjL2.end(), compareIdForMain) ;
    }

    void createAdjListConnectID1(vector<studentPair> &sList, vector<adjList> &adjL)
    {
        for (int i = 0; i < adjL.size(); i++)
        {
            for (int j = 0; j < sList.size(); j++)
            {
                if (strcmp(adjL.at(i).mainId, sList.at(j).id1) == 0
                        && sList.at(j).weight <= threshold)
                {
                    if (adjL.at(i).head == NULL)
                    {
                        adjListNode *temp = new adjListNode ;
                        strcpy(temp->subId, sList.at(j).id2) ;
                        temp->weight = sList.at(j).weight ;
                        temp->next = NULL ;
                        temp->usedStartPt = false ;
                        adjL.at(i).head = temp ;
                    }
                    else // head exist and sort list by compare id from small to large
                    {
                        adjListNode *temp = new adjListNode ;
                        strcpy(temp->subId, sList.at(j).id2) ;
                        temp->weight = sList.at(j).weight ;
                        temp->next = NULL ;
                        temp->usedStartPt = false ;

                        if (strcmp(temp->subId, adjL.at(i).head->subId) < 0)
                        {
                            adjListNode *oldHead = adjL.at(i).head ;
                            temp->next = oldHead ;
                            adjL.at(i).head = temp ;
                        }
                        else
                        {
                            bool inserted = false ;
                            adjListNode *keepPrev = adjL.at(i).head ;

                            for (adjListNode *x = adjL.at(i).head->next ; x != NULL; x = x->next)
                            {
                                if (strcmp(temp->subId, x->subId) < 0)
                                {
                                    keepPrev->next = temp ;
                                    temp->next = x ;
                                    inserted = true ;
                                    break ;
                                }

                                keepPrev = keepPrev->next ;
                            }

                            if (!inserted) // temp is greater than all the IDs in this list
                            {
                                keepPrev->next = temp ;
                            }
                        } // end inner
                    } // end outer
                }
            } // end inner for
        } // end outer for
    }

    void createAdjListConnectID2(vector<studentPair> &sList, vector<adjList> &adjL)
    {
        for (int i = 0; i < adjL.size(); i++)
        {
            for (int j = 0; j < sList.size(); j++)
            {
                if (strcmp(adjL.at(i).mainId, sList.at(j).id2) == 0
                        && sList.at(j).weight <= threshold)
                {
                    if (adjL.at(i).head == NULL)
                    {
                        adjListNode *temp = new adjListNode ;
                        strcpy(temp->subId, sList.at(j).id1) ;
                        temp->weight = sList.at(j).weight ;
                        temp->next = NULL ;
                        temp->usedStartPt = false ;
                        adjL.at(i).head = temp ;

                    }
                    else // head exist and sort list by compare id from small to large
                    {
                        adjListNode *temp = new adjListNode ;
                        strcpy(temp->subId, sList.at(j).id1) ;
                        temp->weight = sList.at(j).weight ;
                        temp->next = NULL ;
                        temp->usedStartPt = false ;

                        if (strcmp(temp->subId, adjL.at(i).head->subId) < 0)
                        {
                            adjListNode *oldHead = adjL.at(i).head ;
                            temp->next = oldHead ;
                            adjL.at(i).head = temp ;
                        }
                        else
                        {
                            bool inserted = false ;
                            adjListNode *keepPrev = adjL.at(i).head ;

                            for (adjListNode *x = adjL.at(i).head->next ; x != NULL; x = x->next)
                            {
                                if (strcmp(temp->subId, x->subId) < 0)
                                {
                                    keepPrev->next = temp ;
                                    temp->next = x ;
                                    inserted = true ;
                                    break ;
                                }

                                keepPrev = keepPrev->next ;
                            }

                            if (!inserted) // temp is greater than all the IDs in this list
                            {
                                keepPrev->next = temp ;
                            }
                        } // end inner
                    } // end outer
                }
            } // end inner for
        } // end outer for
    }

    void createTotalAdjList()
    {
        totalAdjL.reserve(adjL1.size()+adjL2.size()) ;

        for (int i = 0; i < adjL1.size(); i++)
        {
            // else: no connection with any node, don't push it into the vector
            if (adjL1.at(i).head != NULL)
                totalAdjL.push_back(adjL1.at(i)) ;
        }

        for (int i = 0; i < adjL2.size(); i++)
        {
            bool found = false ;

            // no connection with any node
            if (adjL2.at(i).head == NULL)
                continue ;

            // check adjL2 has a same id in totalAdjL
            for (int j = 0; j < totalAdjL.size(); j++)
            {
                if (strcmp(totalAdjL.at(j).mainId, adjL2.at(i).mainId) == 0)
                {
                    found = true ;
                    break ;
                }
            }

            if (!found)
                totalAdjL.push_back(adjL2.at(i)) ;
        }

        vector<adjList> (totalAdjL).swap(totalAdjL) ;
        sort(totalAdjL.begin(), totalAdjL.end(), compareIdForMain) ;
    }

    void writeAdjL()
    {
        ofstream outFile ;
        stringstream threshold_str ;
        threshold_str << threshold ;

        outFile.open(("pairs" + fileNum + "_" + threshold_str.str() + ".adj").c_str(), ios::out) ;

        if (!outFile.is_open())
        {
            cout << "Unknown error! can not create output file: "
                 << "pairs" + fileNum + "_" + threshold_str.str() + ".adj"
                 << "\nplease execute the program again!" << endl ;
            return ;
        }

        cout << "\n<<< There are " << totalAdjL.size() << " IDs in total. >>>\n" << endl ;
        outFile << "<<< There are " << totalAdjL.size() << " IDs in total. >>>" << endl ;

        int nodes = 0 ;

        for (int i = 0; i < totalAdjL.size(); i++)
        {
            outFile << "[ " << i+1 << "] " << totalAdjL.at(i).mainId << ":" << endl ;

            adjListNode *ptr = totalAdjL.at(i).head ;
            int idx = 1 ;

            outFile << "      " ;
            while (ptr != NULL)
            {
                outFile << "(" << idx << ")" << left << setw(10) << ptr->subId << ", " << ptr->weight << "  " ;
                nodes++ ;
                ptr = ptr->next ;

                if (idx % 10 == 0)
                {
                    outFile << "\n" ;
                    outFile << "      " ;
                }

                idx++ ;
            }

            outFile << "\n" << endl ;
        }

        cout << "<<< There are " << nodes << " nodes in total. >>>\n" << endl ;
        outFile << "<<< There are " << nodes << " nodes in total. >>>" << endl ;
        outFile.close() ;
        // output adjL
    }

    int judgeStartPoint()
    {
        bool found = false ;
        int pos ;

        for (pos = 0; pos < totalAdjL.size(); pos++)
        {
            if (!totalAdjL.at(pos).visited)
            {
                found = true ;
                break ;
            }
        }

        if (!found)
            return -1 ; // all id are reached

        return pos ;
    }

    void clearVisited() // initialize all info. about running graph
    {
        for (int i = 0; i < totalAdjL.size(); i++)
        {
            totalAdjL.at(i).visited = false ;
            totalAdjL.at(i).joined = false ;
            totalAdjL.at(i).shortestPath = INFINITY ;

            for (adjListNode *ptr = totalAdjL.at(i).head; ptr != NULL; ptr = ptr->next)
                ptr->usedStartPt = false ;
        }
    }

    void traverseReached() // while called, means a sub-graph is added
    {
        vector<string> thisGraphIds ;
        thisGraphIds.reserve(GUESS_SUBIDS) ;

        for (int i = 0; i < totalAdjL.size(); i++)
        {
            if (totalAdjL.at(i).visited && !totalAdjL.at(i).joined)
            {
                string cStrToStr ;
                cStrToStr.assign(totalAdjL.at(i).mainId) ;
                thisGraphIds.push_back(cStrToStr) ;

                totalAdjL.at(i).joined = true ;
            }
        }

        vector<string> (thisGraphIds).swap(thisGraphIds) ;
        subGraphGroup temp ;
        temp.groupMemberId = thisGraphIds ;
        subGraphMemo.push_back(temp) ;

        components++ ;
    }

    int searchIdInMain(int left, int right, const char *newStart)
    {
        while (left <= right)
        {
            int mid = (left + right) / 2 ;

            if (strcmp(totalAdjL.at(mid).mainId, newStart) == 0)
                return mid ;
            else if (strcmp(totalAdjL.at(mid).mainId, newStart) > 0)
                right = mid - 1 ;
            else
                left = mid + 1 ;
        }

        return -1 ; // can not find input id
    }

    void DFS(int startPt)
    {
        stack<adjList> findpath ;

        totalAdjL.at(startPt).visited = true ;
        findpath.push(totalAdjL.at(startPt)) ;

        adjListNode *connectId = totalAdjL.at(startPt).head ;

        while (!findpath.empty())
        {
            // search next start point
            while (connectId != NULL)
            {
                if (!connectId->usedStartPt) // the connected id has not been used as a start point
                {
                    // new start point
                    char *newStart = connectId->subId ;
                    connectId->usedStartPt = true ;

                    // search connected node position in main array
                    int mainPos = searchIdInMain(0, totalAdjL.size()-1, newStart) ;

                    if (strcmp(newStart, totalAdjL.at(mainPos).mainId) == 0)
                    {
                        // not reached
                        if (!totalAdjL.at(mainPos).visited)
                        {
                            totalAdjL.at(mainPos).visited = true ;
                            findpath.push(totalAdjL.at(mainPos)) ;

                            connectId = totalAdjL.at(mainPos).head ; // renew start point
                        }
                        // reached
                        else
                        {
                            connectId = connectId->next ;
                        }
                    }
                }
                else // the connected id has been used as a start point
                    connectId = connectId->next ;

            } // end for (search connected id)

            if (connectId == NULL) // all connected ids have been used as a start point
            {
                findpath.pop() ;

                if (!findpath.empty())
                    connectId = findpath.top().head ;
            }
        }

    } // end DFS()

    static bool compareSubGraphNumsOfId(const subGraphGroup &first, const subGraphGroup &second)
    {
        if(first.groupMemberId.size() >= second.groupMemberId.size())
            return true ;
        else
            return false ;
    }

    void sortSubGraph() // compare ids that sub-graphs including
    {
        sort(subGraphMemo.begin(), subGraphMemo.end(), compareSubGraphNumsOfId) ;
    }

    void writeSubGraphs()
    {
        ofstream outFile ;
        stringstream threshold_str ;
        threshold_str << threshold ;

        outFile.open(("pairs" + fileNum + "_" + threshold_str.str() + ".cc").c_str(), ios::out) ;
        if (!outFile.is_open())
        {
            cout << "Unknown error! can not create output file: "
                 << "pairs" + fileNum + "_" + threshold_str.str() + ".adj"
                 << "\nplease execute the program again!" << endl ;
            return ;
        }

        outFile << "<<< There are " << components << " connected components in total. >>>" << endl ;
        cout << "\n<<< There are " << components << " connected components in total. >>>\n" << endl ;

        for (int i = 0; i < subGraphMemo.size(); i++)
        {
            outFile << "{" << setw(2) << i+1 << "}" << " Connected Component: size = " <<
                    subGraphMemo.at(i).groupMemberId.size() << "\n   " ;

            for (int j = 0, endline = 0; j < subGraphMemo.at(i).groupMemberId.size(); j++, endline++)
            {
                outFile << "(" << setw(3) << j+1 << ") " << subGraphMemo.at(i).groupMemberId.at(j) << "  " ;

                if (endline == 7)
                {
                    outFile << "\n   " ;
                    endline = -1 ;
                }
            }

            outFile << "\n" << endl ;
        }

        outFile.close() ;
    }

    void listAllNode()
    {
        cout << endl ;

        for (int i = 0, endLine = 0; i < totalAdjL.size(); i++, endLine++)
        {
            cout << setw(MAX_LEN) << totalAdjL.at(i).mainId ;

            if (endLine == 7)
            {
                cout << "\n" ;
                endLine = -1 ;
            }
        }

        cout << endl ;
    }

    int getExistInputIdPosition()
    {
        int startPt ; // find position of input id in main array
        string inputId ;
        cout << "Input a student ID [0: exit]:" ;
        cin >> inputId ;

        while (cin.fail())
        {
            cout << "Input is not a legal id, please input a new id\n"
                 << "Input a student ID [0: exit]:" ;
            cin.clear() ;
            cin.sync() ;
            cin >> inputId ;
        }

        if (inputId.compare("0") == 0)
        {
            cout << "Quit." << endl ;
            return ENDLOOP ;
        }

        startPt = searchIdInMain(0, totalAdjL.size()-1, inputId.c_str()) ;

        if (startPt == -1)
        {
            cout << "###The id not exist in graph, please enter a new id in this graph!###\n" ;
        }

        return startPt ;
    }

    void Dijkstra(int startPt)
    {
        totalAdjL.at(startPt).shortestPath = 0.0 ;
        totalAdjL.at(startPt).visited = true ;
        adjListNode *connectIdPtr = totalAdjL.at(startPt).head ;

        int nextSt = 0 ;
        float mini = INFINITY ;

        while (connectIdPtr != NULL)
        {
            // find other vertices that first vertex can reach
            int mainPos = searchIdInMain(0, totalAdjL.size()-1, connectIdPtr->subId) ;
            totalAdjL.at(mainPos).shortestPath = connectIdPtr->weight ;

            // find the small weight vertex position as a next start vertex position
            if (mini > totalAdjL.at(mainPos).shortestPath)
            {
               mini = totalAdjL.at(mainPos).shortestPath ;
               nextSt = mainPos ;
            }

            connectIdPtr = connectIdPtr->next ;
        }

        // use the smallest shortestPath to reach other vertices
        while (1)
        {
            float curOffset = totalAdjL.at(nextSt).shortestPath ;
            totalAdjL.at(nextSt).visited = true ;

            connectIdPtr = totalAdjL.at(nextSt).head ;

            while (connectIdPtr != NULL)
            {
                int mainPos = searchIdInMain(0, totalAdjL.size()-1, connectIdPtr->subId) ;

                if (totalAdjL.at(mainPos).shortestPath == INFINITY)
                    totalAdjL.at(mainPos).shortestPath = curOffset + connectIdPtr->weight ;
                else
                    totalAdjL.at(mainPos).shortestPath = min(curOffset + connectIdPtr->weight,
                                                             totalAdjL.at(mainPos).shortestPath) ;
                connectIdPtr = connectIdPtr->next ;
            }

            // check all vertices in this sub graph are reached
            // if not, choose the vertex who has a minimum shortestPath as new startPt
            bool allVisited = false ;
            mini = INFINITY ;
            bool blockCheck = false ;

            for (int i = 0; i < totalAdjL.size(); i++)
            {
                // find the vertex has not been visited && the smallest shortestPath
                if (!totalAdjL.at(i).visited && totalAdjL.at(i).shortestPath < RANGE_CHECK)
                {
                    if (mini > totalAdjL.at(i).shortestPath)
                    {
                        mini = totalAdjL.at(i).shortestPath ;
                        nextSt = i ;
                    }

                    allVisited = false ;
                    blockCheck = true ;
                }
                else
                {
                    if (!blockCheck)
                       allVisited = true ;
                }
            }

            if (allVisited)
                break ; // end once Dijkstra

        } // end while (1)
    } // end Dijkstra()

    static bool compareShortest(const adjList &first, const adjList &second)
    {
        if (first.shortestPath < second.shortestPath)
            return true ;
        else
            return false ;
    }

    void writeShortest(vector<adjList> &shortestIds)
    {
        ofstream outFile ;
        stringstream threshold_str ;
        threshold_str << threshold ;

        outFile.open(("pairs" + fileNum + "_" + threshold_str.str() + ".ds").c_str(), ios::app) ;
        if (!outFile.is_open())
        {
            cout << "Unknown error! can not create output file: "
                 << "pairs" + fileNum + "_" + threshold_str.str() + ".adj"
                 << "\nplease execute the program again!" << endl ;
            return ;
        }

        cout << "origin: " << shortestIds.at(0).mainId << "\n" ;
        outFile << "origin: " << shortestIds.at(0).mainId << "\n" ;

        for (int i = 1, endLine = 0; i < shortestIds.size(); i++, endLine++)
        {
            cout << "(" << std::right << setw(3) << i << ")" ;
            cout << std::right << setw(10) << shortestIds.at(i).mainId << ", "
                 << std::left << setw(4) << shortestIds.at(i).shortestPath << "  " ;

            outFile << "(" << std::right << setw(3) << i << ")" ;
            outFile << std::right << setw(10) << shortestIds.at(i).mainId << ", "
                    << std::left << setw(4) << shortestIds.at(i).shortestPath << "  " ;

            if (endLine == 7)
            {
                cout << "\n" ;
                outFile << "\n" ;
                endLine = -1 ;
            }
        }

        cout << "\n" << endl ;
        outFile << "\n" << endl ;

        outFile.close() ;
    }

public:

    UnDirectedGraph()
    {
        fileNum = "" ;
        threshold = 0.0 ;
        components = 0 ;
    }

    UnDirectedGraph(UnDirectedGraph &obj)
    {
        adjL1 = obj.adjL1 ;
        adjL2 = obj.adjL2 ;
        vector<subGraphGroup> ().swap(subGraphMemo) ;
        fileNum = obj.fileNum ;
        threshold = obj.threshold ;
        components = 0 ;

    }

    ~UnDirectedGraph() {release();}

    // successful creation will return true
    bool create(string &fileNum)
    {
        vector<studentPair> sList ;
        sList = readFile(fileNum) ;

        if (sList.empty()) // file doesn't exist
        {
            cout << "Please input a new file number\n" ;
            return false ;
        }

        this->fileNum = fileNum ;

        cout << "Please input a threshold(0,1]:" ;
        cin >> threshold ;

        while (!(threshold > 0 && threshold <= 1) || cin.fail())
        {
            cin.clear() ;
            cin.sync() ;
            cout << "Threshold is out of range or input is not a number,"
                 << "please enter a correct threshold!\n" ;
            cout << "Please input a threshold(0,1]:" ;

            cin >> threshold ;
        }

        // create adjL
        createAdjListMain(sList) ;
        createAdjListConnectID1(sList, adjL1) ;
        createAdjListConnectID2(sList, adjL1) ;

        createAdjListConnectID1(sList, adjL2) ;
        createAdjListConnectID2(sList, adjL2) ;

        createTotalAdjList() ;

        // choose adjL data from memory, not read adj file,
        // so we need to remove main ID(s) that greater than threshold
        writeAdjL() ;

        return true ;
    }

    void BuildSubGraphs()
    {
        subGraphMemo.reserve(GUESS_SUBIDS) ;

        int startPt = judgeStartPoint() ;

        while (startPt != -1)
        {
            DFS(startPt) ;
            traverseReached() ; // while called, means a sub-graph is added
            startPt = judgeStartPoint() ;
        }

        vector<subGraphGroup> (subGraphMemo).swap(subGraphMemo) ;
        sortSubGraph() ;
        writeSubGraphs() ;
    }

    void release()
    {
        vector<adjList> ().swap(this->adjL1) ;
        vector<adjList> ().swap(this->adjL2) ;
        vector<adjList> ().swap(this->totalAdjL) ;
        vector<subGraphGroup> ().swap(this->subGraphMemo) ;
        fileNum = "" ;
        threshold = 0.0 ;
        components = 0 ;

    }

    void clearSubGraphs()
    {
        clearVisited() ;
        subGraphMemo.clear() ;
        components = 0 ;
    }

    void findShortestPath()
    {
        int startPt = 0 ;

        while (startPt != -2)
        {
            // list all id in the graph
            listAllNode() ;
            int startPt = getExistInputIdPosition() ;

            if (startPt == -1)
                ;
            else if (startPt == ENDLOOP)
                return ;
            else // do Dijkstra()
            {
                Dijkstra(startPt) ; // it will renew startPt every operation end

                vector<adjList> shortestIds ;
                shortestIds.reserve(GUESS_SUBIDS) ;

                for (int i = 0; i < totalAdjL.size(); i++)
                {
                    if (totalAdjL.at(i).visited)
                    {
                        adjList temp ;
                        temp.head = NULL ;
                        temp.joined = false ;
                        strcpy(temp.mainId, totalAdjL.at(i).mainId) ;
                        temp.shortestPath = totalAdjL.at(i).shortestPath ;
                        temp.visited = totalAdjL.at(i).visited ;

                        shortestIds.push_back(temp) ;
                    }
                }

                sort(shortestIds.begin(), shortestIds.end(), compareShortest) ;
                writeShortest(shortestIds) ;
                cout << "The record has been added into file.\n" ;

                clearVisited() ;
            }
        }
    }

} ; // end class

void modeChoose(char *mode) ;

int main()
{
    char mode[] = "1" ;

    while (!strcmp(mode, "0") || !strcmp(mode, "1") || !strcmp(mode, "2"))
    {
        cout << endl ;
        cout << "******* Graph data applications ******\n"
             << "* 0. Create adjacency lists          *\n"
             << "* 1. Build connected components      *\n"
             << "* 2. Find shortest paths by Dijkstra *\n"
             << "**************************************\n"
             << "Input a choice(0, 1, 2) [Any other key: QUIT]:" ;
        cin >> mode ;

        if (cin.fail())
        {
            cout << "Quit.\n" ;
            break ;
        }

        modeChoose(mode) ;
    }

    return 0 ;
}

void modeChoose(char *mode)
{
    static bool success ;
    static UnDirectedGraph aGraph ;

    if (strcmp(mode, "0") == 0) // Create adjacency lists
    {
        success = false ;
        aGraph.release() ;
        cout << "Please input a file number:" ;
        string fileNum ;
        cin >> fileNum ;

        while (cin.fail())
        {
            cout << "Input is not a legal string, please input a new file number\n"
                 << "Please input a file number:" ;
            cin.clear() ;
            cin.sync() ;
            cin >> fileNum ;
        }

        cout << "Create adjacency lists...\n" ;
        success = aGraph.create(fileNum) ;

        if (success)
            cout << "The adjacency lists is created.\n" ;

    }
    else if (strcmp(mode, "1") == 0) // Build connected components
    {
        if (!success)
        {
            cout << "### There is no graph and choose 0 first. ###" << endl ;
        }
        else
        {
            cout << "SubGraphs are building...\n" ;
            aGraph.BuildSubGraphs() ;
            cout << "SubGraphs are created.\n" ;
            aGraph.clearSubGraphs() ;
        }

    }
    else if (strcmp(mode, "2") == 0) // Find shortest paths by Dijkstra
    {
        if (!success)
        {
            cout << "### There is no graph and choose 0 first. ###" << endl ;
        }
        else
        {
            cout << "Shortest paths are building...\n" ;
            aGraph.findShortestPath() ;
        }
    }
    else
    {
        cout << "Quit.\n" ;
    }
}
