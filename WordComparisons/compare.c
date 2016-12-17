#include <iostream>
#include <mpi.h>

bool equal(const unsigned char *c1, const unsigned char *c2)
{
	while (*c1 || *c2) {
		if (*c1++ != *c2++) {
			return false;
		}
	}
	return true;
}

int main()
{
	const unsigned char str1[] = "Hello, world!";
	const unsigned char str2[] = "Hello, there!";

	std::cout << "hello " << equal(str1,str1) << equal(str1,str2) << std::endl;
	return 0;
}
