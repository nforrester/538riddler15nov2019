#include <iostream>
#include <cassert>
#include <sstream>

int64_t gcd(int64_t a, int64_t b)
{
    assert(a > 0);
    assert(b > 0);

    while (a != b)
    {
        if (a < b)
        {
            int64_t c = a;
            a = b;
            b = c;
        }

        a -= b;
    }

    return a;
}

int64_t lcm(int64_t a, int64_t b)
{
    assert(a > 0);
    assert(b > 0);
    return a / gcd(a, b) * b;
}

struct Frac
{
    int64_t n;
    int64_t d;

    Frac(int64_t n): Frac(n, 1) {}

    Frac(int64_t nn, int64_t dd)
    {
        if (nn == 0)
        {
            n = 0;
            d = 1;
        }
        else
        {
            int64_t g = gcd(std::abs(nn), dd);
            n = nn / g;
            d = dd / g;
        }

        int64_t an = std::abs(n);
        assert(an < std::numeric_limits<int32_t>::max());
        assert(d < std::numeric_limits<int32_t>::max());
        assert(d >= 0);
    }

    Frac recip() const
    {
        int_fast8_t sign = n / std::abs(n);
        return Frac(sign * d, std::abs(n));
    }

    double to_double() const
    {
        return static_cast<double>(n) / static_cast<double>(d);
    }

    std::string show() const
    {
        std::ostringstream o;
        o << n << "/" << d << " (" << to_double() << ")";
        return o.str();
    }
};

Frac operator+(Frac const & a, Frac const & b)
{
    int64_t d = lcm(a.d, b.d);
    int64_t fa = d / a.d;
    int64_t fb = d / b.d;
    return Frac(fa * a.n + fb * b.n, d);
}

Frac operator-(Frac const & a, Frac const & b)
{
    int64_t d = lcm(a.d, b.d);
    int64_t fa = d / a.d;
    int64_t fb = d / b.d;
    return Frac(fa * a.n - fb * b.n, d);
}

Frac operator*(Frac const & a, Frac const & b)
{
    return Frac(a.n * b.n, a.d * b.d);
}

Frac operator/(Frac const & a, Frac const & b)
{
    return a * b.recip();
}

template<typename T>
T f(int n)
{
    if (n == 0)
    {
        return 0;
    }
    T t(0);
    for (int i = 1; i < n; ++i)
    {
        t = t + i + f<T>(i);
    }
    return (n + t) / (10 * n + 9);
}

int roll()
{
    return static_cast<double>(rand()) / RAND_MAX * 10;
}

double play()
{
    double d = 0.1;
    double s = 0;
    int p = 9;
    while (true)
    {
        int r = roll();
        if (r > p)
        {
            continue;
        }
        if (r == 0)
        {
            break;
        }
        s += d * r;
        d /= 10;
        p = r;
        if (d < 1e-30)
        {
            break;
        }
    }
    return s;
}

int main()
{
    std::cout << "f(0) = " << f<Frac>(0).show() << ", " << f<double>(0) << "\n";
    std::cout << "f(1) = " << f<Frac>(1).show() << ", " << f<double>(1) << "\n";
    std::cout << "f(2) = " << f<Frac>(2).show() << ", " << f<double>(2) << "\n";
    std::cout << "f(3) = " << f<Frac>(3).show() << ", " << f<double>(3) << "\n";
    std::cout << "f(4) = " << f<Frac>(4).show() << ", " << f<double>(4) << "\n";
    std::cout << "f(5) = " << f<Frac>(5).show() << ", " << f<double>(5) << "\n";
    std::cout << "f(6) = " << f<Frac>(6).show() << ", " << f<double>(6) << "\n";
    std::cout << "f(7) = " << f<Frac>(7).show() << ", " << f<double>(7) << "\n";
    std::cout << "f(8) = " << f<Frac>(8).show() << ", " << f<double>(8) << "\n";
    std::cout << "f(9) = " << f<Frac>(9).show() << ", " << f<double>(9) << "\n";

    double expected = f<Frac>(9).to_double();
    double average = play();
    int64_t n = 1;
    while (n < 1e9)
    {
        average *= static_cast<double>(n) / (n + 1);
        average += play() / n;
        ++n;
        if (n % static_cast<int64_t>(10e6) == 0)
        {
            std::cout << "After " << n << " games, average is " << average << " (error is " << expected - average << ")" << "\n";
        }
    }
    std::cout << "After " << n << " games, average is " << average << " (error is " << expected - average << ")" << "\n";
}
