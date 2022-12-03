#include<iostream>
#include"mini-gmp.h"
#include"mini-gmpxx.h"
#include"random.h"
#include<vector>
#include<bitset>
#include<chrono>
#include<stack>
#include<fstream>
#include"Windows.h"

using namespace std;
using namespace chrono;

static Random r;

mpz_class FastDegree(mpz_class a, mpz_class w, mpz_class n) {
	mpz_class S = 1, V = w, c = a;
	while (V != 0) {
		if (V % 2 == 1)
			S = (S * c) % n;
		V /= 2;
		c = (c * c) % n;
	}
	return a == 0 ? 0 : S;	
}

mpz_class GCD(mpz_class a, mpz_class b) {
	while (b != 0) {
		a %= b;
		swap(a, b);
	}

	return a;
}

mpz_class AdvancedGCD(mpz_class a, mpz_class b, mpz_class* x, mpz_class* y) {
	if (a < b) {
		swap(a, b);
		swap(x, y);
	}

	mpz_class r0 = a, r1 = b, r2 = 0;
	mpz_class s0 = 1, s1 = 0, s2 = 0;
	mpz_class t0 = 0, t1 = 1, t2 = 0;
	
	do {
		r2 = r0 - (r0 / r1) * r1;
		s2 = s0 - (r0 / r1) * s1;
		t2 = t0 - (r0 / r1) * t1;

		r0 = r1; r1 = r2;
		s0 = s1; s1 = s2;
		t0 = t1; t1 = t2;
	} while (r2 != 0);

	if(x != nullptr)
		*x = s0;
	if(y != nullptr)
		*y = t0;

	return r0;
}

bool IsCoPrime(mpz_class a, mpz_class b) {
	return GCD(a, b) == 1;
}

mpz_class ChoosePublicExp(mpz_class f) {
	uint32_t cool[] = {17, 257, 65537};
	for (auto i : cool)
		if (i < f && IsCoPrime(i, f))
			return i;
	
	for (mpz_class i = 2; i < f; i++)
		if (IsCoPrime(i, f))
			return i;

	throw;
}

mpz_class ChoosePrivateExp(mpz_class f, mpz_class e) {
	mpz_class x;
	AdvancedGCD(e, f, &x, nullptr);
	return x > 0 ? x : x + f;
}

mpz_class Random512bit() {
	mpz_class num = 0;

	for (int i = 0; i < 8; i++) {
		uint64_t temp = r.Next();
		uint32_t p1 = temp >> 32;
		uint32_t p2 = temp;
		num = (num << 32) | p1;
		num = (num << 32) | p2;
	}

	return num;
}
bool MillerRabin(mpz_class n, int rounds) {
	if (n % 2 == 0)
		return false;
	
	if (rounds > n - 2)
		rounds = n.get_ui() - 2;

	mpz_class s = 0, t = n - 1;
	do {
		t /= 2;
		s++;
	} while (t % 2 == 0);

	vector<mpz_class> nums;
	for (int k = 0; k < rounds; k++) {
		mpz_class temp = (Random512bit() % (n - 2)) + 2;
		if (find(nums.begin(), nums.end(), temp) != nums.end()) {
			k--;
			continue;
		}
		nums.push_back(temp);
	}
	
	for (const mpz_class& a : nums) {
		mpz_class x = FastDegree(a, t, n);
		if (x == 1 || x == n - 1)
			goto cntn;

		for (int i = 0; i < s - 1; i++) {
			x = FastDegree(x, 2, n);
			if (x == 1)
				return false;
			if (x == n - 1)
				goto cntn;
		}
		return false;
	cntn:
		continue;
	}

	return true;
}


vector<mpz_class> Encrypt(const vector<uint8_t>& s, mpz_class e, mpz_class n) {
	const int charcount = 124;
	vector<mpz_class> v;
	size_t i = 0;
	for (; i + charcount-1 < s.size(); i += charcount) {
		mpz_class c = 0;
		for (int j = 0; j < charcount; j++)
			c = (c << 8) | s[i + j];
		v.push_back(FastDegree(c, e, n));
	}

	if (s.size() % charcount != 0) {
		int k = 0;
		mpz_class c = 0;
		for (; i < s.size(); i++, k++)
			c = (c << 8) | s[i];
		c <<= 8 * (charcount - k);
		v.push_back(FastDegree(c, e, n));
	}
	return v;
}

vector<uint8_t> Decrypt(const vector<mpz_class>& s, mpz_class d, mpz_class n) {
	const int charcount = 124;

	vector<uint8_t> v;
	for (auto c : s) {
		mpz_class a = FastDegree(c, d, n);
		for (int i = 0; i < charcount / 4; i++) {
			for (int j = 0; j < 4; j++)
				v.push_back((mpz_class(a >> 32 * (charcount / 4 - i - 1)).get_ui() >> (8 * (3 - j))) & 0xff);
		}
	}
	return v;
}

vector<uint8_t> ConvertToVectorOfUnsignedChars(const vector<mpz_class>& mpv) {
	vector<uint8_t> result;
	for (auto i : mpv) {
		mpz_class temp = i;
		stack<uint8_t> st;
		while (temp != 0) {
			st.push(temp.get_ui() & 0xff);
			temp >>= 8;
		}
		
		while (!st.empty()) {
			result.push_back(st.top());
			st.pop();
		}
	}
	return result;
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	size_t round_count = 20;

	mpz_class p = Random512bit(), q = Random512bit();
	
	p |= (mpz_class(1) << 511) | 1;
	q |= (mpz_class(1) << 511) | 1;
	p &= (mpz_class(1) << 512) - 1 - (mpz_class(1) << 64);
	q &= (mpz_class(1) << 512) - 1 - (mpz_class(1) << 64);


	while (!MillerRabin(p, round_count))
		p += 2;
	while (!MillerRabin(q, round_count))
		q += 2;


	mpz_class n = p * q, f = (p - 1) * (q - 1); 

	mpz_class e = ChoosePublicExp(f);
	mpz_class d = ChoosePrivateExp(f, e);


	string raw_text;
	cout << "Введите текст:" << endl;
	getline(cin, raw_text);

	vector<uint8_t> raw_text_vector;
	copy(raw_text.begin(), raw_text.end(),	back_inserter(raw_text_vector));
	vector<mpz_class> crypt_text_vector = Encrypt(raw_text_vector, e, n);
	vector<uint8_t> decrypt_text_vector = Decrypt(crypt_text_vector, d, n);

	cout << "Текст: " << endl;
		cout << "\t" << raw_text << endl;
	cout << endl << endl;
	

	vector<uint8_t> crypt_char_vector = ConvertToVectorOfUnsignedChars(crypt_text_vector);
	cout << "Кодированный текст: " << endl;
	for (auto i : crypt_text_vector)
		cout << i;
		
	cout << endl << endl;

	cout << "Расшифрованный текст: " << endl;
		cout << "\t";
		for (auto i : decrypt_text_vector)
			cout << i;
		cout << endl;
	
	cout << endl << endl;
	cout << "Открытый ключ: " << endl;
	cout << "\te: " << e.get_str() << endl;
	cout << "\tn: " << n.get_str() << endl;

	ofstream fout("secretkey.txt");

	fout << "Закрытый ключ: " << endl;
	fout << "\td: " << d.get_str() << endl;
	fout << "\tn: " << n.get_str() << endl;

	fout.close();
}