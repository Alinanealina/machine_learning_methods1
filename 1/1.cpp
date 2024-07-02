#include <iostream>
#include <fstream>
#include <ctime>
#pragma warning(disable : 4996)
using namespace std;
// парз. окно с фикс. h, прямоуг K(x) = [r <= 1], 2 файл
const int N = 10000, r = 75, Nl = N * r / 100;
class obj
{
private:
    int mih, s;
    double S;

public:
    int c;
    obj() : mih(0), s(0), c(-1), S(-1)
    {

    }
    obj(int mih, int s) : mih(mih), s(s), c(-1), S(-1)
    {

    }
    obj(char* str) : S(-1)
    {
        sscanf(str, "%d,%d,%d", &mih, &s, &c);
    }

    void print()
    {
        cout << " mih = " << mih << ", s = " << s << ", c = " << c << endl;
    }

    void dist(obj a)
    {
        S = sqrt(pow(a.mih - mih, 2) + pow(a.s - s, 2));
    }

    int K(int h)
    {
        double rr = S / h;
        if (rr <= 1)
            return 1;
        else
            return 0;
    }
};

int classif(obj X[N], int h, int L, int R, int ind = -1)
{
    int i, y0 = 0, y1 = 0;
    for (i = L; i < R; i++)
    {
        if (i == ind)
            continue;
        if (X[i].c == 0)
            y0 += X[i].K(h);
        else
            y1 += X[i].K(h);
    }
    if (y0 > y1)
        return 0;
    else
        return 1;
}

void shuf(obj X[N], int L = 0, int R = N, int n = N, int rnd = 0)
{
    obj obm;
    int i, j;
    for (i = L; i < R; i++)
    {
        j = rnd + rand() % n;
        obm = X[i];
        X[i] = X[j];
        X[j] = obm;
    }
}

void sort(obj X[N])
{
    obj obm;
    int i, j, k;
    for (i = 0; i < N - 1; i++)
    {
        k = i;
        for (j = i + 1; j < N; j++)
        {
            if (X[k].c < X[j].c)
                k = j;
        }
        if (k != i)
        {
            obm = X[i];
            X[i] = X[k];
            X[k] = obm;
        }
    }
}

void fill_X(obj X[N])
{
    ifstream f("..\\..\\data2.csv");
    obj obm;
    char str[256];
    int i, j, k, L = Nl * (100 - r) / 100;
    for (i = -1; i < N; i++)
    {
        f >> str;
        if (i >= 0)
            X[i] = obj(str);
    }
    f.close();
    shuf(X);
    sort(X);
    for (i = Nl - L, j = N - 1; i < Nl; i++, j--)
    {
        obm = X[i];
        X[i] = X[j];
        X[j] = obm;
    }
    shuf(X, 0, Nl, Nl);
    shuf(X, Nl, N, N - Nl, Nl);
}

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    obj X[N];
    int i, j, n = 0, n0 = 0, h = 0;
    fill_X(X);
    do
    {
        h++;
        n0 = n;
        n = 0;
        for (i = 0; i < Nl; i++)
        {
            for (j = 0; j < Nl; j++)
            {
                if (i != j)
                    X[j].dist(X[i]);
            }
            if (classif(X, h, 0, Nl, i) != X[i].c)
                n++;
        }
        cout << " Количество ошибок обучающей выборки при h = " << h << ": " << n << endl;
    } while ((n == 0) || (n0 == 0) || (n0 > n));

    h--;
    cout << " h = " << h << ", " << n * 100 / Nl << "% ошибок" << endl;
    n = 0;
    for (i = Nl; i < N; i++)
    {
        for (j = Nl; j < N; j++)
        {
            if (i != j)
                X[j].dist(X[i]);
        }
        if (classif(X, h, Nl, N, i) != X[i].c)
            n++;
    }
    cout << " Количество ошибок тестовой выборки: " << n << ", " << n * 100 / (N - Nl) << "% ошибок";
    return 0;
}