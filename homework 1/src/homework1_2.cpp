#include <iostream>
#include <string>
using namespace std;
string a;
void P(string s, const int m, const int n)
{
    if (m == n)
    {
        cout << "{ ";
        for (int i = 0; i < a.size(); i++)
        {
            cout << a[i];
        }
        cout << " }" << endl;
    }
    else
    {
        P(s, m + 1, n);
        a.push_back(s[m]);
        P(s, m + 1, n);
        a.pop_back();
    }
}
int main()
{
    string s;
    if (cin >> s)
    {
        int t = s.size();
        P(s, 0, t);
    }
    return 0;
}