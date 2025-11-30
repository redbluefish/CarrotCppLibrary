#include "ThreadTools.h"

int main()
{
	cqueue<int> a(64);
	for (int k = 0; k < 68; k++)
	{
		a.push_back(32);
	}
	return 0;
}