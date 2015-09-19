#include <omp.h>
#include <iostream>
#include <limits>
#include <math.h>

using namespace std;


// Using long division to test if given number is perfect square
// (i.e. square root of the number is an integer)
// http://www.math-only-math.com/square-root-of-a-perfect-square-by-using-the-long-division-method.html
//
// Returns the square root if it is a perfect square. Zero otherwise. (Zero also returns zero)
long long perfectSquare(long long num)
{
    // obvious cases
    int lastDigit = num % 10;
    if (num <= 0 || lastDigit == 2 || lastDigit == 3 || lastDigit == 7 || lastDigit == 8)
    {
        return 0;
    }

    long long section = 1;
    long long pow100 = 1;
    while (num / pow100 >= 100)
    {
        section++;
        pow100 *= 100;
    }

    long long period = num / pow100;  // first period
    long long remainder = num % pow100;
    long long quotient = 0;

    while (section > 0)
    {
        section--;
        pow100 /= 100;

        long long divisor;
        int newQuotientDigit;
        long long multi;
        for (newQuotientDigit = 9, divisor = quotient * 20 + newQuotientDigit; newQuotientDigit >= 0; newQuotientDigit--, divisor--)
        {
            multi = divisor * newQuotientDigit;
            if (period >= multi)
            {
                break;
            }
        }
        if (newQuotientDigit < 0)
        {
            return 0;
        }

        long long period_remain = period - multi;
        quotient = quotient * 10 + newQuotientDigit;

        if (section > 0)
        {
            period = period_remain * 100 + remainder / pow100;
            remainder = remainder % pow100;
        }
        else if (section == 0 && period_remain == 0 && remainder == 0)
        {
            return quotient;
        }
    }

    return 0;
}


int main(int argc, char* args[])
{
    #pragma omp parallel for
    //for (long long i = LLONG_MAX; i >= 0LL; i--)
    for (long long i = 2147483647LL; i >= 0LL; i--)
    {
        long long result = perfectSquare(i);
        long double sqResult = sqrtl(i);
        if ((result == 0 && sqResult - floor(sqResult) > 1e-8) ||
            (result == sqResult))
        {
            //cout << i << ": OK" << endl;
        }
        else
        {
            cout << i << ": Failed" << endl;
            cout << "perfectSquare returned " << result << " while sqrtl returned " << sqResult << endl;
        }
    }

    #pragma omp parallel for
    for (long long i = 0; i < 2147483647LL; i++)
    {
        long long square = i * i;
        if (perfectSquare(square) != i)
        {
            cout << i << ": Failed" << endl;
        }
        else
        {
            //cout << i << ": OK" << endl;
        }
    }


    return 0;
}

