#include <iostream>
#include <cmath>
using namespace std;

struct Term {
    float coef; // 係數
    int exp;    // 次方
};

class Polynomial {
private:
    Term termArray[50]; // 直接用固定大小陣列
    int terms;          // 非零項數量
public:
    Polynomial() {
        terms = 0;
    }

    void AddTerm(float coef, int exp) {
        // 檢查是否有相同次方
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                return;
            }
        }
        // 新增一個項目
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    float Eval(float x) {
        float total = 0;
        for (int i = 0; i < terms; i++) {
            total += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return total;
    }

    Polynomial Add(const Polynomial& p) {
        Polynomial total;
        for (int i = 0; i < terms; i++) {
            total.AddTerm(termArray[i].coef, termArray[i].exp);
        }
        for (int j = 0; j < p.terms; j++) {
            total.AddTerm(p.termArray[j].coef, p.termArray[j].exp);
        }
        return total;
    }

    Polynomial Mult(const Polynomial& p) {
        Polynomial total;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < p.terms; j++) {
                float newCoef = termArray[i].coef * p.termArray[j].coef;
                int newExp = termArray[i].exp + p.termArray[j].exp;
                total.AddTerm(newCoef, newExp);
            }
        }
        return total;
    }

    void Print() {
        for (int i = 0; i < terms; i++) {
            cout << termArray[i].coef;
            if (termArray[i].exp > 0) {
                cout << "x";
                if (termArray[i].exp > 1) {
                    cout << "^" << termArray[i].exp;
                }
            }
            if (i < terms - 1) cout << " + ";
        }
    }
};

int main() {
    Polynomial p, q;
    int n;

    // 輸入 A(x)
    cout << "請輸入多項式 A(x) 有幾項: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        float coef;
        int exp;
        cout << "輸入第 " << i + 1 << " 項的係數與次方: ";
        cin >> coef >> exp;
        p.AddTerm(coef, exp);
    }

    // 輸入 B(x)
    cout << "請輸入多項式 B(x) 有幾項: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        float coef;
        int exp;
        cout << "輸入第 " << i + 1 << " 項的係數與次方: ";
        cin >> coef >> exp;
        q.AddTerm(coef, exp);
    }

    // 計算
    Polynomial sum = p.Add(q);
    Polynomial prod = p.Mult(q);

    float x;
    cout << "輸入 x 的值: ";
    cin >> x;

    // 輸出
    cout << "A(x) = "; p.Print(); cout << endl;
    cout << "B(x) = "; q.Print(); cout << endl;
    cout << "Sum = "; sum.Print(); cout << endl;
    cout << "Product = "; prod.Print(); cout << endl;
    cout << "A(" << x << ") = " << p.Eval(x) << endl;
    cout << "B(" << x << ") = " << q.Eval(x) << endl;

    return 0;
}
