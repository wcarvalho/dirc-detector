#include <iostream>

using namespace std;

/*================================================================================================
Get critical angle for total internal reflection
================================================================================================*/

void TabToLevel(int level)
{
	int i;
	for (i = 0; i < level-1; i++)
	{
		cout << "\t";
	}
}