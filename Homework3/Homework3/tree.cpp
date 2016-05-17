
using namespace std;

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 < 0)
		n1 = 0;
	if (n2 < 0)
		n2 = 0;
	if (n2 == 0)//this condition is only met if there were matches to be made
		return 1;
	if (n1 == 0)//nothing left in the first array to check
		return 0;
	if (a1[0] == a2[0])//i want to check for repeats, so i advance one and try another
		return (countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1) + countIncludes(a1 + 1, n1 - 1, a2, n2));
	else
		return (countIncludes(a1 + 1, n1 - 1, a2, n2));//no find in this case so I advance a1
}

//FOR THE SECOND PART OF THIS PROBLEM


// Exchange two doubles
void exchange(double& x, double& y)//GIVEN METHOD
{
	double t = x;
	x = y;
	y = t;
}

void separate(double a[], int n, double separator, int& firstNotGreater, int& firstLess)
{//GIVEN METHOD
	if (n < 0)
		n = 0;
	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > separator
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == separator
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < separator

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < separator)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > separator)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)//following example of a mergesort...
{
	if (n <= 1)
		return;  // This is not always correct.
	int fng = 0;
	int less = 0;
	separate(a, n, a[n - 1], fng, less);//makes a (greater than a[n-1], equal to a[n-1], less than a[n-1] 
	order(a, fng);//"sorts" the values until the a[n-1]
	order(a + less, n - less);//"sorts" the second half
}