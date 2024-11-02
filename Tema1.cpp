/*
*	Chira Andrei, gr. 30222
* 
*	Am implementat 4 algoritmi de sortare a unui sir:bubble sort, insertion sort, selection sort si o var a insertion sort-ului ce foloseste cautarea
*binara pentru a determina poz de inserare a elementului curent.
*	Pe cazul mediu, pentru care am efectuat 5 teste pentru fiecare dimensiune a sirului, cel mai bun algoritm este insertion sort cu cautare binara, chiar daca
* ca si numar de asignari selection sort are valoarea cea mai mica, ca numar de comparatii, tot insertion sort cu cautare binara e cel mai bun
*	Pe cazul favorabil, bubble sort e cel mai bun, avand cel mai mic numar de comparatii ca si insertion sort, dar fata de insertion face 0 asignari, deoarece nu
* este necesar sa se interchimbe elemente.
*	Pe cazul nefavorabil, selection sort e cel mai bun, dar foarte aproape se afla si insertion sort cu cautare binara, acesta avand cel mai mic numar de comparatii,
* dar un numar semnificativ mai mare de asignari decat selection sort.
*/

#include<stdio.h>
#include"Profiler.h"

Profiler p("Tema1");


void BubbleSort(int* v, int n)
{
	Operation opAssign = p.createOperation("bubble.assign", n);
	Operation opComp = p.createOperation("bubble.comp", n); 

	for (int i = n - 1; i >= 0; i--)
	{
		bool swapped = false;
		for (int j = 0; j < i; j++)
		{
			opComp.count();
			if (v[j] > v[j + 1])
			{
				opAssign.count(3);
				int aux = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux;
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
}

void InsertionSort(int* v, int n)
{
	Operation opAssign = p.createOperation("insert.assign", n);
	Operation opComp = p.createOperation("insert.comp", n);

	for (int i = 1; i < n; i++) {
		opAssign.count();
		int key = v[i];
		int j = i - 1;
		opComp.count();
		while ((v[j] > key) && j >= 0) {
			opComp.count();
			opAssign.count();
			v[j + 1] = v[j];
			j = j - 1;
		}
		opAssign.count();
		v[j + 1] = key;
	}
}

void SelectionSort(int* v, int n)
{
	Operation opAssign = p.createOperation("select.assign", n);
	Operation opComp = p.createOperation("select.comp", n);

	for (int i = 0; i < n - 1; i++) {
		int pmin = i;
		for (int j = i + 1; j < n; j++) {
			opComp.count();
			if (v[j] < v[pmin]) {
				pmin = j;
			}
		}
		if (pmin != i) {
			opAssign.count(3);
			int aux = v[i];
			v[i] = v[pmin];
			v[pmin] = aux;
		}
	}
}

int binarySearch(int* v, int left, int right, int value, Operation* opAssign, Operation* opComp)
{
	if (opComp)
		opComp->count();
	while (left < right)
	{
		int mid = (left + right) / 2;
		if (opComp)
			opComp->count();
		if (v[mid] == value)
			return mid;
		if (opComp)
			opComp->count();
		if (v[mid] < value) {
			left = mid + 1;
		}
		if (opComp)
			opComp->count();
		if (v[mid] > value) {
			right = mid - 1;
		}
	}
}

void binaryInsertionSort(int* v, int n)
{
	Operation opAssign = p.createOperation("binary.assign", n);
	Operation opComp = p.createOperation("binary.comp", n);

	for (int i = 1; i < n; i++) {
		int key = v[i];
		int j = i - 1;
		int poz = binarySearch(v, 0, j, key, &opAssign, &opComp);
		opComp.count();
		if (v[poz] < key)
			poz++;
		for (int k = i; k > poz; k--) {
			opAssign.count();
			v[k] = v[k - 1];
		}
		opAssign.count();
		v[poz] = key;
	}
}

void perf_avg(int caz)
{
	int v[10000], cop1[10000], cop2[10000], cop3[10000];
	for (int n = 100; n <= 10000; n += 100) {
		for (int i = 0; i < 5; i++) {
			FillRandomArray(v, n, 10, 50000, false, caz);
			CopyArray(cop1, v, n);
			CopyArray(cop2, v, n);
			CopyArray(cop3, v, n);

			BubbleSort(v, n);
			InsertionSort(cop1, n);
			SelectionSort(cop2, n);
			binaryInsertionSort(cop3, n);
		}
	}
	p.divideValues("bubble.assign", 5);
	p.divideValues("bubble.comp", 5);
	p.addSeries("bubble", "bubble.assign", "bubble.comp");

	p.divideValues("insert.assign", 5);
	p.divideValues("insert.comp", 5);
	p.addSeries("insert", "insert.assign", "insert.comp");

	p.divideValues("select.assign", 5);
	p.divideValues("select.comp", 5);
	p.addSeries("select", "select.assign", "select.comp");

	p.divideValues("binary.assign", 5);
	p.divideValues("binary.comp", 5);
	p.addSeries("binary", "binary.assign", "binary.comp");

	p.createGroup("assign", "bubble.assign", "insert.assign", "select.assign", "binary.assign");
	p.createGroup("comp", "bubble.comp", "insert.comp", "select.comp", "binary.comp");
	p.createGroup("total", "bubble", "insert", "select", "binary");

	p.showReport();
}

void perf_best_worst(int caz)
{
	int v[10000], cop1[10000], cop2[10000], cop3[10000];
	for (int n = 100; n <= 10000; n += 100) {
		FillRandomArray(v, n, 10, 50000, false, caz);
		CopyArray(cop1, v, n);
		CopyArray(cop2, v, n);
		CopyArray(cop3, v, n);

		BubbleSort(v, n);
		InsertionSort(cop1, n);
		SelectionSort(cop2, n);
		binaryInsertionSort(cop3, n);
	}

	p.addSeries("bubble", "bubble.assign", "bubble.comp");
	p.addSeries("insert", "insert.assign", "insert.comp");
	p.addSeries("select", "select.assign", "select.comp");
	p.addSeries("binary", "binary.assign", "binary.comp");

	p.createGroup("assign", "bubble.assign", "insert.assign", "select.assign", "binary.assign");
	p.createGroup("comp", "bubble.comp", "insert.comp", "select.comp", "binary.comp");
	p.createGroup("total", "bubble", "insert", "select", "binary");

	p.showReport();

}

void perf_all()
{
	p.reset("AVERAGE");
	perf_avg(UNSORTED);
	p.reset("BEST");
	perf_best_worst(ASCENDING);
	p.reset("WORST");
	perf_best_worst(DESCENDING);
}

void demo()
{
	int v[6] = { 7, 4, 3, 9, 12, 1 };
	int a[6] = { 4, 5, 1, 9, 7, 2 }, b[6] = { 45, 12, 32, 88, 3, 1 }, c[6] = {8, 1, 2, 5, 3, 2};
	

	BubbleSort(v, 6);
	printf("Bubble Sort: ");
	for (int i = 0; i < 6; i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	InsertionSort(a, 6);
	printf("InsertionSort: ");
	for (int i = 0; i < 6; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");

	SelectionSort(b, 6);
	printf("SelectionSort: ");
	for (int i = 0; i < 6; i++)
	{
		printf("%d ", b[i]);
	}
	printf("\n");

	binaryInsertionSort(c, 6);
	printf("binaryInsertionSort: ");
	for (int i = 0; i < 6; i++)
	{
		printf("%d ", c[i]);
	}
	printf("\n");

}

int main()
{
	demo();
	//perf_all();
	return 0;
}