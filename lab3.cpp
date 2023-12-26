#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;


struct node //структура для представления узлов дерева
{
	int value;
	node* left;
	node* right;

	node(int v)
	{
		value = v;
		left = right = nullptr;
	}
};

class BinaryTree // Реализация двоичного дерева
{
private:
	int* nodes;
	int i = 0;

	void orderEvent(int value) // Посещение узла
	{
		nodes[i] = value;
		++i;
	}

	void preOrder(node* a) // Прямой порядок обхода
	{
		if (a)
		{
			orderEvent(a->value);
			preOrder(a->left);
			preOrder(a->right);
		}
	}

	void inOrder(node* a) // Симметричный обход
	{
		if (a)
		{
			inOrder(a->left);
			orderEvent(a->value);
			inOrder(a->right);
		}
	}

	void postOrder(node* a) // Обратный порядок обхода
	{
		if (a)
		{
			postOrder(a->left);
			postOrder(a->right);
			orderEvent(a->value);
		}
	}

	bool isNumber(char a) // Проверка на число (да/нет)
	{
		return a >= '0' && a <= '9';
	}

	node* parsing(node* a, string s, int& i)  // Парсинг скобочной записи дерева
	{
		if (s[i] == '(')
		{
			string valueStr;
			i++;
			while (isNumber(s[i]))
			{
				valueStr += s[i]; 
				i++;
			}

			if (valueStr.size() != 0)  // Запись значения узла
			{
				int valueTmp = stoi(valueStr);
				a->left = new node(valueTmp);
				++counter;
			}
			else
			{
				return nullptr;
			}

			if (s[i] == '(') // Уход влево по левой скобочке
			{
				parsing(a->left, s, i);
			}

			if (s[i] != ')')
			{
				return nullptr;
			}
			i++;
		}
		if (s[i] == '(')
		{
			string valueStr;
			i++;
			while (isNumber(s[i])) // Аналогичное развитие событий для правого поддерева
			{
				valueStr += s[i];
				i++;
			}

			if (valueStr.size() != 0)
			{
				int valueTmp = stoi(valueStr);
				a->right = new node(valueTmp);
				++counter;
			}
			else
			{
				return nullptr;
			}

			if (s[i] == '(')
			{
				parsing(a->right, s, i);
			}

			if (s[i] != ')')
			{
				return nullptr;
			}
			i++;
		}
		return a;
	}

	node* findMin(node* a) // Поиск минимального узла в дереве a
	{
		return a->left ? findMin(a->left) : a;
	}

	node* removeMin(node* a) // его удаление из дерева
	{
		if (a->left == 0)
		{
			return a->right;
		}
		a->left = removeMin(a->left);

		return a;
	}
	node* insert(node* a, int value) // Добавить узел
	{
		if (!a)
		{
			return new node(value);
		}

		if (value < a->value)
		{
			a->left = insert(a->left, value);
		}
		else
		{
			a->right = insert(a->right, value);
		}
		return a;
	}
	node *remove(node *a, int value) // Удаление ключа value из дерева a
	{
		if (!a)
		{
			return 0;
		}

		if (value < a->value)
		{
			a->left = remove(a->left, value);
		}
		else
		{
			if (value > a->value)
			{
				a->right = remove(a->right, value);
			}
			else
			{
				node* b = a->left;
				node* c = a->right;
				delete a;
				if (!c)
				{
					return b;
				}
				node* min = findMin(c);
				min->right = removeMin(c);
				min->left = b;

				return min;
			}
		}

		return a;
	}

public:
	node* root;
	int counter = 0; 

	BinaryTree() // конструктор 
	{
		root = nullptr;
		nodes = nullptr;
		counter = 0;
	}

	void add(int value) // при добавлении (+1 к счётчику узлов)
	{
		root = insert(root, value);
		++counter;
	}

	void del(int value) // при удалении (-1 к счётчику узлов)
	{
		root = remove(root, value);
		counter--;
	}

	void strToTree(string s) // проверка корректности ввода, если ошибок нет - добавляет считанное число
	{
		bool error = 0;
		int brackets = 0;
		s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] == '(')
			{
				brackets++;
			}
			else if (s[i] == ')')
			{
				brackets--;
			}
			else if (!isNumber(s[i]))
			{
				error = 1;
				break;
			}
		}

		if (s.size() != 0 && !error && !brackets)
		{
			int i = 0;
			string tmp;
			while (isNumber(s[i]))
			{
				tmp += s[i];
				i++;
			}
			add(stoi(tmp));
			root = parsing(root, s, i);
		}
	}

	int* preOrder() // root для прямого обхода
	{
		if (root)
		{
			delete[]nodes;
			nodes = new int[counter];
			preOrder(root);
			i = 0;
			return nodes;
		}
	}

	int* inOrder() // root для симметричного обхода
	{
		if (root)
		{
			delete[]nodes;
			nodes = new int[counter];
			inOrder(root);
			i = 0;
			return nodes;
		}
	}

	int* postOrder() // root для обратного обхода
	{
		if (root)
		{
			delete[]nodes;
			nodes = new int[counter];
			postOrder(root);
			i = 0;
			return nodes;
		}
	}
};

struct avlNode // структура для представления узлов АВЛ-дерева
{
	int value;
	int height;
	avlNode* left;
	avlNode* right;

	avlNode(int v)
	{
		value = v;
		left = right = nullptr;
		height = 1;
	}
};

class AVL // Реализация АВЛ-дерева
{
private:
	int* nodes = 0;
	int i;

	void orderEvent(int value)
	{
		nodes[i] = value;
		++i;
	}

	void preOrder(avlNode* a) // Прямой обход
	{
		if (a)
		{
			orderEvent(a->value);
			preOrder(a->left);
			preOrder(a->right);
		}
	}
	 
	void inOrder(avlNode* a) // Симметричный обход
	{
		if (a)
		{
			inOrder(a->left);
			orderEvent(a->value);
			inOrder(a->right);
		}
	}

	void postOrder(avlNode* a) // Обратный обход
	{
		if (a)
		{
			postOrder(a->left);
			postOrder(a->right);
			orderEvent(a->value);
		}
	}

	int getHeight(avlNode* a) // Возвращает высоту дерева a
	{
		return a ? a->height : 0;
	}

	int getBalanceFactor(avlNode* a) // Возвращает разницу высот правого и левого поддеревьев а
	{
		return getHeight(a->right) - getHeight(a->left);
	}

	void editHeight(avlNode* a) // Восстанавливает корректное значение высоты заданного узла
	{
		int heightLeft = getHeight(a->left);
		int heightRight = getHeight(a->right);
		a->height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
	}

	avlNode *rightRotate(avlNode* a) // правый поворот вокруг a
	{
		avlNode* q = a->left;
		a->left = q->right;
		q->right = a;

		editHeight(a);
		editHeight(q);

		return q;
	}

	avlNode *leftRotate(avlNode* a) // левый поворот вокруг a
	{
		avlNode* b = a->right;
		a->right = b->left;
		b->left = a;

		editHeight(a);
		editHeight(b);

		return b;
	}

	avlNode *balance(avlNode* a) // Балансировка: если разница стала критической (2 или -2) - выполняется соответствующий поворот
	{
		editHeight(a);

		if (getBalanceFactor(a) == 2)
		{
			if (getBalanceFactor(a->right) < 0)
			{
				a->right = rightRotate(a->right);
			}
			return leftRotate(a);
		}

		if (getBalanceFactor(a) == -2)
		{
			if (getBalanceFactor(a->left) > 0)
			{
				a->left = leftRotate(a->left);
			}
			return rightRotate(a);
		}

		return a;
	}

	avlNode *findMin(avlNode* a) // поиск мин. узла в дереве a
	{
		return a->left ? findMin(a->left) : a;
	}

	avlNode *removeMin(avlNode* a) // его удаление из дерева a
	{
		if (a->left == 0)
		{
			return a->right;
		}
		a->left = removeMin(a->left);

		return balance(a);
	}

	avlNode *insert(avlNode* a, int value) // вставка ключа value в дерево с корнем a
	{
		if (!a)
		{
			return new avlNode(value);
		}

		if (value < a->value)
		{
			a->left = insert(a->left, value);
		}
		else
		{
			a->right = insert(a->right, value);
		}

		return balance(a);
	}

	avlNode *remove(avlNode* a, int value) // удаляет ключ value из дерева a
	{
		if (!a)
		{
			return 0;
		}

		if (value < a->value)
		{
			a->left = remove(a->left, value);
		}
		else
		{
			if (value > a->value)
			{
				a->right = remove(a->right, value);
			}
			else
			{
				avlNode* b = a->left;
				avlNode* c = a->right;
				delete a;
				if (!c)
				{
					return b;
				}
				avlNode* min = findMin(c);
				min->right = removeMin(c);
				min->left = b;

				return balance(min);
			}
		}

		return balance(a);
	}

public:
	avlNode* root;
	int counter;

	AVL() // конструктор
	{
		root = nullptr;
		nodes = nullptr;
		counter = 0;
		i = 0;
	}

	void add(int value) // счетчик узлов +1
	{
		root = insert(root, value);
		++counter;
	}

	void del(int value) // счетчик узлов -1
	{
		root = remove(root, value);
		counter--;
	}

	int* preOrder() // root для прямого обхода
	{
		if (root)
		{
			delete[]nodes;
			nodes = new int[counter];
			preOrder(root);
			i = 0;
			return nodes;
		}
	}

	int* inOrder() // root для симметричного обхода
	{
		if (root)
		{
			delete[]nodes;
			nodes = new int[counter];
			inOrder(root);
			i = 0;
			return nodes;
		}
	}

	int* postOrder() // root для обратного обхода
	{
		if (root)
		{
			delete[]nodes;
			nodes = new int[counter];
			postOrder(root);
			i = 0;
			return nodes;
		}
	}
};

void prTree(node* a, int lvl) // Вывод двоичного дерева в консоль
{
	if (a)
	{
		prTree(a->right, lvl + 1);
		for (int i = 0; i < lvl; i++)
		{
			cout << "   ";
		}
		cout << a->value << "-<\n";
		prTree(a->left, lvl + 1);
	}
}

void prAVL(avlNode* a, int lvl) // Вывод АВЛ-дерева в консоль
{
	if (a)
	{
		prAVL(a->right, lvl + 1);
		for (int i = 0; i < lvl; i++)
		{
			cout << "   ";
		}
		cout << a->value << "-<\n";
		prAVL(a->left, lvl + 1);
	}
}

int main(int argc, char* argv[]) {
	BinaryTree tree;
	string s;
	s = "8 (4(1)(4(2)(7))) (9(11(9)))";
	tree.strToTree(s);
	AVL avlTree;
	int* treeNodes = tree.preOrder();

	for (int i = 0; i < tree.counter; ++i)
	{
		avlTree.add(treeNodes[i]);
	}
	cout << "Tree:\n";
	prTree(tree.root, 1);
	cout << "\n\nAVL:\n";
	prAVL(avlTree.root, 1);
	cout << "\n\n";
	int *avlTreeNodes = avlTree.preOrder();
	cout << "Preorder - ";
	for (int i = 0; i < avlTree.counter; i++)
	{
		cout << avlTreeNodes[i] << " ";
	}
	avlTreeNodes = avlTree.inOrder();
	cout << "\nInorder - ";
	for (int i = 0; i < avlTree.counter; i++)
	{
		cout << avlTreeNodes[i] << " ";
	}

	avlTreeNodes = avlTree.postOrder();
	cout << "\nPostorder - ";
	for (int i = 0; i < avlTree.counter; i++)
	{
		cout << avlTreeNodes[i] << " ";
	}

	cout << "\n";
	system("pause");
}
