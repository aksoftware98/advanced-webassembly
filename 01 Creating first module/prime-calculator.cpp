
#include <iostream>


int IsPrime(int value)
{
    if (value == 2)
    {
        return 1;
    }

    if (value <= 1 || value % 2 == 0)
    {
        return 0;
    }

    for (int i = 3; (i * i) <= value; i += 2)
    {
        if (value % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int start = 3;
    int end = 10000;

    std::cout << "Print all the primate numbers between " << start << " and " << end << std::endl;

    for (int i = start; i <= end; i += 2)
    {
        if (IsPrime(i) == 1)
        {
            std::cout << i << " is prime" << std::endl;
        }
    }
}