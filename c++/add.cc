// Sum up two unsigned binary numbers
// (C) 2013 Igor Pozgaj <ipozgaj@fb.com>

#include <iostream>
#include <string>

using namespace std;

const string digits = "0123456789ABCDEF";

string add(const string& num1, const string& num2, const int base)
{
  string result(max(num1.length(), num2.length()) + 1, '0');

  auto carry = 0;

  for (unsigned int offset = 1 ; offset <= result.length() ; offset++) {
    auto d1 = (offset > num1.length()) ? '0' : num1[num1.length() - offset];
    auto d2 = (offset > num2.length()) ? '0' : num2[num2.length() - offset];
    auto val1 = digits.find(d1);
    auto val2 = digits.find(d2);
    result[result.length() - offset] = digits[(val1 + val2 + carry) % base];
    carry = (carry + val1 + val2) / base;
  }

  return result.substr(result.find_first_not_of("0"));
}

int main(int argc, char* argv[])
{
  if (argc < 3) {
    cerr << argv[0] << " syntax: " << argv[0] << " num1 num2 [base]" << endl;
    return -1;
  }

  const string num1 = string(argv[1]);
  const string num2 = string(argv[2]);

  int base = 2;

  if (argc == 4) {
    base = stoi(argv[3]);
    if (base < 2 || base > 16) {
      cerr << "Base must be in range [2, 16]" << endl;
      return 1;
    }
  }

  if (num1.find_first_not_of(digits.substr(0, base)) != string::npos ||
      num2.find_first_not_of(digits.substr(0, base)) != string::npos) {
    cerr << "Arguments must consist only of digits in ["
         << digits.substr(0, base) << "]" << endl;
    return 1;
  }

  const string sum = add(num1, num2, base);
  cout << sum << endl;

  return 0;
}
