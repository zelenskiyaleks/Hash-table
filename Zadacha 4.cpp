#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int Hash_f(const string& str, int m) { //функция хэширования
	int hash = 0;
	for (int i = 0; i < str.size(); i++) {
		hash = (hash * 7 + str[i]) % m;
	}
	return hash;
}

int Hash_p(int hash_f, int i, int m) {  //функция перехэширования
	return (hash_f + i * (i + 1) / 2) % m;
}

struct HashTableNode {
	string key;
	bool Del;
	HashTableNode(string key_) : key(move(key_)), Del(false) {}
};

class HashTable {
public:
	explicit HashTable(size_t initial_size);
	~HashTable();
	bool Has(const std::string& key) const;
	bool Add(const std::string& key);
	bool Remove(const std::string& key);

private:
	vector<HashTableNode*> table;
	unsigned count;
	void ReHash(int new_size);
	bool Add_add(const std::string& key);
};

HashTable::HashTable(size_t initial_size) : table(initial_size, nullptr), count(0) {}

HashTable::~HashTable() {
	for (HashTableNode* head : table) {
		delete head;
	}
}

bool HashTable::Has(const std::string& key) const {
	assert(!key.empty());
	int hash_f = Hash_f(key, table.size());
	int hash_p = Hash_p(hash_f, 0, table.size());
	for (int i = 0; (i < table.size()) and (table[hash_p] != nullptr); i++) {
		if (table[hash_p]->key == key) {
			return true;
		}
		hash_p = Hash_p(hash_f, i + 1, table.size());
	}
	return false;
}

bool HashTable::Add(const string& key) {
	assert(!key.empty());
	double m = double(count) / table.size();
	if (m >= 0.75) {
		ReHash(2 * table.size());
	}

	bool z = Add_add(key);
	return z;
}

bool HashTable::Add_add(const string& key) {
	int hash_f = Hash_f(key, table.size());
	int hash_p = Hash_p(hash_f, 0, table.size());
	for (int i = 0; ((i < table.size()) and (table[hash_p] != nullptr)); i++) {
		if (table[hash_p]->key == key) {
			return false;
		}
		if (table[hash_p]->Del == true) {
			table[hash_p]->key = key;
			table[hash_p]->Del = false;
			count++;
			hash_p = Hash_p(hash_f, i + 1, table.size());
			for (int j = i + 1; ((j < table.size()) and (table[hash_p] != nullptr)); j++) {
				hash_p = Hash_p(hash_f, j, table.size());
				if (table[hash_p]->key == key) {
					table[hash_p]->Del = true;
					count--;
					table[hash_p]->key = "";
					return false;
				}
			}
			return true;
		}
		hash_p = Hash_p(hash_f, i + 1, table.size());
	}
	table[hash_p] = new HashTableNode(key);
	count++;
	return true;
}

bool HashTable::Remove(const std::string& key) {
	assert(!key.empty());
	int hash_f = Hash_f(key, table.size());
	int hash_p = Hash_p(hash_f, 0, table.size());
	for (int i = 0; ((i < table.size()) and (table[hash_p] != nullptr)); i++) {
		if (table[hash_p]->key == key) {
			table[hash_p]->Del = true;
			count--;
			table[hash_p]->key = "";
			return true;
		}
		else { hash_p = Hash_p(hash_f, i + 1, table.size()); }
	}
	return false;
}

void HashTable::ReHash(int new_size) {
	vector<HashTableNode*> new_table(new_size, nullptr);
	for (int i = 0; i < table.size(); i++) {
		if ((table[i] != nullptr) and (table[i]->Del == false)) {
			int j = 0;
			int hash_f = Hash_f(table[i]->key, new_size);
			int hash_p = Hash_p(hash_f, j, new_size);
			while (j < new_size) {
				if (new_table[hash_p] == nullptr) {
					new_table[hash_p] = table[i];
					break;
				}
				j++;
				hash_p = Hash_p(hash_f, j, new_size);
			}
		}
	}
	table = new_table;
}

int main() {
	HashTable table(8);
	char command = ' ';
	string value;

	while (cin >> command >> value) {
		switch (command) {
		case '?':
			cout << (table.Has(value) ? "OK" : "FAIL") << endl;
			break;
		case '+':
			cout << (table.Add(value) ? "OK" : "FAIL") << endl;
			break;
		case '-':
			cout << (table.Remove(value) ? "OK" : "FAIL") << endl;
			break;
		}
	}
	return 0;
}