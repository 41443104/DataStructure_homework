#include <iostream>
#include <cmath>
#include <chrono>   // 用來計時
#include <cstdlib>  // 為了用 rand()
#include <ctime>    // 為了用 time()

using namespace std;
using namespace std::chrono;
class Polynomial;
class Term
{
private:
    float coef; // 係數項
    int exp;    // 次方項
    friend class Polynomial; // 讓Polynomial類別可以直接用
};

class Polynomial
{
private:
    Term* termArray; // 動態非零項陣列
    int capacity;    // 陣列容量
    int terms;       // 非零項數量
public:
    Polynomial();                     // 建構子
    void Input();                     // 輸入多項式函式
    void Display();                   // 輸出多項式函式
    float Eval(float n);              // 多項式求值
    Polynomial Add(const Polynomial& p);   // 多項式加法
    Polynomial Mult(const Polynomial& p);  // 多項式乘法
    void AddTerm(float coef, int exp);     // 新增項
};

// 建構子
Polynomial::Polynomial()
{
    capacity = 8;                     // 給一個初始容量
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
        if (exp < 0)
        {
            cout << "次方不能為負，去除此項 " << endl;
        }

        AddTerm(coef, exp);     // 呼叫AddTerm自動處理合併跟擴容

    }
}

// 輸出函式 (直接印出 A(x) 多項式的模樣)
void Polynomial::Display()
{
    if (terms == 0)
    {   // 沒有東西就輸出0
        cout << "0";
        return;
    }
    for (int i = 0; i < terms; i++) {
        float c = termArray[i].coef;
        int e = termArray[i].exp;

        // 處理正負號
        if (c > 0 && i > 0) cout << " + ";
        else if (c < 0) cout << " - ";

        cout << abs(c); // 輸出係數

        // 輸出變數與次方
        if (e > 0) {
            cout << "x";
            if (e > 1) {
                cout << "^" << e;
            }
        }
    }
}

// 多項式求值 (算P(n))
float Polynomial::Eval(float n)
{
    float total = 0;
    for (int i = 0; i < terms; i++)
    {
        total += termArray[i].coef * pow(n, termArray[i].exp);  // 每一項：coef * n^exp
    }
    return total;
}

// 多項式加法 (合併兩個多項式)
Polynomial Polynomial::Add(const Polynomial& p)
{
    Polynomial total;   // 新的多項式用來存結果
    // 先把"自己"的所有項目丟進 total
    for (int a = 0; a < terms; a++)
    {
        total.AddTerm(termArray[a].coef, termArray[a].exp);
    }
    // 再把"另一個多項式P"的所有項目丟進 total
    for (int b = 0; b < p.terms; b++)
    {
        total.AddTerm(p.termArray[b].coef, p.termArray[b].exp);
    }
    return total;   // 回傳合併完成的多項式
}

// 多項式乘法 (逐項相乘後合併同次項)
Polynomial Polynomial::Mult(const Polynomial& p)
{
    Polynomial total;   // 新的多項式 用來存結果
    for (int a = 0; a < terms; a++)
    {
        for (int b = 0; b < p.terms; b++)
        {
            float newcoef = termArray[a].coef * p.termArray[b].coef;
            int newexp = termArray[a].exp + p.termArray[b].exp;
            total.AddTerm(newcoef, newexp); // 把結果丟進去
        }
    }
    return total;
}

// 新增項（若次方相同則合併係數，並保持由大到小排序）
void Polynomial::AddTerm(float coef, int exp) {
    if (coef == 0) return;  // 係數為 0 不需要加入
    if (exp <= 0) return;
    // 檢查有沒有相同次方，有就直接相加係數
    for (int i = 0; i < terms; i++) {
        if (termArray[i].exp == exp) {
            termArray[i].coef += coef;
            return; // 找到同次項就更新 不新增新項
        }
    }

    // 如果容量不夠 放大兩倍
    if (terms >= capacity) {
        capacity = capacity * 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            temp[i] = termArray[i]; // 搬移舊的資料
        }
        delete[] termArray; // 刪掉舊的陣列
        termArray = temp;   // 指向新陣列
    }

    // 尋找由大到小排序的插入位置
    int insertIndex = terms;
    for (int i = 0; i < terms; i++) {
        if (exp > termArray[i].exp) {
            insertIndex = i;
            break;
        }
    }

    // 將後面的項目往後搬移
    for (int i = terms; i > insertIndex; i--) {
        termArray[i] = termArray[i - 1];
    }

    // 在正確的位置插入新項
    termArray[insertIndex].coef = coef;
    termArray[insertIndex].exp = exp;
    terms++;
}
int main() {
    // 亂數種子
    srand(time(0));

    cout << "=== 多項式自動效能與規模測試 (大範圍評測) ===" << endl;

    // 測試不同的項數規模
    int testSizes[] = { 10, 50, 100, 500 };

    for (int size : testSizes) {
        Polynomial p, q;

        // 自動亂數產生多項式 A(x) 與 B(x)
        for (int i = 0; i < size; i++) {
            float coef1 = (rand() % 10) + 1;
            int exp1 = rand() % (size * 2);
            p.AddTerm(coef1, exp1);

            float coef2 = (rand() % 10) + 1;
            int exp2 = rand() % (size * 2);
            q.AddTerm(coef2, exp2);
        }

        cout << "\n----------------------------------------" << endl;
        cout << "【測試規模】目標項數約: " << size << " 項" << endl;

        // 1. 測試加法時間
        auto start_add = high_resolution_clock::now();
        Polynomial sum = p.Add(q);
        auto end_add = high_resolution_clock::now();
        auto duration_add = duration_cast<microseconds>(end_add - start_add).count();
        cout << "-> 加法耗時: " << duration_add << " 微秒" << endl;

        // 2. 測試乘法時間
        if (size <= 500) {
            auto start_mult = high_resolution_clock::now();
            Polynomial prod = p.Mult(q);
            auto end_mult = high_resolution_clock::now();
            auto duration_mult = duration_cast<microseconds>(end_mult - start_mult).count();
            cout << "-> 乘法耗時: " << duration_mult << " 微秒" << endl;
        }
        else {
            cout << "-> 乘法耗時: (規模過大，略過避免計算過久)" << endl;
        }

        // 3. 測試求值時間 (以 x = 2 為例)
        auto start_eval = high_resolution_clock::now();
        float res = p.Eval(2);
        auto end_eval = high_resolution_clock::now();
        auto duration_eval = duration_cast<microseconds>(end_eval - start_eval).count();
        cout << "-> A(2) 求值耗時: " << duration_eval << " 微秒" << endl;
    }

    return 0;
}
