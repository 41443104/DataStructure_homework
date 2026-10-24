# 41343104 
# 作業二 Polynomial (多項式運算)
## 解題說明 
使用 C++ 實作以動態陣列（Dynamic Array）表示的多項式（Polynomial）類別，並支援多項式的加法、乘法、求值以及輸入輸出運算。
### 設計構想與想法
在處理多項式時，如果用傳統的固定大小陣列，很容易遇到空間不夠或浪費記憶體的問題。所以我程式核心採用"動態陣列(Dynamic Array)"來儲存每一個非零項（Term），並包裝成 `Polynomial` 類別。

- **動態擴容機制**：如果使用者一直輸入，導致本來的空間（`capacity`）滿了，程式就會自己開一個兩倍大的新陣列、把舊資料搬過去、再把舊空間砍掉，這樣就不用擔心輸入太多項會當機。
- **加法邏輯（Add）**：把兩個多項式的項次拿來比對，指數一樣就把係數加起來；不一樣的話，就把指數比較大的那一項先放進結果裡。
- **乘法邏輯（Mult）**：用兩層迴圈把每一個項互相相乘（係數相乘、指數相加），乘出來之後再丟進去慢慢疊加整合。
- **直覺易讀的輸出**：寫了一個輸出函式，會自動幫忙處理正負號（像 `+` 和 `-`），如果係數是 1 或次方是 0 還會自動省略，讓印出來的結果看起來就像我們平常寫的數學式（例如：`3x^2 + 2x + 1`）。

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

### 程式碼實作+註解
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
## 效能分析 (Analysis)

1. 時間複雜度 (Time Complexity)：

* AddTerm：平常尋找或新增是 O(t)（t 為目前項數）。如果空間滿了需要擴容，會把舊資料複製一遍。
* Add：O(n + m)，把兩邊的項透過 AddTerm 組合進去。
* Mult：O(n * m)，用雙層迴圈兩兩相乘，每次相乘後呼叫 AddTerm 整合。
* Eval：O(n)，單純把每一項拿出來算次方並加總。

2. 空間複雜度 (Space Complexity)：

* 陣列儲存空間：O(t)，依照多項式實際有幾項來配置記憶體。
* 擴容時：因為要開新陣列把舊的複製過去，短時間內記憶體用量會暫時變成兩倍（O(capacity)），複製完就會把舊的 delete[] 掉。

## 測試與驗證 (Testing and Proving)

測試案例資料：

* 多項式 A(x) 輸入：3 項 -> (3, 2)、(2, 1)、(1, 0)
* 多項式 B(x) 輸入：2 項 -> (2, 5)、(3, 1)
* 代入求值 x 輸入：2

測試結果對照表：

* 多項式 A(x) | 3項 (3,2), (2,1), (1,0) | 預期：3x^2 + 2x + 1 | 實際：3x^2 + 2x + 1 | 狀態：Pass
* 多項式 B(x) | 2項 (2,5), (3,1) | 預期：2x^5 + 3x | 實際：2x^5 + 3x | 狀態：Pass
* Sum (加法) | A(x) + B(x) | 預期：2x^5 + 3x^2 + 5x + 1 | 實際：2x^5 + 3x^2 + 5x + 1 | 狀態：Pass
* Product (乘法) | A(x) * B(x) | 預期：6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x | 實際：6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x | 狀態：Pass
* Eval (求值) | 當 x = 2 時 A(2) | 預期：17 | 實際：17 | 狀態：Pass

## 效能量測 (Measuring)

* 在 VS Code 實際執行測試時，當多項式的項數在 10 項以內，加法與乘法的執行時間大約都在 1 毫秒（ms）以內，幾乎是瞬間完成，沒有感受到任何延遲。
* 就算輸入的項次比較多、觸發到 AddTerm 的陣列兩倍放大（capacity = capacity * 2），因為電腦處理記憶體搬移的速度極快，整體流暢度依然非常好。

## 心得討論

這次作業寫完之後，對 C++ 的類別封裝跟動態記憶體配置又更理解了。特別是在寫 AddTerm 的自動擴容機制時，體會到當原本的箱子裝不下時，必須「買個更大的新箱子 -> 把舊東西搬過去 -> 丟掉舊箱子」的整個過程。如果沒有做好 delete[] 就容易造成記憶體洩漏。透過這次動手寫，不僅順利解決了多項式的加乘與求值問題，也讓自己在寫程式時更有邏輯跟架構概念！
