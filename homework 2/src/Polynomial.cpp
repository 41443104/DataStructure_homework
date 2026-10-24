#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term
{
private:
    float coef; // 係數項
    int exp;    // 次方項
    friend class Polynomial;
};

class Polynomial
{
private:
    Term* termArray; // 非零項陣列
    int capacity;    // 陣列容量
    int terms;       // 非零項數量
public:
    Polynomial();                  // 建構子
    void Input();                  // 輸入多項式函式
    void Display();                // 輸出多項式函式
    float Eval(float n);           // 多項式求值
    Polynomial Add(const Polynomial& p);   // 多項式加法
    Polynomial Mult(const Polynomial& p);  // 多項式乘法
    void AddTerm(float coef, int exp);     // 新增項
};

// 建構子
Polynomial::Polynomial()
{
    capacity = 10;                  // 給一個初始容量
    terms = 0;
    termArray = new Term[capacity]; // 動態配置陣列
}

// 輸入函式 
void Polynomial::Input()
{
    int n;
    cout << "請輸入多項式有幾項: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        float coef;
        int exp;
        cout << "輸入第 " << i + 1 << " 項的係數與次方: ";
        cin >> coef >> exp;
        AddTerm(coef, exp);
    }
}

// 輸出函式 (直接印出 A(x) 的模樣)
void Polynomial::Display()
{
    if (terms == 0) {
        cout << "0";
        return;
    }
    for (int i = 0; i < terms; i++) {
        float c = termArray[i].coef;
        int e = termArray[i].exp;

        if (c > 0 && i > 0) cout << " + ";
        else if (c < 0) cout << " - ";

        cout << abs(c);
        if (e > 0) {
            cout << "x";
            if (e > 1) {
                cout << "^" << e;
            }
        }
    }
}

// 多項式求值
float Polynomial::Eval(float n)
{
    float total = 0;
    for (int i = 0; i < terms; i++)
    {
        total += termArray[i].coef * pow(n, termArray[i].exp);
    }
    return total;
}

// 多項式加法
Polynomial Polynomial::Add(const Polynomial& p)
{
    Polynomial total;
    for (int a = 0; a < terms; a++)
    {
        total.AddTerm(termArray[a].coef, termArray[a].exp);
    }
    for (int b = 0; b < p.terms; b++)
    {
        total.AddTerm(p.termArray[b].coef, p.termArray[b].exp);
    }
    return total;
}

// 多項式乘法
Polynomial Polynomial::Mult(const Polynomial& p)
{
    Polynomial total;
    for (int a = 0; a < terms; a++)
    {
        for (int b = 0; b < p.terms; b++)
        {
            float newcoef = termArray[a].coef * p.termArray[b].coef;
            int newexp = termArray[a].exp + p.termArray[b].exp;
            total.AddTerm(newcoef, newexp);
        }
    }
    return total;
}

// 新增項（若次方相同則合併係數）
void Polynomial::AddTerm(float coef, int exp) {
    if (coef == 0) return;

    // 檢查是否有相同次方
    for (int i = 0; i < terms; i++) {
        if (termArray[i].exp == exp) {
            termArray[i].coef += coef;
            return;
        }
    }

    // 如果容量不夠，簡單放大兩倍
    if (terms >= capacity) {
        capacity = capacity * 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            temp[i] = termArray[i];
        }
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = coef;
    termArray[terms].exp = exp;
    terms++;
}

int main()
{
    Polynomial p, q;

    // 輸入多項式 p 與 q
    cout << "輸入多項式 A(x)\n";
    p.Input();

    cout << "\n輸入多項式 B(x)\n";
    q.Input();

    // 計算加法和乘法
    Polynomial sum = p.Add(q);
    Polynomial prod = p.Mult(q);

    // 輸入 x
    float x;
    cout << "\n輸入 x 的值: ";
    cin >> x;

    // 輸出結果
    cout << "\n 運算結果 " << endl;
    cout << "A(x) = "; p.Display(); cout << endl;
    cout << "B(x) = "; q.Display(); cout << endl;
    cout << "Sum = "; sum.Display(); cout << endl;
    cout << "Product = "; prod.Display(); cout << endl;
    cout << "A(" << x << ") = " << p.Eval(x) << endl;
    cout << "B(" << x << ") = " << q.Eval(x) << endl;

    return 0;
}
