#include <iostream>
#include <cassert>
#include <vector>

#include "scopedptr.h"

int main() {
	
	int* p1 = new int(10);
	ScopedPtr<int>sp(p1);
	sp.Release();
	delete p1;
	int x = 2;

	return 0;
}