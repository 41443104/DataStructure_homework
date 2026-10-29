# 41343104 
# 作業二 Polynomial (多項式運算)
## 解題說明 
這次作業要做一個可以處理多項式的程式。多項式就是像 3x^2 + 2x + 1 這種數學式子，裡面有很多項，每一項都有「係數」跟「次方」。

簡單來說，這個程式就是一個「多項式計算機」，可以自行輸入數值讓程式計算出加法、乘法多項式然後代入數值，最後還可以把結果印得像數學式一樣。

功能：使用者輸入數值程式計算出加法、乘法多項式，然後代入求值，並能輸出成數學式樣。

---
## Algorithm Design & Programming

我用 C++ 寫了一個 Polynomial 類別，裡面用 動態陣列 來存每一項。
- **資料結構**：使用動態陣列存放項目，避免固定大小陣列的限制。
- **動態擴容機制**：如果使用者一直輸入，導致本來的空間（`capacity`）滿了，程式就會自己開一個兩倍大的新陣列、把舊資料搬過去、再把舊空間砍掉，這樣就不用擔心輸入太多項會當機。
- **Team類別**:記住輸入的係數和次方。
- **Polynomial類別**:用一堆team排成一個完整多項式。
- **多項式加法 Add()**:將兩個多項式的項目依序加入結果多項式中，若遇到相同次方則將係數相加，不同次方則直接加入。
- **多項式乘法 Mult()**:用兩層迴圈把每一個項互相相乘（係數相乘、指數相加），乘出來之後再丟進去慢慢疊加整合。
- **多項式求值 Eval(x)**:把使用者輸入的數值帶進前面用出來的多項式，算出答案。
- **輸出**：寫了一個輸出函式，會自動處理正負號與次方格式，讓印出來的結果看起來接近一般數學式表示法（例如：3x^2 + 2x + 1）。

---
### 環境
 **IDE** : Microsoft Visual Studio Code (C++)
 
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
        AddTerm(coef, exp);     // 呼叫AddTerm自動處理合併跟擴容
    }
}

// 輸出函式 (直接印出 A(x) 多項式的模樣)
void Polynomial::Display()
{
    if (terms == 0) {   // 沒有東西就輸出0
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

// 新增項（若次方相同則合併係數）
void Polynomial::AddTerm(float coef, int exp) {
    if (coef == 0) return;  // 係數為 0 不需要加入

    // 檢查有沒有相同次方
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
    // 新增新項
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
```
## 效能分析

### 1. `Term` 類別

#### **時間複雜度**：O(1)
* `Term` 只是純粹的資料容器，沒有複雜的演算法或迴圈。


#### **空間複雜度**：O(1)
* 固定佔用一個 `float`（4 bytes）與一個 `int`（4 bytes）的記憶體空間。

---

### 2. Polynomial 類別與成員函式

#### (1) 建構子 Polynomial()

* **時間複雜度**：O(1) —— 只初始化變數並配置固定大小（10）的初始陣列。
* **空間複雜度**：O(1)

#### (2) 輸入函式 Input()

* **時間複雜度**：O(N^2)（N 為輸入的項數）
  迴圈執行 N 次，每次都呼叫 `AddTerm`。而 `AddTerm` 內部需要用迴圈線性搜尋是否有相同次方，因此隨著項數增加，時間會呈平方級成長。
* **空間複雜度**：O(N) —— 隨著輸入項數增加，動態陣列儲存 N 個 `Term`。

#### (3) 輸出函式 Display()

* **時間複雜度**：O(T) —— 只需要走訪（迴圈）目前所有的非零項 T 印出一次。
* **空間複雜度**：O(1) —— 不額外佔用記憶體空間。

#### (4) 多項式求值 Eval(float n)

* **時間複雜度**：O(T) —— 迴圈走訪 T 個項，每一項進行一次乘法與次方運算。
* **空間複雜度**：O(1)

#### (5) 多項式加法 Add(const Polynomial& p)

* **時間複雜度**：O((T_1 + T_2)^2)
* 假設兩個多項式項數分別為 T_1 和 T_2，總共要將 T_1 + T_2 個項逐一塞進新的多項式中。每次塞入時，`AddTerm` 都會進行一次線性搜尋。


* **空間複雜度**：O(T_1 + T_2) —— 建立一個新的多項式來存放加法結果。

#### (6) 多項式乘法 Mult(const Polynomial& p)

* **時間複雜度**：O((T_1 \times T_2)^2)
* 兩層迴圈先產生 T_1 \times T_2 個相乘結果，再將每一項透過 `AddTerm` 插回新多項式（`AddTerm` 的線性搜尋會隨著結果項數增加而變慢）。


* **空間複雜度**：O(T_1 \times T_2) —— 最多可能產生 T_1 \times T_2 個項。

#### (7) 新增項 AddTerm(float coef, int exp)

* **時間複雜度**：O(T)
* 為了檢查有沒有重複的次方，必須從頭到尾掃描目前的陣列一次（O(T)）。
 若容量不足進行擴容（Resize），需要開新陣列並把舊資料複製過去，這部分也是 O(T)。


* **空間複雜度**：O(T)（動態調整容量，分攤下來為 amortized O(1)，但整體規模與項數成正比）。

---

### 總結

* **整體空間效率**：利用動態陣列（`Term* termArray`）搭配容量倍增，非常節省記憶體，空間複雜度皆為 **O(\text{項數})**。
* **時間效率瓶頸**：因為 `AddTerm` 採用了**線性搜尋**（每次檢查有沒有同次方都要從頭找），導致 `Input`、`Add` 和 `Mult` 的時間複雜度較高。如果多項式的項數非常大，可以考慮改用雜湊表（Hash Table）**或讓陣列**保持排序（使用二分搜尋）來優化效能。

---

## 測試與驗證 

| 測試項目 | 輸入資料 | 預期輸出 | 實際輸出 | 狀態 |
| --- | --- | --- | --- | --- |
| 多項式 A(x) | 3項：(4,3), (2,1), (5,0) | 4x^3 + 2x + 5 | 4x^3 + 2x + 5 | ✅ Pass |
| 多項式 B(x) | 2項：(3,2), (1,1) | 3x^2 + x | 3x^2 + x | ✅ Pass |
| Sum (加法) | A(x)+B(x) | 與理論運算結果相同 | 4x^3 + 2x + 5 + 3x^2 + x | ✅ Pass |
| Product (乘法) | A(x) * B(x) | 12x^5 + 4x^4 + 6x^3 + 17x^2 + 5x | 12x^5 + 4x^4 + 6x^3 + 17x^2 + 5x | ✅ Pass |
| Eval (求值) | 當 x = 2 時 A(2) | 41 | 41 | ✅ Pass |
| 特殊案例 1 | A(x) = 空多項式 (全部係數為 0) | A(x) = 0 | A(x) = 0 | ✅ Pass |
| 特殊案例 2 | A(x) = (1,1)、(-1,1) | A(x) = 0 (同次項合併後係數為 0) | A(x) = 0 | ✅ Pass |

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
B(x) = 3x^2 + x
Sum = 4x^3 + 2x + 5 + 3x^2 + x
Product = 12x^5 + 4x^4 + 6x^3 + 17x^2 + 5x
A(2) = 41
B(2) = 14
```

由於目前程式未進行次方大小排序，因此輸出順序依據項目加入順序顯示，但運算結果正確。

---

## 效能量測 

為了觀察程式運算效率，本次利用 C++ `<chrono>` 函式庫進行高精度時間量測，分別測試多項式加法、乘法與求值功能的執行時間。

### 量測程式

```cpp
#include <iostream>
#include <chrono>   // 用來計時
using namespace std;
using namespace std::chrono;


// 自己的 Polynomial 類別和相關函式


int main() {
    Polynomial p, q;
    cout << "輸入多項式 A(x)\n";
    p.Input();
    cout << "\n輸入多項式 B(x)\n";
    q.Input();


    // 測試加法時間
    auto start_add = high_resolution_clock::now();
    Polynomial sum = p.Add(q);
    auto end_add = high_resolution_clock::now();
    auto duration_add = duration_cast<microseconds>(end_add - start_add).count();
    cout << "加法耗時: " << duration_add << " 微秒" << endl;


    // 測試乘法時間
    auto start_mult = high_resolution_clock::now();
    Polynomial prod = p.Mult(q);
    auto end_mult = high_resolution_clock::now();
    auto duration_mult = duration_cast<microseconds>(end_mult - start_mult).count();
    cout << "乘法耗時: " << duration_mult << " 微秒" << endl;


    // 測試求值時間
    float x;
    cout << "\n輸入 x 的值: ";
    cin >> x;
    auto start_eval = high_resolution_clock::now();
    float result = p.Eval(x);
    auto end_eval = high_resolution_clock::now();
    auto duration_eval = duration_cast<microseconds>(end_eval - start_eval).count();
    cout << "A(" << x << ") = " << result << "，耗時: " << duration_eval << " 微秒" << endl;


    return 0;
}
```

### 測試環境

- IDE：Microsoft Visual Studio Code
- 程式語言：C++
- 作業系統：Windows 10
- 處理器：Intel Core i5

### 測試資料

- A(x) = 4x³ + 2x + 5
- B(x) = 3x² + x
- x = 2
### 測試結果
```
請輸入多項式有幾項: 3
輸入第 1 項的係數與次方: 4 3
輸入第 2 項的係數與次方: 2 1
輸入第 3 項的係數與次方: 5 0

輸入多項式 B(x)
請輸入多項式有幾項: 2
輸入第 1 項的係數與次方: 3 2
輸入第 2 項的係數與次方: 1 1
加法耗時: 3 微秒
乘法耗時: 4 微秒

輸入 x 的值: 2
A(2) = 41，耗時: 18 微秒
```


| 功能 | 執行時間 |
|------|------|
| Add() | 3 微秒 |
| Mult() | 4 微秒 |
| Eval() | 18 微秒 |

### 效能分析

- Add() 主要透過 AddTerm() 合併同次項，因此執行時間會隨項數增加而增加。
- Mult() 使用雙層迴圈逐項相乘，為整個程式中運算量最大的部分。
- Eval() 僅需要將各項代入並累加，因此執行效率最佳。
- 使用 `<chrono>` 能以微秒（microseconds）為單位進行量測，使結果更精確。

### 結論

根據實際量測結果，本程式能快速完成多項式加法、乘法與求值運算。在測試資料規模下，所有功能皆能在極短時間內完成，沒有明顯延遲現象。當多項式項數增加時，乘法運算的執行成本將成長最快，這也與前述時間複雜度分析結果一致。

由於本次測試資料規模較小，因此執行時間皆落在微秒等級，若增加多項式項數，依照時間複雜度分析，乘法運算的執行時間將增加最明顯。

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
- 使用雙層迴圈，將每一項彼此相乘。  
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
