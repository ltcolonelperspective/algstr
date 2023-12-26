#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;

struct Node;
class stack;
int min(int n1, int n2);
int max(int n1, int n2);
int get_minrun(int arrl);
int gallop(int* arr, int& gallop_p, int gallop_value, int gallop_end);
void merge_sort(int* arr, Node f_run, Node s_run);
void insertion_sort(int* arr, int arrl, int arrs_i);
void runsdev(int* arr, stack& runs, int arrl);
void merge(int* arr, stack& runs);

void timsort(int* arr, int arrl);


int main()
{
	const int n = 10000;
	int arr[n], value;
	for (int i = 0; i < n; i++)
	{
		value = rand() % 1000;
		arr[i] = value;
	}
	cout << "runtime:\n";
	auto start = chrono::system_clock::now();
	timsort(arr, sizeof(arr) / sizeof(int));
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << fixed << elapsed_seconds.count() << "s\n";
	system("pause");
	return 0;
}

struct Node
{
	int length = 0;
	int start_index = 0;
	Node* pnext;
};

int min(int n1, int n2) // определение меньшего из элементов
{
	return (n1 <= n2) ? n1 : n2;
}

int max(int n1, int n2) // определение большего из элементов
{
	return (n1 >= n2) ? n1 : n2;
}

class stack  // реализация стека: конструктор, деструктор, проверка на пустоту, добавление и удаление крайнего элемента, передача значения крайнего элемента
{
public:
	Node null_node;
	Node* head;
	int size = 0;
	stack()
	{
		head = nullptr;
	}
	bool is_empty()
	{
		return head == nullptr;
	}
	void Push(int run_start_i, int run_length)
	{
		Node* temp = new Node;
		temp->start_index = run_start_i;
		temp->length = run_length;
		temp->pnext = head;
		head = temp;
		size++;
	}
	void Pop()
	{
		if (!is_empty())
		{
			Node* temp = head;
			head = head->pnext;
			size--;
			delete temp;
		}
	}
	Node Peek(int index = 0)
	{
		if (!is_empty() && index < size)
		{
			Node* temp = head;
			for (int i = 0; i < index && index < size; i++)
			{
				temp = temp->pnext;
			}

			return *temp;
		}
		else
		{
			return null_node;
		}
	}
	void Edit(int run_start_i, int run_length, int index = 0)
	{
		if (!is_empty())
		{
			Node* temp = head;
			temp->start_index = run_start_i;
			temp->length = run_length;
		}
	}
	~stack()
	{
		while (!is_empty())
		{
			Pop();
		}
	}
};

int get_minrun(int arrl) // определение минрана
{
	int other_bits_det = 0;
	while (arrl >= 64)
	{
		other_bits_det |= arrl & 1;
		arrl >>= 1;
	}
	return arrl + other_bits_det;
}

// модификация "галоп"
// создана для ускорения слияния подмассивов: если первые 7 эл-тов взяли из одного массива, то алгоритм
// начинает брать из него элементов больше элементов, с каждым шагом на 2^i, пока он не закончится или перестанет подходить
int gallop(int* arr, int& gallop_p, int gallop_value, int gallop_end)  
{											
	int gallop_start = gallop_p;		 
	int left = 0, right = 0, middle = 0, degree = 0; 
	while (true)									
	{
		gallop_p += min((int)pow(2, degree), (gallop_end - gallop_p)); 
		if (arr[gallop_p] > gallop_value || gallop_p >= gallop_end)  
		{
			left = gallop_start;
			if (gallop_p > gallop_end - 1)
			{
				right = gallop_end - 1;
			}
			else
			{
				right = gallop_p;
			}
			do
			{
				middle = (left + right) / 2;

				if (arr[middle] < gallop_value)
				{
					left = middle + 1;
				}
				else
				{
					right = middle - 1;
				}
			} while (left <= right);

			gallop_p = left;
			return left;
		}
		else
		{
			degree++;
		}
	}
}

void insertion_sort(int* arr, int arrl, int arrs_i) // сортировка вставками в подмассивах
{
	int left = 0, right = 0, mid = 0;
	for (int i = arrs_i; i - arrs_i < arrl - 1; i++)
	{
		if (arr[i] > arr[i + 1])
		{
			int insert_value = arr[i + 1];
			left = arrs_i;
			right = i;
			do
			{
				mid = (left + right) / 2;
				if (arr[mid] < insert_value)
				{
					left = mid + 1;
				}
				else
				{
					right = mid - 1;
				}
			} while (left <= right);

			for (int j = i; j >= left; j--)
			{
				arr[j + 1] = arr[j];
			}
			arr[left] = insert_value;
		}
	}
}

void merge_sort(int* arr, Node f_run, Node s_run) // сортировка слиянием с проверкой на галоп
{
	int runs_length = f_run.length + s_run.length, first_p = f_run.start_index, second_p = s_run.start_index;
	int first_run_end = f_run.length + f_run.start_index, second_run_end = s_run.length + s_run.start_index;
	int first_gallop_count = 0, second_gallop_count = 0, gallop_start = 0, gallop_end = 0;
	int* temp = new int[runs_length];
	for (int i = 0; i < runs_length; i++)
	{
		if (first_p >= first_run_end)
		{
			temp[i] = arr[second_p];
			second_p++;
		}
		else if (second_p >= second_run_end)
		{
			temp[i] = arr[first_p];
			first_p++;
		}
		else if (arr[first_p] < arr[second_p])
		{
			temp[i] = arr[first_p];
			first_p++;
			first_gallop_count++;
			second_gallop_count = 0;
		}
		else
		{
			temp[i] = arr[second_p];
			second_p++;
			second_gallop_count++;
			first_gallop_count = 0;
		}
		if (first_gallop_count > 7)
		{
			gallop_start = first_p;
			gallop_end = gallop(arr, first_p, arr[second_p], first_run_end);
		}
		else if (second_gallop_count > 7)
		{
			gallop_start = second_p;
			gallop_end = gallop(arr, second_p, arr[first_p], second_run_end);
		}
		if (first_gallop_count > 7 || second_gallop_count > 7)
		{
			for (int j = 0; j < gallop_end - gallop_start; j++)
			{
				temp[i + j + 1] = arr[j + gallop_start];
			}
			i += gallop_end - gallop_start;
			first_gallop_count = 0;
		}
	}
	int array_start_i = min(f_run.start_index, s_run.start_index);
	for (int i = 0; i < runs_length; i++)
	{
		arr[i + array_start_i] = temp[i];
	}
}

void merge(int* arr, stack& runs) // слияние подмассивов
{
	Node x, y, z;
	x = runs.Peek(2), y = runs.Peek(1), z = runs.Peek(0);
	bool is_x_greater = x.length > y.length + z.length || runs.size < 3, is_y_greater = y.length > z.length || runs.size < 2;
	while (!is_x_greater || !is_y_greater)
	{
		runs.Pop();
		if (x.length)
		{
			runs.Pop();
			if (z.length > x.length)
			{
				merge_sort(arr, y, x);
				runs.Edit(min(y.start_index, x.start_index), y.length + x.length);
				runs.Push(z.start_index, z.length);
			}
			else
			{
				merge_sort(arr, y, z);
				runs.Push(min(y.start_index, z.start_index), y.length + z.length);
			}
		}
		else
		{
			merge_sort(arr, y, z);
			runs.Edit(min(y.start_index, z.start_index), y.length + z.length);
		}

		x = runs.Peek(2), y = runs.Peek(1), z = runs.Peek(0);
		is_x_greater = x.length > y.length + z.length || runs.size < 3, is_y_greater = y.length > z.length || runs.size < 2;
	}
	while (!runs.is_empty())
	{
		Node first_run = runs.Peek(1), second_run = runs.Peek(0);
		merge_sort(arr, first_run, second_run);
		runs.Pop();
		runs.Edit(min(first_run.start_index, second_run.start_index), first_run.length + second_run.length);
	}
}

void runsdev(int* arr, stack& runs, int arrl) // определение run'ов (подмассивов)
{
	int i = 0, min_run_length = get_minrun(arrl);
	while (i < arrl)
	{
		int run_start_index = i, run_length = min_run_length;
		if (i + min_run_length < arrl)
		{
			insertion_sort(arr, i, min_run_length);
			i += min_run_length;
			while (arr[i - 1] <= arr[i])
			{
				run_length++;
				i++;
			}
		}
		else
		{
			insertion_sort(arr, i, arrl - i);
			run_length = arrl - i;
			i = arrl;
		}
		runs.Push(run_start_index, run_length);
	}
}

void timsort(int* arr, int arrl) // непосредственно тимсорт
{
	stack runs;
	runsdev(arr, runs, arrl);
	merge(arr, runs);
}
