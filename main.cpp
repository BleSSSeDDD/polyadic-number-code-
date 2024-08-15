#include <iostream>
#include <locale.h>
#include "BigInt.h"
using namespace std;
BigInt* PolyodicCode(BigInt number, BigInt* bases, int size) {
	BigInt* res = new BigInt[size];
	for (int i = 0; i < size; i++) {
		res[i] = number / bases[i];
		number = number % bases[i];
	}
	return res;
}
bool PairCoPrime(BigInt a, BigInt b) {
	BigInt zero = BigInt(0);
	BigInt tmp;
	bool flag = 1;
	while (1) {
		if (a >= b) {
			a = a - b;
		}
		else {
			tmp = b;
			b = a;
			a = b;
		}
		if (b == zero) break;
	}
	return a == 1;
}
int NotCoPrime(BigInt* bases, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = i; j < n; j++) {
			if (PairCoPrime(bases[i], bases[j])) return 1;
		}
	}
	return 0;
}
void bubbleSort(BigInt arr[], int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				BigInt temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}
int main() {
	setlocale(LC_ALL, "Russian");
	BigInt* res;
	BigInt number;
	int size = 0;
	cout << "Введите количество оснований: ";
	cin >> size;
	if (size <= 0) {
		cout << "Количество оснований должно быть больше нуля!" << endl;
		return 1;
	}
	BigInt* bases = new BigInt[size];
	if (bases == NULL) {
		cout << "Memory error" << endl;
		return 1;
	}
	BigInt err = 1;
	cout << "Введите основания: " << endl;
	for (int i = 0; i < size; i++) {
		cin >> bases[i];
		err = err * bases[i];
	}
	if (err < 0) {
		err = err * -1;
	}
	if (NotCoPrime(bases, size)) {
		cout << "Основания должны быть взаимно просты! " << endl;
		return 1;
	}
	cout << "Введите число: ";
	cin >> number;
	if (number <= 0) {
		cout << "Число должно быть натуральным!" << endl;
		return 1;
	}
	if (number > err) {
		cout << "Слишком большое число! " << endl;
		return 1;
	}
	bubbleSort(bases, size);
	res = PolyodicCode(number, bases, size);
	cout << "Ответ: (";
	for (int i = 0; i < size; i++) {
			cout << res[i];
		if (i != size - 1) {
			cout << ", ";
		}
	}
	cout << ")" << endl;
	return 0;
}