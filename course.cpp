#include <iostream>
#include <string>
#include <fstream>
#pragma once

using namespace std;

template <typename T>
class D_array 
{
	private:
		size_t size_, capacity_;
		T* arr_;
	public:
		D_array() {
			size_ = 0;
			capacity_ = 1;
			arr_ = new T[capacity_];
		};
		~D_array() {
		};
		T& operator[](size_t index) {
			if (index < size_) {
				return arr_[index];
			}
			else {
				cout << "Invalid index";
			}
		};
		size_t arr_length() {
			return size_;
		};
		void xtwo()
		{
			capacity_ *= 2;
			T* tmp = new T[capacity_];
			for (int i = 0; i < size_; i++)
			{
				if (i < size_)
				{
					tmp[i] = arr_[i];
				}
			}
			delete[] arr_;
			arr_ = tmp;
		};
		void ins(size_t index, T new_el) {
			if (index > size_) {
				cout << "Invalid index";
			}
			if (size_ == capacity_) {
				xtwo();
			}
			for (size_t i = size_; i != index; i--) {
				arr_[i] = arr_[i - 1];
			}
			arr_[index] = new_el;
			size_++;
		};
		void push_back(T new_el) {
			this->ins(size_, new_el);
		};
		void el_swap(int el1, int el2) {
			if (el1 > size_ || el2 > size_) {
				cout << "Invalid index";
			}
			T temp = arr_[el1];
			arr_[el1] = arr_[el2];
			arr_[el2] = temp;
		};
		void pr() {
			for (size_t i = 0; i < size_; i++) {
				cout << arr_[i] << ' ';
			}
			cout << endl;
		};
};
template <typename Keytype, typename Datatype>
class Arr2 {
private:
	struct node {
		Keytype key;
		Datatype data;
	};
	D_array<node> arr_;
	size_t size_;
public:
	Arr2() {
		size_ = 0;
	}
	Datatype& operator[](Keytype key) {
		for (size_t i = 0; i < size_; i++) {
			if (arr_[i].key == key) {
				return arr_[i].data;
			}
		}
		cout << "Invalid key";
	}
	void ins(Keytype key, Datatype data) {
		node temp;
		temp.key = key;
		temp.data = data;
		arr_.push_back(temp);
		size_++;
	}
	void del_key(Keytype key) {
		for (size_t i = 0; i < size_; i++) {
			if (arr_[i].key = key) {
				arr_.remove(i);
				size_--;
				return;
			}
		}
		cout << "Invalid key";
	}
	void del_data(Datatype data) {
		for (size_t i = 0; i < size_; i++) {
			if (arr_[i].data = data) {
				arr_.remove(i);
				size_--;
				return;
			}
		}

		cout << "Invalid data";
	}
};

template <typename T>
class Ufds {
private:
	Arr2<T, T> arr_;
public:
	Ufds(D_array<T> arr) {
		for (size_t i = 0; i < arr.arr_length(); i++) {
			arr_.ins(arr[i], arr[i]);
		}
	}
	void MakeSet(D_array<T> arr) {
		for (size_t i = 0; i < arr.arr_length(); i++) {
			arr_.ins(arr[i], arr[i]);
		}
	}
	T Find(T node) {
		if (arr_[node] == node) {
			return node;
		}
		else {
			return Find(arr_[node]);
		}
	}
	void Unite(T a, T b) {
		T x = this->Find(a);
		T y = this->Find(b);
		arr_[x] = y;
	}
};

class Algorithm {
	private:
		struct edge {
			string v1;
			string v2;
			int weight;
		};
		D_array<string> v;
		D_array<edge> edges;
		size_t num_of_v;
		void sort() {
			for (size_t i = 0; i < edges.arr_length(); i++) {
				size_t j = i;
				while (j > 0 && edges[j - 1].weight > edges[j].weight) {
					edges.el_swap(j - 1, j);
					j--;
				}
			}
		}	
	public:
		Algorithm(string file_name) {
			ifstream file;
			file.open(file_name);
			string str_v;
			getline(file, str_v, '\n');
			for (size_t i = 0; i < str_v.length(); i++) {
				string temp;
				while (str_v[i] != ' ' && i < str_v.length()) {
					temp += str_v[i];
					i++;
				}
				if (temp != "") {
					v.push_back(temp);
				}
			}
			num_of_v = v.arr_length();
			for (size_t i = 0; i < num_of_v; i++) {
				for (size_t j = 0; j < num_of_v; j++) {
					int x;
					file >> x;
					if (x > 0) {
						edge temp_edge;
						temp_edge.v1 = v[i];
						temp_edge.v2 = v[j];
						temp_edge.weight = x;
						edges.push_back(temp_edge);
					}
				}
			}	
			file.close();
		}
		void Kruskal() {
			D_array<edge> result;
			Ufds<string> ds(v);
			this->sort();
			for (size_t i = 0; i < edges.arr_length(); i++) {
				if (ds.Find(edges[i].v1) != ds.Find(edges[i].v2)) {
					ds.Unite(edges[i].v1, edges[i].v2);
					result.push_back(edges[i]);
				}
			}
			int sum = 0;
			for (size_t i = 0; i < result.arr_length(); i++) {
				cout << result[i].v1 << " " << result[i].v2 << endl;
				sum += result[i].weight;
			}
			cout << sum << endl;
		}
};

int main()
{
	Algorithm graph("graph.txt");
	graph.Kruskal();
	return 0;
}
