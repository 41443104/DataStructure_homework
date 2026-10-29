# 41343104 
# 作業二 Polynomial (多項式運算)
## 解題說明 
這次作業要做一個可以處理多項式的程式。多項式就是像 `3x^2 + 2x + 1` 這種數學式子，裡面有很多項，每一項都有「係數」跟「次方」。

簡單來說，這個程式就是一個「多項式計算機」，可以自行輸入數值讓程式計算出加法、乘法多項式然後代入數值，最後還可以把結果印得像數學式一樣。

功能：使用者輸入數值程式計算出加法、乘法多項式，然後代入求值，並能輸出成數學式樣。

---
## Algorithm Design & Programming

我用 C++ 寫了一個 Polynomial 類別，裡面用 動態陣列 來存每一項。
- **資料結構**：使用動態陣列存放項目，避免固定大小陣列的限制。
- **動態擴容機制**：如果使用者一直輸入，導致本來的空間（`capacity`）滿了，程式就會自己開一個兩倍大的新陣列、把舊資料搬過去、再把舊空間砍掉，這樣就不用擔心輸入太多項會當機。
- **Term類別**: 記住輸入的係數和次方。
- **Polynomial類別**: 用一堆term排成一個完整多項式。
- **多項式加法 Add()**: 將兩個多項式的項目依序加入結果多項式中，若遇到相同次方則將係數相加，不同次方則直接加入。
- **多項式乘法 Mult()**: 用兩層迴圈把每一個項互相相乘（係數相乘、指數相加），乘出來之後再丟進去慢慢疊加整合。
- **多項式求值 Eval(x)**: 把使用者輸入的數值帶進前面用出來的多項式，算出答案。
- **輸出**：寫了一個輸出函式，會自動處理正負號與次方格式，讓印出來的結果看起來接近一般數學式表示法（例如：3x^2 + 2x + 1）。

---
## 程式實作
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
    cout << "運算結果" << endl;
    cout << "A(x) = "; p.Display(); cout << endl;
    cout << "B(x) = "; q.Display(); cout << endl;
    cout << "A+B = "; sum.Display(); cout << endl;
    cout << "A*B = "; prod.Display(); cout << endl << endl;
    cout << "A(" << x << ") = " << p.Eval(x) << endl;
    cout << "B(" << x << ") = " << q.Eval(x) << endl;

    return 0;
}
```
---
## 測試與驗證

| **測試項目** | **輸入資料** | **預期輸出** | **實際輸出** | 
| :--- | :--- | :--- | :--- | 
| 多項式 A(x) | 3項：(4,3), (2,1), (5,0) | 4x^3 + 2x + 5 | 4x^3 + 2x + 5 |
| 多項式 B(x) | 2項：(3,2), (1,1) | 3x^2 + 1x | 3x^2 + 1x |
| Sum (加法) | A(x)+B(x) | 4x^3 + 3x^2 + 3x + 5 | 4x^3 + 3x^2 + 3x + 5 | 
| Product (乘法) | A(x) * B(x) | 12x^5 + 4x^4 + 6x^3 + 17x^2 + 5x | 12x^5 + 4x^4 + 6x^3 + 17x^2 + 5x | 
| Eval (求值) | 當 x = 2 時 A(2) | 41 | 41 |
| 特殊案例 1 | A(x) = 空多項式 (全部係數為 0) | A(x) = 0 | A(x) = 0 | 
| 特殊案例 2 | A(x) = (1,1)、(-1,1) | A(x) = 0 (同次項合併後係數為 0) | A(x) = 0 | 

### 測試輸入
```
輸入多項式 A(x)
請輸入多項式有幾項: 3
輸入第 1 項的係數與次方: 4 3
輸入第 2 項的係數與次方: 2 1
輸入第 3 項的係數與次方: 5 0

輸入多項式 B(x)
請輸入多項式有幾項: 2
輸入第 1 項的係數與次方: 3 2
輸入第 2 項的係數與次方: 1 1

輸入 x 的值: 2
```
### 運算結果
```
A(x) = 4x^3 + 2x + 5
B(x) = 3x^2 + 1x
A+B = 4x^3 + 3x^2 + 3x + 5
A*B = 12x^5 + 4x^4 + 6x^3 + 17x^2 + 5x
A(2) = 41
B(2) = 14
```
---
## 效能分析

### 1. `Term` 類別

#####  **時間複雜度**：O(1)
*  `Term` 只是純粹的資料容器，沒有複雜的演算法或迴圈。


#####  **空間複雜度**：O(1)
*  固定佔用一個 `float`（4 bytes）與一個 `int`（4 bytes）的記憶體空間。

---

### 2. Polynomial 類別與成員函式(`T` 代表多項式目前總共有幾項（Non-zero terms）)

`T` 代表多項式目前總共有幾項（Non-zero terms）。

`T1,T2`兩個輸入多項式的項數。

| 函式名稱 | 時間複雜度 (Time Complexity) | 空間複雜度 (Space Complexity) | 解釋 |
| :--- | :--- | :--- | :--- |
| **建構子 (`Polynomial`)** | $O(1)$ | $O(1)$ | 一開始設定初始值 8 個位置的空間，瞬間完成。 |
| **輸入 (`Input`)** | $O(n^2)$ | $O(n)$ | 每輸入一項都要檢查同次方並擴容，項數越多越慢。 |
| **輸出 (`Display`)** | $O(T)$ | $O(1)$ | 有幾項就照順序印幾項，項數越多印越久。 |
| **求值 (`Eval`)** | $O(T)$ | $O(1)$ | 把 $x$ 代進每一項算一次，有幾項就做幾次數學計算。 |
| **加法 (`Add`)** | $O((T_1 + T_2)^2)$ | $O(T_1 + T_2)$ | 逐項塞進新結果中（因為每次塞都要檢查合併）。 |
| **乘法 (`Mult`)** | $O(T_1 \times T_2 \times \text{總項數})$ | $O(T_1 \times T_2)$ | 兩個迴圈互相相乘，項數越多組合呈倍數成長。 |

---

### 總結

* **最耗記憶體的地方**：動態陣列（termArray），當項數超過容量時，程式會自動把陣列放大兩倍，空間隨項數成長（$O(T)$）。
* **最耗時間的地方**：AddTerm 的檢查機制（每次加入都要從頭檢查有沒有同次方），導致加法和乘法在項數多時會變比較慢。

---

## 效能量測 

為了觀察程式運算效率，本次利用 C++ `<chrono>` 函式庫進行高精度時間量測，分別測試多項式加法、乘法與求值功能的執行時間。

### 量測程式

```cpp
#include <iostream>
#include <cmath>
#include <chrono>   // 用來計時
#include <cstdlib>  // 為了用 rand()
#include <ctime>    // 為了用 time()

using namespace std;
using namespace std::chrono;

// 加上自己的程式

int main() {
    // 亂數種子
    srand(time(0));

    cout << "=== 多項式自動化效能評測程式 ===" << endl;

    // 設定要測試的不同項數規模
    int testSizes[] = { 10, 50, 100, 500, 1000 };

    for (int size : testSizes) {
        Polynomial p, q;

        // 自動隨機產生指定項數的多項式 p 與 q
        for (int i = 0; i < size; i++) {
            float coef1 = (rand() % 10) + 1;
            int exp1 = rand() % (size * 2);
            p.AddTerm(coef1, exp1);

            float coef2 = (rand() % 10) + 1;
            int exp2 = rand() % (size * 2);
            q.AddTerm(coef2, exp2);
        }

        cout << "\n----------------------------------------" << endl;
        cout << "【測試規模】目標項數: " << size << " 項" << endl;

        // 1. 測試加法耗時
        auto start_add = high_resolution_clock::now();
        Polynomial sum = p.Add(q);
        auto end_add = high_resolution_clock::now();
        auto duration_add = duration_cast<microseconds>(end_add - start_add).count();
        cout << "-> 加法耗時: " << duration_add << " 微秒" << endl;

        // 2. 測試乘法耗時（項數大於 500 時乘法會比較久，故超過 500 可自行決定是否略過）
        if (size <= 1000) {
            auto start_mult = high_resolution_clock::now();
            Polynomial prod = p.Mult(q);
            auto end_mult = high_resolution_clock::now();
            auto duration_mult = duration_cast<microseconds>(end_mult - start_mult).count();
            cout << "-> 乘法耗時: " << duration_mult << " 微秒" << endl;
        }

        // 3. 測試求值耗時 (以 x = 2 為例)
        auto start_eval = high_resolution_clock::now();
        float res = p.Eval(2);
        auto end_eval = high_resolution_clock::now();
        auto duration_eval = duration_cast<microseconds>(end_eval - start_eval).count();
        cout << "-> 求值耗時 (Eval): " << duration_eval << " 微秒" << endl;
    }

    cout << "\n=== 測試完畢 ===" << endl;
    return 0;
}
```
---
### 測試結果
```
=== 多項式自動效能與規模測試 (大範圍評測) ===

----------------------------------------
【測試規模】目標項數約: 10 項
-> 加法耗時: 4 微秒
-> 乘法耗時: 4 微秒
-> A(2) 求值耗時: 6 微秒

----------------------------------------
【測試規模】目標項數約: 50 項
-> 加法耗時: 16 微秒
-> 乘法耗時: 317 微秒
-> A(2) 求值耗時: 3 微秒

----------------------------------------
【測試規模】目標項數約: 100 項
-> 加法耗時: 39 微秒
-> 乘法耗時: 2160 微秒
-> A(2) 求值耗時: 6 微秒

----------------------------------------
【測試規模】目標項數約: 500 項
-> 加法耗時: 730 微秒
-> 乘法耗時: 107654 微秒
-> A(2) 求值耗時: 21 微秒
```
---
### 效能分析

- Add() 主要透過 AddTerm() 合併同次項，因此執行時間會隨項數增加而增加。
- Mult() 使用雙層迴圈逐項相乘，為整個程式中運算量最大的部分。
- Eval() 僅需要將各項代入並累加，因此執行效率最佳。
- 使用 `<chrono>` 能以微秒（microseconds）為單位進行量測，使結果更精確。

---

## 申論及開發報告

### Polynomial 類別  
**屬性包含：**
- `termArray`：用動態陣列儲存多項式的各項。
- `capacity`：目前陣列的容量，必要時會自動擴充。
- `terms`：實際儲存的項數。

**主要方法：**
- `Add()`：執行多項式加法。
- `Mult()`：執行多項式乘法。
- `Eval()`：代入數值求值。
- `AddTerm()`：新增項目，並自動合併同次項或擴充容量。
---

### 多項式加法 Add()  
- 將兩個多項式的項目逐一加入新多項式。  
- 若遇到相同次方，則將係數相加。  
- 若次方不同，則直接加入結果中。
- 最後回傳合併完成的多項式。

---

### 多項式乘法 Mult()  
- 使用兩個迴圈，把每一項相乘。  
- 計算方式：新係數 = 係數相乘，新次方 = 次方相加。  
- 呼叫 `AddTerm()` 將結果項加入，並自動合併同次項。  

程式邏輯範例：  
```cpp
for (int i = 0; i < terms; i++) {
    for (int j = 0; j < p.terms; j++) {
        float newCoef = termArray[i].coef * p.termArray[j].coef;
        int newExp = termArray[i].exp + p.termArray[j].exp;
        total.AddTerm(newCoef, newExp);
    }
}
```

---

### 多項式求值 Eval(x)
將輸入的 x 值代入多項式：

$$P(x) = \sum_i (coef_i \times x^{exp_i})$$

逐項計算後累加，得到最終結果。
 
### 程式限制與未來改進
目前程式已經實作多項式的輸入、輸出、加法、乘法及求值功能，但仍有一些可以改進的地方。

第一，termArray 使用原始指標管理動態記憶體，需要補上完整的記憶體管理機制，避免記憶體洩漏或複製物件時發生問題。

第二，係數使用 float 型別，若進行較多小數運算，可能產生浮點數精度誤差，因此可以考慮使用 double，並在判斷係數是否為零時加入適當的容許誤差。

第三，目前的 AddTerm() 需要逐項搜尋是否存在相同次方，當項目數量增加時，搜尋及插入的成本也會提高。未來可以考慮使用其他資料結構或改良演算法，提升大型多項式的運算效率。

第四，輸入功能可以進一步加強錯誤處理，例如檢查項數是否為負數、輸入是否為有效數字，以及次方是否符合規定。

透過這些改進，可以讓程式在正確性、記憶體管理及執行效率方面更加完善。

---

## 心得討論

1. 類別封裝的理解
   - 透過 Polynomial 類別設計，更加掌握物件導向的封裝概念，能將多項式運算邏輯集中管理。

2. 動態記憶體配置與擴容機制
   - 學會在容量不足時建立新陣列、搬移舊資料及刪除舊陣列。
   - 更理解記憶體管理的重要性，避免記憶體洩漏。

3. 同次項合併的重要性
   - 在 AddTerm() 中處理相同次方的合併，使結果更簡潔且正確。

4. 數學概念轉換成程式邏輯
   - Add()：實現多項式加法。
   - Mult()：實現多項式乘法。
   - Eval()：完成多項式求值。

5. 程式架構思維的提升
   - 學會將複雜問題拆分成多個函式與模組，提高程式可讀性與維護性。

6. 物件導向設計的好處
   - 提升程式的可讀性、可維護性與擴充性。
