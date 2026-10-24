# 41343104 
# 作業二 Polynomial (多項式運算)
## 解題說明 
使用 C++ 實作以動態陣列（Dynamic Array）表示的多項式（Polynomial）類別，並支援多項式的加法、乘法、求值以及輸入輸出運算。
### 設計構想與想法
在處理多項式時，如果用傳統的固定大小陣列，很容易遇到空間不夠或浪費記憶體的問題。所以我程式核心採用**動態陣列（Dynamic Array）**來儲存每一個非零項（Term），並包裝成 `Polynomial` 類別。

- **動態擴容機制**：當輸入的項數超過目前陣列的容量（`capacity`）時，程式會自動開闢一倍大的新記憶體空間、把舊資料搬過去、釋放舊空間，確保可以無限輸入。
- **加法邏輯（Add）**：遍歷兩個多項式的項次，若指數相同則合併係數；若不同則把指數較大的先放入結果中。
- **乘法邏輯（Mult）**：透過雙層迴圈把每一個項互相相乘（係數相乘、指數相加），再逐步累加進結果多項式。
- **直覺易讀的輸出**：實作了 `Display()` 函式，能妥善處理正負號、省略係數為 1 或指數為 0 的排版，讓印出來的結果像正常的數學式（例如：`3x^2 + 2x + 1`）。

### 目的
- 可以接收多項式的輸入（係數與指數，有可能會有未排序或重複指數）
- 計算多項式加法與乘法（需要合併同次項並移除係數為 0 的項）
- 對任意 x 值進行代入求值
- 以我們可閱讀的格式輸出多項式（處理 1 與 -1、整數係數顯示）
### 解題策略
物件導向:

- Term和Polynomial類別
- 多項式加法 Add()
- 多項式乘法 Mult()
- 多項式求值 Eval(x)

## Algorithm Design & Programming

### 開發環境
- **IDE**: Microsoft Visual Studio Code (C/C++)

### 程式碼實作
```cpp
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
