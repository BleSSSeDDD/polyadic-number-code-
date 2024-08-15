#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <cstdint> 
const int Osn = 1000;
int sp;
class BigInt {
public:
 int* data;
 int size;
 bool negative = false;
 BigInt() {
 data = new int[1000];
 std::fill(data, data + 1000, 0);
 size = 0;
 negative = false;
 }
 BigInt(int num) {
 data = new int[1000];
 std::fill(data, data + 1000, 0);
 size = 0;
 negative = (num < 0);
 num = std::abs(num);
 while (num > 0) {
 data[++size] = num % Osn;
 num /= Osn;
 }
10
 }
 BigInt(const BigInt& other) {
 size = other.size;
 negative = other.negative;
 
 data = new int[1000];
 std::fill(data, data + 1000, 0);
 std::memcpy(data, other.data, (size + 1) * sizeof(int));
 }
 
 BigInt(BigInt&& other) noexcept {
 
 size = other.size;
 negative = other.negative;
 data = other.data;
 
 other.size = 0;
 other.negative = false;
 other.data = nullptr;
 }
 
 BigInt& operator=(const BigInt& other) {
 if (this != &other) { 
 delete[] data;
 size = other.size;
 negative = other.negative;
 
 data = new int[1000];
 std::fill(data, data + 1000, 0);
 for (int i = 0; i <= size; i++) {
 data[i] = other.data[i];
 }
 }
 return *this;
 }
11
 
 BigInt& operator=(BigInt&& other) noexcept {
 if (this != &other) { 
 delete[] data;
 
 size = other.size;
 negative = other.negative;
 data = other.data;
 
 other.size = 0;
 other.negative = false;
 other.data = nullptr;
 }
 return *this;
 }
 void Negate() {
 negative = !negative;
 }
 friend std::istream& operator>>(std::istream& is, BigInt& num) {
 num = BigInt();
 char ch;
 is.get(ch);
 if (ch == '-') {
 num.negative = true;
 is.get(ch);
 }
 while (ch < '0' || ch > '9') {
 is.get(ch);
 }
 while (ch >= '0' && ch <= '9') {
 for (int i = num.size; i >= 1; i--) {
 num.data[i + 1] += num.data[i] * 10 / Osn;
12
 num.data[i] = num.data[i] * 10 % Osn;
 }
 num.data[1] = num.data[1] + (ch - '0');
 if (num.data[num.size + 1] > 0) {
 num.size++;
 }
 is.get(ch);
 }
 return is;
 }
 friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
 if (num.negative) {
 os << "-";
 }
 std::string ls = std::to_string(Osn / 10);
 if (num.size == 0) {
 os << "0";
 return os;
 }
 os << num.data[num.size];
 for (int i = num.size - 1; i >= 1; i--) {
 std::string s = std::to_string(num.data[i]);
 while (s.length() < ls.length()) {
 s = "0" + s;
 }
 os << s;
 }
 return os;
 }
 BigInt operator*(const BigInt& B) const {
13
 BigInt result;
 if (negative && !B.negative || B.negative && !negative)
 {
 result.negative = true;
 }
 else
 {
 result.negative = false;
 }
 long long dv;
 for (int i = 1; i <= size; i++)
 {
 for (int j = 1; j <= B.size; j++)
 {
 dv = long long int(data[i]) * B.data[j] + result.data[i + j - 1];
 result.data[i + j] += int(dv / Osn);
 result.data[i + j - 1] = int(dv % Osn);
 }
 }
 result.size = size + B.size;
 while (result.size > 1 && result.data[result.size] == 0)
 {
 result.size--;
 }
 return result;
 }
 BigInt operator*(int K) {
 BigInt result;
 if ((K > 0 && negative) || (K < 0 && !negative))
 {
 result.negative = true;
 }
 else if (K < 0 && negative)
 {
 result.negative = false;
 }
 K = std::abs(K);
14
 for (int i = 0; i <= size; i++)
 {
 result.data[i + 1] = (int(data[i]) * K + result.data[i]) / Osn;
 result.data[i] = (int(data[i]) * K + result.data[i]) % Osn;
 if (result.data[i + 1] > 0)
 {
 result.size = size + 1;
 }
 else
 {
 result.size = size;
 }
 }
 return result;
 }
 BigInt operator+(BigInt other) {
 BigInt result;
 BigInt b = other;
 int sum;
 if ((!negative && !other.negative) || (negative && other.negative)) {
 int carry = 0;
 for (int i = 1; i <= std::max(size, other.size); i++) {
 sum = (data[i] + other.data[i] + carry);
 carry = sum / Osn;
 result.data[i] = sum % Osn;
 if (carry != 0)
 {
 result.size = size + 1;
 }
 else
 {
 result.size = size;
 }
 }
 while (result.size > 1 && result.data[result.size] == 0) {
 result.size--;
 }
 result.negative = negative;
 }
 else {
15
 BigInt temp = *this;
 temp.Negate();
 result = other - temp;
 }
 return result;
 }
 BigInt operator+(int num) const {
 BigInt result = *this;
 BigInt numBigInt;
 numBigInt.data[0] = std::abs(num);
 numBigInt.size = 0;
 numBigInt.negative = (num < 0);
 result = result + numBigInt;
 return result;
 }
 BigInt operator-(BigInt B) const {
 BigInt A;
 if (*this > B)
 {
 A = *this;
 }
 else
 {
 A = B;
 B = *this;
 }
 if ((negative && B.negative) || (!negative && B.negative))
 {
 return (A + B);
16
 }
 int i, j, sd = 0;
 for (i = 1; i <= size; i++)
 {
 int cur = int(A.data[i]) - int(B.data[i]) - sd;
 if (cur < 0)
 {
 A.data[i] = cur + Osn;
 sd = 1;
 }
 else
 {
 A.data[i] = cur;
 sd = 0;
 }
 }
 i = A.size;
 while (i > 1 && A.data[i] == 0)
 {
 i--;
 }
 A.size = i;
 return A;
 }
 bool operator==(const BigInt& B) const {
 
 for (int i = size; i >= 0; i--) {
 if (data[i] != B.data[i]) {
 return false;
 }
 }
 return true;
 }
 bool operator==(int right) const {
 BigInt temp;
 temp.data[0] = right;
17
 temp.size = 0;
 temp.negative = (right < 0);
 return (*this == temp);
 }
 bool operator!=(const BigInt& B) const {
 return !(*this == B);
 }
 bool operator<(const BigInt& right) const {
 if (negative && !right.negative) {
 return true;
 }
 else if (!negative && right.negative) {
 return false;
 }
 else if (negative && right.negative) {
 if (size != right.size) {
 return size < right.size;
 }
 else {
 for (int i = size; i >= 0; i--) {
 if (data[i] != right.data[i]) {
 return data[i] < right.data[i];
 }
 }
 return false;
 }
 }
 else {
 if (size != right.size) {
 return size < right.size;
 }
 else {
 for (int i = size; i >= 0; i--) {
 if (data[i] != right.data[i]) {
 return data[i] < right.data[i];
 }
 }
 return false;
 }
 }
18
 }
 bool operator<=(const BigInt& right) const {
 return (*this < right || *this == right);
 }
 bool operator>(const BigInt& right) const {
 return !(*this <= right);
 }
 bool operator>=(const BigInt& right) const {
 return !(*this < right);
 }
 void Sub(BigInt& A, BigInt B, int sp)
 {
 int i, j;
 for (i = 1; i <= B.size; i++)
 {
 A.data[i + sp] -= B.data[i];
 j = i;
 while (A.data[j + sp] < 0 && j <= A.size)
 {
 A.data[j + sp] += Osn;
 A.data[j + sp + 1]--;
 j++;
 }
 }
 i = A.size;
 while (i > 1 && A.data[i] == 0) 
 {
 i--;
 }
 A.size = i;
 }
 char More(const BigInt& A, const BigInt& B, int sdvig) {
 int i;
 if (A.size > (B.size + sdvig)) {
 return 0;
 }
 else if (A.size < (B.size + sdvig)) {
 return 1;
19
 }
 else {
 i = A.size;
 while (i > sdvig && A.data[i] == B.data[i - sdvig]) {
 i--;
 }
 if (i == sdvig) {
 for (i = 1; i <= sdvig; i++) {
 if (A.data[i] > 0) {
 return 0;
 }
 }
 return 2;
 }
 else {
 return (A.data[i] < B.data[i - sdvig]) ? 1 : 0;
 }
 }
 }
 long int FindBin(BigInt& Ost, BigInt& B, int sp) {
 int Down = 0;
 int Up = Osn;
 BigInt C;
 int K;
 while (Up - 1 > Down) {
 K = ((Up + Down) / 2);
 C = B * K;
 switch (More(Ost, C, sp)) {
 case 0:
 Down = (Down + Up) / 2;
 break;
 case 1:
 Up = (Up + Down) / 2;
 break;
 case 2:
 Up = (Up + Down) / 2;
 Down = Up;
 break;
 }
 }
20
 K = ((Up + Down) / 2);
 C = B * K;
 if (More(Ost, C, 0) == 0) {
 Sub(Ost, C, sp);
 }
 else {
 Sub(C, Ost, sp);
 Ost = C;
 }
 return (Up + Down) / 2;
 }
 void MakeDel(BigInt A, BigInt B, BigInt& Res, BigInt& Ost)
 {
 int sp;
 Ost = A;
 sp = A.size - B.size;
 if (More(A, B, sp) == 1)
 {
 sp--;
 }
 Res.size = sp + 1;
 while (sp >= 0)
 {
 Res.data[sp + 1] = FindBin(Ost, B, sp);
 sp--;
 }
 }
 BigInt operator%(BigInt& B)
 {
 BigInt Res, Ost;
 if ((negative && B.negative) || !(negative && B.negative))
 {
 Res.negative = false;
 }
 else
 {
 Res.negative = true;
 }
21
 switch (More(*this, B, 0))
 {
 case 0:
 MakeDel(*this, B, Res, Ost);
 break;
 case 1:
 Ost = *this;
 break;
 case 2:
 Res.data[1] = 1;
 Res.size = 1;
 break;
 }
 return Ost;
 }
 BigInt operator/(BigInt B)
 {
 if (B.data[1] == 0)
 {
 return 0;
 }
 BigInt Res, Ost;
 if ((negative && B.negative) || !(negative && B.negative))
 {
 Res.negative = false;
 }
 else
 {
 Res.negative = true;
 }
 switch (More(*this, B, 0))
 {
 case 0:
 MakeDel(*this, B, Res, Ost);
 break;
 case 1:
 Ost = *this;
 break;
 case 2:
 Res.data[1] = 1;
22
 Res.size = 1;
 break;
 }
 return Res;
 }
 BigInt operator/(int b) {
 if (b == 0) {
 return BigInt(0);
 }
 int carry = 0;
 BigInt result;
 result.size = size; 
 for (int i = result.size; i > 0; i--) {
 long long cur = data[i] + carry * Osn;
 result.data[i] = static_cast<int>(cur / b);
 carry = static_cast<int>(cur % b);
 }
 if (carry >= b) {
 result.data[result.size] += 1;
 }
 while (result.size > 1 && result.data[result.size - 1] == 0) {
 result.size--;
 }
 return result;
 }
 ~BigInt() {
 if (data != nullptr) {
 delete[] data;
 data = nullptr; 
 }
 }
}