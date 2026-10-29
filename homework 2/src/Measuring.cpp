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
    friend class Polynomial; // 讓 Polynomial 類別可以直接存取
};

class Polynomial
{
private:
    Term* termArray; // 動態非零項陣列
    int capacity;    // 陣列容量
    int terms;       // 非零項數量
public:
    Polynomial();                     // 建構子
    void Display();                   // 輸出多項式函式
    float Eval(float n);              // 多項式求值
    Polynomial Add(const Polynomial& p);   // 多項式加法
    Polynomial Mult(const Polynomial& p);  // 多項式乘法
    void AddTerm(float coef, int exp);     // 新增項（會自動依次方由大到小排序）
};

// 建構子
Polynomial::Polynomial()
{
    capacity = 8;                     // 初始容量
    terms = 0;
    termArray = new Term[capacity]; // 動態配置陣列
}

// 輸出函式 (印出多項式模樣)
void Polynomial::Display()
{
    if (terms == 0)
    {
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

// 多項式求值 (算 P(n))
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

// 多項式乘法 (逐項相乘後合併同次項)
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

// 新增項（若次方相同則相加，並保持次方由大到小排序）
void Polynomial::AddTerm(float coef, int exp) {
    if (coef == 0) return;  // 係數為 0 不需要加入

    // 1. 檢查有沒有相同次方，有就直接把係數相加
    for (int i = 0; i < terms; i++) {
        if (termArray[i].exp == exp) {
            termArray[i].coef += coef;
            return;
        }
    }

    // 2. 如果容量不夠，放大兩倍
    if (terms >= capacity) {
        capacity = capacity * 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            temp[i] = termArray[i];
        }
        delete[] termArray;
        termArray = temp;
    }

    // 3. 尋找正確的插入位置（保持次方由大到小排序）
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

    // 插入新項
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

        // 3. 測試求值時間 (以 x = 1.5 為例)
        auto start_eval = high_resolution_clock::now();
        float res = p.Eval(1.5f);
        auto end_eval = high_resolution_clock::now();
        auto duration_eval = duration_cast<microseconds>(end_eval - start_eval).count();
        cout << "-> A(1.5) 求值耗時: " << duration_eval << " 微秒" << endl;
    }

    return 0;
}
