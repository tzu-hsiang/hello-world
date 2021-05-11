/*
   team:27 member:10626305 魏子翔 10627252 徐綺柔
   mission: quadratic hashing, double hashing
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cmath>

#define LINE_SIZE 255

using namespace std ;

struct sData{
    char sId[10] ;
    char sName[10] ;
    unsigned char score[6] ;
    float mean ;

} ;

struct hashSlot{
    int hValue ;
    char sId[10] ;
    char sName[10] ;
    float mean ;

} ;

enum type{
    qHash, dHash
} ;

void modeChoose(char *mode) ;
int textToBinary(string &fileNum) ;
void readBinary(string &fileNum, vector<sData> &sList) ;

int getTableSize(int dataLine) ;
int getSmallestPrime(int basisNum) ;
vector<hashSlot> quadraticHash(vector<sData> &sList) ; // return hash table
int HashFunc(char *key, int tableSize) ;
void quadraticColliHandle(vector<hashSlot> &hashTable, const int curPos,
                        int tableSize, sData oneSt) ;
float searchSuccessAvgTime(vector<hashSlot> &hashTable, vector<sData> &sList,
                           int type) ;
float searchFailAvgTime(vector<hashSlot> &hashTable) ;
int quadraticSearch(vector<hashSlot> &hashTable, sData oneSt, int times) ;
int quadraticFailSearch(vector<hashSlot> &hashTable, int testPos, int times) ;
void writeQuadratic(vector<hashSlot> &hashTable, string fileNum) ;
void writeQhashCompareTimes(vector<hashSlot> &hashTable, vector<sData> &sList) ;

vector<hashSlot> doubleHash(vector<sData> &sList) ; // return hash table
int heightStepFunc(int dataLine) ;
int doubleHashStep(char *key, int heightStep) ;
void doubleColliHandle(vector<hashSlot> &hashTable, int curPos,
                        int tableSize, sData oneSt, int heightStep) ;
int doubleSearch(vector<hashSlot> &hashTable, sData oneSt, int sListSize, int times) ;
float doubleSearchAvgTime(vector<hashSlot> &hashTable, vector<sData> &sList) ;
void writeDhashCompareTimes(vector<hashSlot> &hashTable, vector<sData> &sList) ;
void writeDouble(vector<hashSlot> &hashTable, string fileNum) ;

int main()
{
    char mode[] = "1" ;

    while (strcmp(mode, "0") != 0)
    {
        cout << "******* Hash Table ******\n"
             << "* 0. QUIT               *\n"
             << "* 1. Quadratic probing  *\n"
             << "* 2. Double hashing     *\n"
             << "*************************\n"
             << "Input a choice(0, 1, 2):" ;
        cin >> mode ;
        modeChoose(mode) ;
    }

    return 0 ;
}

void modeChoose(char *mode)
{
    if (strcmp(mode, "0") == 0)
    {
        cout << "Quit.\n" ;
    }
    else if (strcmp(mode, "1") == 0) // quadraticHash
    {
        string fileNum ;

        cout << "Input a file number:" ;
        cin >> fileNum ;

        vector<sData> sList ; // save input file data

        // read .bin file if it exists, or try to create
        // .bin file if.txt file exists.
        readBinary(fileNum, sList) ;

        if (sList.size() == 0) // .txt file does not exist
            return ;

        vector<hashSlot> hashTable ;
        hashTable = quadraticHash(sList) ;

        // write file
        writeQuadratic(hashTable, fileNum) ;
        writeQhashCompareTimes(hashTable, sList) ;
    }
    else if (strcmp(mode, "2") == 0) // double hashing
    {
        string fileNum ;

        cout << "Input a file number:" ;
        cin >> fileNum ;

        vector<sData> sList ; // save input file data

        // read .bin file if it exists, or try to create
        // .bin file if.txt file exists.
        readBinary(fileNum, sList) ;

        if (sList.size() == 0) // .txt file does not exist
            return ;

        vector<hashSlot> hashTable ;
        hashTable = doubleHash(sList) ;

        // write file
        writeDouble(hashTable, fileNum) ;
        writeDhashCompareTimes(hashTable, sList) ;
    }
    else
    {
        cout << "Illegal input! please enter 0, 1 or 2!\n\n" ;
    }
}

int textToBinary(string &fileNum)
{
    fstream inFile, outFile ;
    int stNo = 0 ;

    inFile.open(("input" + fileNum + ".txt").c_str(), fstream::in) ;

    if (!inFile.is_open()) // file does not exist
        return -1 ; // end, re-enter new file number


    string outName = "input" + fileNum + ".bin" ;
    outFile.open(outName.c_str(), fstream::out|fstream::binary) ;

    if (outFile.is_open())
    {
        char rBuf[LINE_SIZE] ;

        while (inFile.getline(rBuf, LINE_SIZE, '\n'))
        {
            string temp ;
            sData oneSt ;
            int cNo = 0, pre = 0, pos = 0 ;

            stNo++ ;
            temp.assign(rBuf) ;
            pos = temp.find_first_of('\t', pre) ;

            while (pos != string::npos)
            {
                switch(++cNo)
                {
                    case 1:
                        strcpy(oneSt.sId, temp.substr(pre, pos-pre).c_str()) ;
                        break ;

                    case 2:
                        strcpy(oneSt.sName, temp.substr(pre, pos-pre).c_str()) ;
                        break ;

                    default:
                        oneSt.score[cNo-3] = atoi(temp.substr(pre, pos-pre).c_str()) ;
                        break ;
                }


                pre = ++pos ;
                pos = temp.find_first_of('\t', pre) ;
            }

            pos = temp.find_last_of('\t') ;
            oneSt.mean = atof(temp.substr(pos+1).c_str()) ;

            outFile.write((char *)&oneSt, sizeof(oneSt)) ;
        }

        outFile.close() ;
    }

    inFile.close() ;

    return stNo ;

}

void readBinary(string &fileNum, vector<sData> &sList)
{
    fstream binFile ;
    sData oneSt ;
    int dataLine = 0 ;

    binFile.open(("input" + fileNum + ".bin").c_str(), fstream::in|fstream::binary) ;

    if (!binFile.is_open())
    {
        cout << "\n...the .bin file does not exist\n" ;
        cout << "try to find .txt file\n" ;

        int studentNum = textToBinary(fileNum) ; // try to create binary file
        if (studentNum == -1) // .txt doesn't exist
        {
            cout << "input" + fileNum + ".txt" << " does not exist!!\n\n" ;
            return ;
        }
        else
        {
            cout << "input" + fileNum + ".bin" << " is created!!\n" ;
            readBinary(fileNum, sList) ;
        }
    }
    else
    {
        cout << "input" + fileNum + ".bin" << " does exist!!\n\n" ;
        cout << "reading...\n" ;

        binFile.seekg(0, binFile.end) ;
        dataLine = binFile.tellg()/sizeof(oneSt) ;
        binFile.seekg(0, binFile.beg) ;

        sList.reserve(dataLine) ; // allocate memory for hash table
        sList.resize(dataLine) ;

        // read
        for (int i = 0; i < dataLine; i++)
        {
            binFile.read((char *)&oneSt, sizeof(oneSt)) ;
            sList[i] = oneSt ;
        }

        binFile.close() ;

    }
}

void writeQuadratic(vector<hashSlot> &hashTable, string fileNum)
{
    ofstream outFile ;
    outFile.open(("quadratic" + fileNum + ".txt").c_str(), ios::out) ;

    outFile << " --- Hash table created by Quadratic probing ---" << endl ;

    for (int i = 0; i < hashTable.size(); i++)
    {
        outFile << "[" ;
        outFile << std::right << setw(3) << i ;
        outFile << "]" ;
        if (strcmp(hashTable[i].sId, "") == 0)
        {
            outFile << endl ;
        }
        else
        {
            outFile << std::right << setw(10) << hashTable[i].hValue << ", "
                    << std::right << setw(10) << hashTable[i].sId << ", "
                    << std::right << setw(10) << hashTable[i].sName << ", "
                    << std::right << setw(10) << hashTable[i].mean << endl ;
        }
    }

    outFile << " -----------------------------------------------------" << endl ;

    outFile.close() ;
}

vector<hashSlot> quadraticHash(vector<sData> &sList)
{
    int tableSize = getTableSize(sList.size()) ;

    vector<hashSlot> hashTable ;
    hashTable.reserve(tableSize) ; // allocate memory for hash table
    hashTable.resize(tableSize) ;
    for (int i = 0; i < tableSize; i++) // initialize
    {
        hashTable[i].hValue = 0 ;
        hashTable[i].mean = 0.0 ;
        strcpy(hashTable[i].sId, "") ;
        strcpy(hashTable[i].sName, "") ;
    }

    for (int i = 0; i < sList.size(); i++) // hashing
    {
        int hashPos = HashFunc(sList[i].sId, tableSize) ;

        if (strcmp(hashTable[hashPos].sId, "") == 0)
        {
            hashTable[hashPos].hValue = hashPos ;
            strcpy(hashTable[hashPos].sName, sList[i].sName) ;
            strcpy(hashTable[hashPos].sId, sList[i].sId) ;
            hashTable[hashPos].mean = sList[i].mean ;

        }
        else // collision
        {
            quadraticColliHandle(hashTable, hashPos, tableSize, sList[i]) ;
        }
    }

    return hashTable ;
}

void writeQhashCompareTimes(vector<hashSlot> &hashTable, vector<sData> &sList)
{
    cout << endl ;
    cout << "Hash table has been successfully created by Quadratic probing\n\n" ;

    float failTimes = searchFailAvgTime(hashTable) ;

    cout << "unsuccessful search: "
         << fixed << setprecision(4) << failTimes
         << " comparisons on average\n" ;

    float successTimes = searchSuccessAvgTime(hashTable, sList, qHash) ;

    cout << "successful search: "
         << fixed << setprecision(4) << successTimes
         << " comparisons on average\n" ;

    cout << endl ;
    cout.unsetf( ios::fixed ) ; // close fixed function
}

void quadraticColliHandle(vector<hashSlot> &hashTable, const int curPos,
                         int tableSize, sData oneSt)
{
    int prob = 1, base = 1 ;
    const int lastPos = tableSize - 1 ;
    int hValue = curPos ;
    int newPos = curPos + prob ;

    if (curPos+prob > lastPos)
        newPos = 0 ; // special case: cur at the last position

    while (strcmp(hashTable[newPos].sId, "") != 0) // next position still not empty
    {
        base++ ;
        prob = base * base ;

        if (curPos+prob > lastPos)
        {
            prob = prob - (lastPos - curPos) ; // offset minus the remaining data

            while (prob > tableSize)
                prob -= tableSize ;

            prob = prob - 1 ;
            newPos = prob ;

        }
        else
            newPos = curPos + prob ;

    }

    hashTable[newPos].hValue = hValue ;
    strcpy(hashTable[newPos].sName, oneSt.sName) ;
    strcpy(hashTable[newPos].sId, oneSt.sId) ;
    hashTable[newPos].mean = oneSt.mean ;

}

int HashFunc(char *key, int tableSize)
{
    long long product = 1 ;
    int hashPos ;

    for (int i = 0; key[i] != '\0'; i++)
        product = (long long)key[i] * product ;

    hashPos = product % tableSize ;

    return hashPos ;
}

int getTableSize(int dataLine)
{
    int basisNum = int(dataLine * 1.2) + 1 ;
    int tableSize = getSmallestPrime(basisNum) ;

    return tableSize ;
}

int getSmallestPrime(int basisNum)
{
    bool found = false ;

    while (!found)
    {
        int findRange = sqrt(basisNum) ;

        for (int i = 2; i <= findRange; i++)
        {
            if (basisNum % i == 0) // basis not a prime
            {
                found = false ;
                break ;
            }
            else
            { // if basisNum % i != 0 every time in the range, it is a prime
                found = true ;
            }
        }

        if (found == true)
            return basisNum ;

        basisNum++ ;
    }
}

float searchSuccessAvgTime(vector<hashSlot> &hashTable, vector<sData> &sList, int type)
{
    int times = 0, total = 0 ;
    int dataLine = 0 ; // existing records

    // calculate existing records
    for (int i = 0; i < hashTable.size(); i++)
    {
        if (hashTable[i].hValue != 0)
            dataLine++ ;
    }

    // calculate search success sum of times
    for (int i = 0; i < sList.size(); i++)
    {
        // times for once search
        if (type == qHash)
            times = quadraticSearch(hashTable, sList[i], times) ;
        else
            times = doubleSearch(hashTable, sList[i], sList.size(), times) ;

        total += times ;
        times = 0 ;
    }

    float avgTimes = (float)total / dataLine ;

    return avgTimes ;
}

float searchFailAvgTime(vector<hashSlot> &hashTable)
{
    int total = 0 ;
    int testPos = 0 ;

    for (int i = 0; i < hashTable.size(); i++)
    {
        // times for once search
        int times = quadraticFailSearch(hashTable, testPos, times) ;
        total += times ;
        times = 0 ;
        testPos++ ;
    }

    float avgTimes = (float)total / hashTable.size() - 1 ;

    return avgTimes ;
}

int quadraticSearch(vector<hashSlot> &hashTable, sData oneSt, int times)
{
    int hashPos = HashFunc(oneSt.sId, hashTable.size()) ;

    // found with not collision
    if (strcmp(hashTable[hashPos].sId, oneSt.sId) == 0)
    {
        times = 1 ;
    }
    else // collision
    {
        times++ ; // before collision occur

        int prob = 1, base = 1 ;
        const int lastPos = hashTable.size() - 1 ;
        int curPos = hashPos ;
        int hValue = curPos ;
        int newPos = curPos + prob ;

        // special case: cur at the last position
        if (curPos+prob > lastPos)
        {
            newPos = 0 ;

            if (strcmp(hashTable[newPos].sId, oneSt.sId) == 0)
            {
                times++ ;
                return times ;
            }
        }

        // for exist search, next position data doesn't equal to target
        while (strcmp(hashTable[newPos].sId, oneSt.sId) != 0)
        {
            times++ ;
            base++ ;
            prob = base * base ;

            if (curPos+prob > lastPos)
            {
                // offset minus the remaining data
                prob = prob - (lastPos - curPos) ;

                while (prob > hashTable.size())
                    prob -= hashTable.size() ;

                prob = prob - 1 ;
                newPos = prob ;
            }
            else
                newPos = curPos + prob ;
        }

        times++ ; // for found after collision
    }

    return times ;
}

int quadraticFailSearch(vector<hashSlot> &hashTable, int testPos, int times)
{
    times = 0 ;

    // found empty with not collision
    if (strcmp(hashTable[testPos].sId, "") == 0)
    {
        times = 1 ;
    }
    else // collision
    {
        times++ ; // before collision occur

        int prob = 1, base = 1 ;
        const int lastPos = hashTable.size() - 1 ;
        int curPos = testPos ;
        int hValue = curPos ;
        int newPos = curPos + prob ;

        // special case: cur at the last position
        if (curPos+prob > lastPos)
        {
            newPos = 0 ;

            if (strcmp(hashTable[newPos].sId, "") == 0)
            {
                times++ ;
                return times ;
            }
        }

        // for non-exist search, next position still not empty
        while (strcmp(hashTable[newPos].sId, "") != 0)
        {
            times++ ;
            base++ ;
            prob = base * base ;

            if (curPos+prob > lastPos)
            {
                prob = prob - (lastPos - curPos) ; // offset minus the remaining data

                while (prob > hashTable.size())
                    prob -= hashTable.size() ;

                prob = prob - 1 ;
                newPos = prob ;
            }
            else
                newPos = curPos + prob ;
        }

        times++ ; // for found after collision
    }

    return times ;
}

vector<hashSlot> doubleHash(vector<sData> &sList)
{
    int tableSize = getTableSize(sList.size()) ;
    int heightStep = heightStepFunc(sList.size()) ;

    vector<hashSlot> hashTable ;
    hashTable.reserve(tableSize) ; // allocate memory for hash table
    hashTable.resize(tableSize) ;

    for (int i = 0; i < tableSize; i++) // initialize
    {
        hashTable[i].hValue = 0 ;
        hashTable[i].mean = 0.0 ;
        strcpy(hashTable[i].sId, "") ;
        strcpy(hashTable[i].sName, "") ;
    }

    for (int i = 0; i < sList.size(); i++) // hashing
    {
        int hashPos = HashFunc(sList[i].sId, tableSize) ;

        if (strcmp(hashTable[hashPos].sId, "") == 0)
        {
            hashTable[hashPos].hValue = hashPos ;
            strcpy(hashTable[hashPos].sName, sList[i].sName) ;
            strcpy(hashTable[hashPos].sId, sList[i].sId) ;
            hashTable[hashPos].mean = sList[i].mean ;

        }
        else // collision
        {
            doubleColliHandle(hashTable, hashPos, tableSize, sList[i], heightStep) ;
        }
    }

    return hashTable ;
}

void doubleColliHandle(vector<hashSlot> &hashTable, int curPos,
                        int tableSize, sData oneSt, int heightStep)
{
    // if collision occur, produce step via step function
    const int base = doubleHashStep(oneSt.sId, heightStep) ;
    const int lastPos = tableSize - 1 ;
    int hValue = curPos ;
    int newPos = curPos ;

    // next position still not empty
    while (strcmp(hashTable[newPos].sId, "") != 0)
    {
        newPos = curPos + base ;

        if (newPos > lastPos)
        {
            newPos = newPos - lastPos - 1 ;
            curPos = newPos ;
        }
        else
        {
            curPos = newPos ;
        }

    }

    hashTable[newPos].hValue = hValue ;
    strcpy(hashTable[newPos].sName, oneSt.sName) ;
    strcpy(hashTable[newPos].sId, oneSt.sId) ;
    hashTable[newPos].mean = oneSt.mean ;
}

int heightStepFunc(int dataLine)
{
    int heightStep = 0, basisNum = 0 ;

    basisNum = dataLine / 3 + 1 ;
    heightStep = getSmallestPrime(basisNum) ;

    return heightStep ;
}

int doubleHashStep(char *key, int heightStep)
{
    long long product = 1 ;
    int hashPos ;

    for (int i = 0; key[i] != '\0'; i++)
        product = (long long)key[i] * product ;

    int stepOffset = heightStep - (product % heightStep) ;

    return stepOffset ;
}

float doubleSearchAvgTime(vector<hashSlot> &hashTable, vector<sData> &sList)
{
    int times = 0, total = 0 ;
    int dataLine = 0 ; // existing records

    // calculate existing records
    for (int i = 0; i < hashTable.size(); i++)
    {
        if (hashTable[i].hValue != 0)
            dataLine++ ;
    }

    // calculate search success sum of times
    for (int i = 0; i < sList.size(); i++)
    {
        // times for once search
        times = doubleSearch(hashTable, sList[i], sList.size(), times) ;
        total += times ;
        times = 0 ;
    }

    float avgTimes = (float)total / dataLine ;

    cout << avgTimes << endl ;
    system("pause") ;
    return avgTimes ;
}

int doubleSearch(vector<hashSlot> &hashTable, sData oneSt, int sListSize, int times)
{
    int hashPos = HashFunc(oneSt.sId, hashTable.size()) ;

    if (strcmp(hashTable[hashPos].sId, oneSt.sId) == 0)
    {
        times = 1 ;
    }
    else // collision
    {
        int heightStep = heightStepFunc(sListSize) ;
        const int base = doubleHashStep(oneSt.sId, heightStep) ;
        const int lastPos = hashTable.size() - 1 ;
        int curPos = hashPos ;
        int hValue = curPos ;
        int newPos = curPos ;

        // next position still not equal
        while (strcmp(hashTable[newPos].sId, oneSt.sId) != 0)
        {
            times++ ;
            newPos = curPos + base ;

            if (newPos > lastPos)
            {
                newPos = newPos - lastPos - 1 ;
                curPos = newPos ;
            }
            else
            {
                curPos = newPos ;
            }

        }

        times++ ;
    }

    return times ;
}

void writeDouble(vector<hashSlot> &hashTable, string fileNum)
{
    ofstream outFile ;
    outFile.open(("double" + fileNum + ".txt").c_str(), ios::out) ;

    outFile << " --- Hash table created by Double hashing    ---" << endl ;

    for (int i = 0; i < hashTable.size(); i++)
    {
        outFile << "[" ;
        outFile << std::right << setw(3) << i ;
        outFile << "]" ;
        if (strcmp(hashTable[i].sId, "") == 0)
        {
            outFile << endl ;
        }
        else
        {
            outFile << std::right << setw(10) << hashTable[i].hValue << ", "
                    << std::right << setw(10) << hashTable[i].sId << ", "
                    << std::right << setw(10) << hashTable[i].sName << ", "
                    << std::right << setw(10) << hashTable[i].mean << endl ;
        }
    }

    outFile << " -----------------------------------------------------" << endl ;

    outFile.close() ;
}

void writeDhashCompareTimes(vector<hashSlot> &hashTable, vector<sData> &sList)
{
    cout << endl ;
    cout << "Hash table has been successfully created by Double hashing \n\n" ;

    float successTimes = searchSuccessAvgTime(hashTable, sList, dHash) ;

    cout << "successful search: "
         << fixed << setprecision(4) << successTimes
         << " comparisons on average\n" ;

    cout << endl ;
    cout.unsetf( ios::fixed ) ; // close fixed function
}
