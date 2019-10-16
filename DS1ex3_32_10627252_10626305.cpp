/*10627252 徐綺柔 10626305 魏子翔                        */
/*@line 29 : main()                                     */
/*@line 153: ffLegality "it judges the fomula ligality" */
/*@line 330: getPostfix "transform infix to postfix   " */

#include <iostream>
#include <string>
#include <cstdio>

#define InMAX 100

using namespace std ;

int fLegality( string & fomula) ; // int 目的: 回傳是否錯誤 0:不合法 1:合法
void getPostfix( string & infix) ; // 中序轉後序

struct Stack
{
    char item ;
    Stack * next ;
};

void push( Stack *, char ) ;
void pop( Stack * ) ;
char getTopElement( Stack * ) ;
bool IsNotEmpty( Stack * ) ;

int main()
{
    char cmd ; // choose
    cout << endl << "*********************************" ;
    cout << endl << "* Arithmetic Expression Evaluator" ;
    cout << endl << "* 0.Quit                        *" ;
    cout << endl << "* 1.Postfix transformation      *" ;
    cout << endl << "*********************************" ;
    cout << endl << "Input a choice(0,1):" ;

    cmd = getchar() ;
    cin.ignore() ; // 接收 enter

    while( !( cmd == '0' || cmd == '1' ) )
    {
        cout << endl << "Error, please enter 0 or 1 to choose mode." ;
        cout << endl << "Input a choice(0,1):" ;
        cmd = getchar() ;
        cin.ignore() ; // 接收 enter
    }

    while( cmd == '1' )
    {

        cout << endl << "Arithmetic:" ;

        char *strptr = new char[InMAX] ;
        int i = 0 ;
        char nextch, ch, prevch = 'X' ; //防止尚未賦值時，亂數恰好為 0~9 的ascii

        while( (ch = getchar()) != '\n' ) // 目的為刪除空格及不必要的 0
        {
            *( strptr + i ) = ch ;
            nextch = *( strptr + i + 1 ) ;
            if( i >= 1 )
                prevch = *( strptr + i - 1 ) ;

            if( ch == ' ')
                i-- ;
            else if( ( prevch == '+' || prevch == '-' || prevch == '*' || prevch == '/' ) &&
                    ( ch == '0') )
            {
                ;
            }
            else if( ( ch == '0' ) && ( nextch == '0' ) )
            {
                int tmp = i ;
                while( *( strptr + i ) != '0' )
                {
                    i++ ;
                }
                *( strptr + tmp ) = *( strptr + i ) ;
            }
            else if( ( ch == '+' || ch == '-' || ch == '*' || ch == '/' ) &&
                    ( prevch == '+' || prevch == '-' || prevch == '*' || prevch == '/' ) )
            {
                ;
            }
            else if( ( ch == '+' || ch == '-' || ch == '*' || ch == '/' ) &&
                    ( prevch == '(' || prevch == ')' ) )
            {
                ;
            }
            else if( ( ch == '+' || ch == '-' || ch == '*' || ch == '/' ) &&
                    !( prevch >= '0' && prevch <= '9' ) )
            { // 如果當下的值為運算子且上一個值為空，那就令當下的值為0，後一個值變回原本的運算子
                *( strptr + i ) = '0' ;
                i++ ;
                *( strptr + i ) = ch ;
            }
            else if( ( ch == '0') && !( prevch >= '0' && prevch <= '9' ) )
                i-- ; // 該值為0 且前一值不為數字或運算子

            i++ ;
        }
        *( strptr + i ) = '\0' ;


        /// cout << strptr << endl ; // 偵錯用

        string fomula = strptr ;
        string infix = strptr ;

        delete [] strptr ;

        /// cout << fomula << "個數 " <<fomula.size() << endl ; // 偵錯用

        if( fLegality( fomula ) == 1 ) // 檢測中序式合法性
        {
            cout << "式子合法，轉換為後序式...\n" ;
            cout << "Postfix expression: " ;
            getPostfix( infix ) ;
        }
        else
        {
            cout << "\n此式子不合法，請重新輸入式子." ;
        }

        cout << endl << "Input a choice(0,1):" ;
        cmd = getchar() ;
        cin.ignore() ; // 接收 enter

        while( !( cmd == '0' || cmd == '1' ) )
        {
            cout << endl << "Error, please enter 0 or 1 to choose mode." ;
            cout << endl << "Input a choice(0,1):" ;
            cmd = getchar() ;
            cin.ignore() ; // 接收 enter
            if( cmd == '0' )
            return 0 ;
        }

        if( cmd == '0' )
            return 0 ;

    } // end while()

    return 0 ;


}

    // 要保證"除數"若為大括弧 內容不得為0
    // 必須使用list或stack來做運算
    // 因此我們交由第二個函式來做
    // 並且error訊息為 math error

int fLegality( string &fomula )
{
    // 此程式允許非 0 數字以 0 開頭，也允許空格，於讀取時都會被忽略
    // 因此不必考慮數字以 0 開頭做運算的特殊情況以及空格

    // 錯誤代碼 1: 括弧錯誤 2:非法字元 3:語法錯誤 4:除數或(及)被除數為0
    // 語法錯誤 list :
    //                3-1:某項目存在於非法位置
    //                3-2:缺少運算子
    //                3-3:多出運算子

    char c ; // 當下運算式的字符
    char nextc ; // c的下一個字符
    bool legal = 0 ;
    int l_brac = 0, r_brac = 0 ;

    /// 判斷是否有使用非法字元或括弧不成對  以及  第一項是否符合語法
    for( int i = 0 ; i < fomula.size() ; i++ )
    {
        c = fomula[i] ;

        switch( c )
        {
        case '0' ... '9': // in ascii, digitals start from 0
            legal = 1 ;
            break ;

        case '+':
        case '-':
        case '*':
        case '/':
            if( fomula[0] == '+' || fomula[0] == '*' || fomula[0] == '/' )
            { // 第一項不得為運算子，除表示負號的 '-'
                cout << "Error3-1: the beginning of the formula can not be a operand, except minus." ;
                legal = 0 ;
                return legal ;
            }
            legal = 1 ;
            break ;

        case '(':
            l_brac++ ;
            legal = 1 ;
            break ;

        case ')':
            if( fomula[0] == ')' ) // 第一項不得為右括
            {
                cout << "Error3-1: the beginning of the formula can not be a right bracket." ;
                legal = 0 ;
                return legal ;
            }
            r_brac++ ;
            legal = 1 ;
            break ;

        default:
            cout << "error2: " << c << " is not a legitimate character." ;
            legal = 0 ;
            return legal ;
        }
    }

    if( l_brac != r_brac ) // 括弧不成對
    {
        legal = 0 ;

        if(l_brac < r_brac )
            cout << "error1: there is one extra close bracket." ;
        else
            cout << "error1: there is one extra open bracket." ;

        return legal ;
    }

    /// 檢查語法錯誤
    for( int i = 0 ; i < fomula.size() - 1 ; i++ )
    {
        c = fomula[i] ;

        nextc = fomula[i + 1] ;

        switch( c )
        {
        case '0' ... '9':
            if( nextc == '(' ) // 數字後面不得為左括
            {
                cout << "Error3-2: lack of operand." ;
                legal = 0 ;
                return legal ;
            }
            else
                legal = 1 ;
            break ;

        case '+':
        case '-':
            if( nextc == ')' ) // 運算子後面不得為右括
            {
                cout << "Error3-1: it can not be ), after the operand." ;
                legal = 0 ;
                return legal ;
            }
            else if( nextc == '+' || nextc == '-' || nextc == '*' || nextc == '/' )
            { // +,-後面不得為運算子
                cout << "Error3-3: there is an extra operand." ;
                legal = 0 ;
                return legal ;
            }
            else
                legal = 1 ;
            break ;

        case '*':
            if( nextc == ')' ) // 運算子後面不得為右括
            {
                cout << "Error3-1: it can not be ), after the operand." ;
                legal = 0 ;
                return legal ;
            }
            else if( nextc == '+' || nextc == '*' || nextc == '/' )
            { // 運算子後面不得為運算子 除了負號
                cout << "Error3-3: there is an extra operand." ;
                legal = 0 ;
                return legal ;
            }
            else
                legal = 1 ;
            break ;

        case '/':
            if( nextc == '0' )
            {
                cout << "Error4: division by zero is undefined." ;
                legal = 0 ;
                return legal ;
            }
            else if( nextc == ')' ) // 運算子後面不得為右括
            {
                cout << "Error3-1: it can not be ), after the operand." ;
                legal = 0 ;
                return legal ;
            }
            else if( nextc == '+' || nextc == '*' || nextc == '/' )
            { // 運算子後面不得為運算子，除了代表負數的'-'
                cout << "Error3-3: there is an extra operand." ;
                legal = 0 ;
                return legal ;
            }
            else
                legal = 1 ;
            break ;

        case '(':
            if( nextc == ')') // 左括後面不得為右括
            {
                cout << "Error3-1: it is empty in the brackets." ;
                legal = 0 ;
                return legal ;
            }
            else if( nextc == '+' || nextc == '*' || nextc == '/' )
            { // 左括後面不得為'+' '*' '/' 可存在表示負號的 '-'
                cout << "Error3-1: it can not be an operand, after the (." ;
                legal = 0 ;
                return legal ;
            }

            legal = 1 ;
            break ;

        case ')':
            if( nextc == '(') // 右括後面不得為左括
            {
                cout << "Error3-1: it can not be (, after the )." ;
                legal = 0 ;
                return legal ;
            }
            else if( nextc >= '0' && nextc <= '9' ) // 右括後面不得為數字
            {
                cout << "Error3-1: it can not be a number, after the )." ;
                legal = 0 ;
                return legal ;
            }

            legal = 1 ;
            break ;
        }
    }

    return legal ;
}

void push( Stack * top, char character )
{
    Stack * temp = new Stack ;

    temp -> item = character ;
    temp -> next = top ;
    top = temp ;

}

void pop( Stack * top ) // 與一般定義的pop()不同
{                       // 需先使用getTopElement函式
    Stack * temp ;      // 再使用pop()才完整的pop

    temp = top ;
    top = top -> next ;

    delete temp ;
    temp = NULL ;

}

char getTopElement( Stack * top )
{
    return top -> item ;
}

bool IsNotEmpty( Stack * top )
{

    return top == NULL ? false : true ;
}


void getPostfix( string & infix )
{
    Stack * top = NULL;
    char character ;


    for( int i = 0; i < infix.size(); i++ )
    {
        character = infix[i] ;

        if( character >= '0' && character <= '9' )
        {
            cout << character ;

            if( !( infix[i + 1] >= '0' && infix[i + 1] <= '9' ) && ( i < infix.size() - 1 ) )
                cout << " , " ;

        }
        else if( character == '(' )
        {
            push( top, character ) ;
        }
        else if( character == '+' || character == '-' || character == '*' || character == '/' )
        {
            push( top, character ) ;

            if( getTopElement( top ) == '*' || getTopElement( top ) == '/' ) // 如果堆疊最上層是 * 或 /
            {                                                                // 優先度大於等於+ - * /

                pop( top ) ; // 要先輸出高優先度的運算子 所以跳過上一個push
                cout << getTopElement( top ) ;

                if( i < infix.size() - 1 )
                        cout << " , " ;

                push( top, character ) ;
            }
        }
        else if ( character == ')' )
        {
            while( getTopElement( top ) != '(' )
            {
                cout << getTopElement( top ) ;
                pop( top ) ;

                if( i < infix.size() - 1 )
                        cout << " , " ;

            }
            pop( top ) ; // 跳過左括

        }

    } //end for

     while( IsNotEmpty(top) )
     {
        cout << getTopElement( top ) ;
        pop( top ) ;
     }


} // end getPostfix( string & postfix)
