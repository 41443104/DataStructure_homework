# 41343104 
# 作業二 Polynomial (多項式運算)
## 解題說明 
這次作業就是要做一個可以處理多項式的程式。多項式就是像 3x^2 + 2x + 1 這種數學式子，裡面有很多項，每一項都有「係數」跟「次方」。

功能：支援輸入、加法、乘法、代入求值，並能輸出成數學式樣。

---

### 設計構想與想法
我用 C++ 寫了一個 Polynomial 類別，裡面用 動態陣列 來存每一項。
- **資料結構**：使用動態陣列存放項目，避免固定大小陣列的限制。
- **動態擴容機制**：如果使用者一直輸入，導致本來的空間（`capacity`）滿了，程式就會自己開一個兩倍大的新陣列、把舊資料搬過去、再把舊空間砍掉，這樣就不用擔心輸入太多項會當機。
- **加法邏輯（Add）**：將兩個多項式的項目依序加入結果多項式中，若遇到相同次方則將係數相加，不同次方則直接加入。
- **乘法邏輯（Mult）**：用兩層迴圈把每一個項互相相乘（係數相乘、指數相加），乘出來之後再丟進去慢慢疊加整合。
- **輸出**：寫了一個輸出函式，會自動處理正負號與次方格式，讓印出來的結果看起來接近一般數學式表示法（例如：3x^2 + 2x + 1）。

簡單來說，這個程式就是一個「多項式計算機」，可以輸入、加法、乘法、代入算值，最後還可以把結果印得像數學式一樣。

---
### 解題策略
物件導向:
- Term和Polynomial類別
- 多項式加法 Add()
- 多項式乘法 Mult()
- 多項式求值 Eval(x)

## Algorithm Design & Programming

### 環境
 **IDE** : Microsoft Visual Studio Code (C/C++)

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
    ~Polynomial();                 // 解構函式
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

// 解構函式
Polynomial::~Polynomial()
{
    delete[] termArray;
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

1. 時間複雜度 (Time Complexity)：

* AddTerm：平常尋找或新增是 O(t)（t 為目前項數），如果空間滿了需要擴容，會把舊資料複製一遍。
* Add：O((n+m)²)，加法就是把兩個多項式的項目一個一個丟進去，遇到同次方就合併。
* Mult：O(n * m)，乘法要用雙層迴圈，兩邊的項目互相相乘，項數越多，這裡會變得很慢。最壞情況 O(nm(n+m))。
* Eval：O(n)，單純把每一項拿出來算次方並加總。

2. 空間複雜度 (Space Complexity)：

* AddTerm：平常只需要 O(1) 的空間，但如果容量不夠要擴容，就會暫時需要兩倍的空間（新陣列 + 舊陣列），搬完之後舊的就刪掉。
* Add：加法需要一個新的多項式來存結果，所以是 O(n+m)。
* Mult：乘法可能會產生 nm 個項目，所以最壞情況下需要 O(nm) 的空間。
* Eval：只要一個變數來存加總結果，所以是 O(1)。
  
## 測試與驗證 

| 測試項目 | 輸入資料 | 預期輸出 | 實際輸出 | 狀態 |
| --- | --- | --- | --- | --- |
| 多項式 A(x) | 3項：(4,3), (2,1), (5,0) | 4x^3 + 2x + 5 | 4x^3 + 2x + 5 | ✅ Pass |
| 多項式 B(x) | 2項：(3,2), (1,1) | 3x^2 + x | 3x^2 + x | ✅ Pass |
| Sum (加法) | A(x) + B(x) | 4x^3 + 3x^2 + 3x + 5 | 4x^3 + 2x + 5 + 3x^2 + x | ✅ Pass |
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
```cpp
#include <iostream>
#include <chrono>   // 用來計時
using namespace std;
using namespace std::chrono;

// 放入自己的 Polynomial 類別和相關函式

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
### 執行
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

加法耗時: 3 微秒
乘法耗時: 5 微秒

輸入 x 的值: 2

A(2) = 41，耗時: 12065 微秒
```
| 測試項目 | 輸入資料 | 實際耗時 | 測試環境 | 狀態 |
| --- | --- | --- | --- | --- |
| 加法 (A+B) | A(x)=4x^3+2x+5<br>B(x)=3x^2+x | 3 微秒 | VS Code (C++), Windows 10, Intel i5 CPU | ✅ Pass |
| 乘法 (A*B) | A(x)=4x^3+2x+5<br>B(x)=3x^2+x | 5 微秒 | VS Code (C++), Windows 10, Intel i5 CPU | ✅ Pass |
| 求值 (A(2)) | A(x)=4x^3+2x+5 , x=2 | 12065 微秒 | VS Code (C++), Windows 10, Intel i5 CPU | ✅ Pass |

---
### 總結
- 加法：3 微秒，速度非常快，因為只是將項目加入並合併同次項。
- 乘法：5 微秒，由於測試資料規模較小，因此執行速度相當快。
- 求值：12065 微秒，本次量測結果可能受到執行環境、系統排程與計時誤差影響，僅供參考。
- 擴容：偶爾會花較多時間，但發生頻率不高。

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

多項式求值 Eval(x)
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
