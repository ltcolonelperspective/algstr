#include <iostream>
#include <string> 
#include <conio.h>

using namespace std;

class stack;
class d_array;
void sort(string infix, string& postfix, int& err);
char prior(char s);
bool prioroper(char s1, char s2);
bool opercheck(char oper);
bool funcheck(char fun);
bool numcheck(char num);

int main()
{
	while (true)
	{
		string infix, postfix;
		int key = 0, err = 0;
		cout << "input:\n";
		getline(cin, infix);
		sort(infix, postfix, err);
		system("cls");
		cout << "infix: " << infix << "\npostfix: ";
		err == 0 ? cout << postfix << "\n" : cout << "wrong expression, try again\n";
		cout << "esc to escape, enter to continue\n";
		while (key != 13 && key != 27)
		{
			key = _getch();
		}
		if (key == 27)
		{
			break;
		}
		system("cls");
	}

}

// реализация стека: конструктор, деструктор, проверка на пустоту, добавление в конец, удаление из конца, передача значения последнего элемента
class stack
{
private:
	struct node
	{
		char data;
		node* pnext;
	};
public:
	node* head;
	int size = 0;
	stack()
	{
		head = nullptr;
	}
	bool is_empty()
	{
		return head == nullptr;
	}
	void Push(char item)
	{
		node* temp = new node;
		temp->data = item;
		temp->pnext = head;
		head = temp;
		size++;
	}
	void pop()
	{
		if (!is_empty())
		{
			node* temp = head;
			head = head->pnext;
			delete temp;
			size--;
		}
	}
	char GetTop(int index_top = 0)
	{
		if (!is_empty() && (index_top < size))
		{
			node* temp = head;
			for (int i = 0; i < index_top; i++)
			{
				temp = temp->pnext;
			}
			return temp->data;
		}
		else
		{
			return 4;
		}
	}

	~stack()
	{
		while (!is_empty())
		{
			pop();
		}
	}
};

// реализация динамического массива: конструктор, деструктор, изменение размера
class d_array
{
public:
	int* arr = nullptr;
	int size = 0;
	d_array(int size_arr = 0)
	{
		if (size_arr > 0)
		{
			arr = new int[size_arr];
			size = size_arr;
		}
	}
	void Resize(int size_arr)
	{
		if (size_arr > 0)
		{
			int* temp = new int[size_arr];
			for (int i = 0; i < size_arr; i++)
			{

				temp[i] = (i < size) ? arr[i] : 0;
			}
			delete[] arr;
			arr = temp;
			size = size_arr;
		}
	}
	~d_array()
	{
		delete[] arr;
	}
};

bool opercheck(char oper) // проверяет, арифметическая операция ли это (да или нет)
{
	return (oper == '+' || oper == '-' || oper == '*' || oper == '/' || oper == '^');
}

bool funcheck(char fun) // проверяет, функция ли это (синус/косинус) (да или нет)
{
	return (fun == 's' || fun == 'c');
}

bool numcheck(char num) // проверяет, является ли символ цифрой (да или нет)
{
	return (num >= '0' && num <= '9');
}

char prior(char s) // возвращает приоритет арифметической операции
{
	switch (s)
	{
	case '^':
		return 3;
		break;
	case '*':
	case '/':
		return 2;
		break;
	case '+':
	case '-':
		return 1;
		break;
	default:
		return s;
		break;
	}
}

bool prioroper(char s1, char s2) // определяет приоритетность s2 над s1 (да или нет)
{
	return prior(s1) <= prior(s2);
}

void sort(string infix, string& postfix, int& err) //непосредственно обработка инфиксного в постфиксный
{
	int i = 0, inf_size = infix.length();
	char a = 0;

	stack stack;

	while (i < inf_size && !err)
	{
		a = infix[i];
		i++;

		if (numcheck(a)) // если число - сразу пишем в постфиксный
		{
			postfix += a;
		}
		else if (funcheck(a)) // если синус\косинус - заносим в стек
		{
			stack.Push(a);
			i += 2;
		}
		else if (opercheck(a)) // если операция, то удаляем другие операции в стеке до тех пор, пока не встретим более приоритетный 
		{		       // (или стек не закончится), затем записываем её в стек и возвращаем ранее удалённые элементы
			while (prioroper(a, stack.GetTop()) && opercheck(stack.GetTop()) && !stack.is_empty())
			{
				postfix += " ";
				postfix += stack.GetTop();
				stack.pop();
			}
			postfix += " ";

			stack.Push(a);
		}
		else if (a == '(') // вносим скобку
		{
			stack.Push(a);
		}
		else if (a == ')') // если скобка оказалась лишней - засчитываем ошибку ввода
		{
			while (stack.GetTop() != '(')
			{
				postfix += " ";
				postfix += stack.GetTop();

				stack.pop();
				if (stack.is_empty())
				{
					err = 1;
					break;
				}
			}

			stack.pop();

			if (funcheck(stack.GetTop()))
			{
				postfix += " ";
				postfix += stack.GetTop();
				if (stack.GetTop() == 's')
				{
					postfix += "in";
				}
				else if (stack.GetTop() == 'c')
				{
					postfix += "os";
				}
				stack.pop();
			}
		}
		else if (a != ' ')
		{
			err = 1;
		}
	}
	while (!stack.is_empty())
	{
		if (stack.GetTop() == '(')
		{
			err = 1;
			break;
		}
		postfix += " ";
		postfix += stack.GetTop();
		stack.pop();
	}
}
