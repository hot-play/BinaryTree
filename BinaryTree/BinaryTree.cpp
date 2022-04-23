#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

class Node // Класс узел
{
private:
	int key;
	Node* left, *right;
public:
	friend class BinaryTree;
	Node()// конструктор по умолчанию
	{
		key = 0;
		left = NULL;
		right = NULL;
	}
	Node(int k)// конструктор по умолчанию c key (для корня дерева),key - ключ узла(ветки)
	{
		key = k;
		left = NULL;
		right = NULL;
	}
	Node(int k, Node* l, Node* r)// конструктор с аргументами, где key - ключ узла (ветки), l - указатель на левую ветку, r - на правую
	{
		key = k;
		left = l;
		right = r;
	}
	bool emptyNode(Node * temp)
	{
		if (key == 0 && left == NULL && right == NULL)
			return 1;
		return 0;
	}
	int Key() //возвращение ключа
	{
		return key;
	}
	friend ostream& operator << (ostream& o, Node* t)//перегрузка оператора вывод
	{
		if (t == NULL)
			cout << "Такого узла нет";
		else
			cout << t->key;//вывод значения ключа
		return o;
	}
};

class BinaryTree //Класс бинарное дерево
{
private:
	Node* root; //указатель на корень
public:
	BinaryTree()
	{
		Node* temp;
		root = temp;
	}
	BinaryTree(Node *temp)
	{
		if (temp)
		{
			root = temp;
		}
		else
			root = NULL;
	}
	BinaryTree(int* a, int n) //конструктор с двумя параметрами, где а-массив элементов, n-кол-во элементов
	{
		root = NULL;
		for (int i = 0; i < n; i++)
		{
			root = add_node(root, a[i]);
		}
	}
	BinaryTree(BinaryTree& temp)
	{
		copy(temp.root, this->root);
	}
	~BinaryTree()
	{
		clear(root);
	}
	int height() //ф-ция вычисления высоты дерева
	{
		return heightNode(root);
	}
	int heightNode(Node* temp) //ф-ция вычисления высоты узла
	{
		if (temp == NULL) return 0; //у пустых деревьев и узлов соответствующая высота
		int h_l = heightNode(temp->left); //в высоту левого поддерева заносим рекурсией значение высоты левого потомка
		int h_r = heightNode(temp->right); //в высоту правого поддерева заносим рекурсией значение высоты правого потомка
		if (h_l > h_r) return h_l + 1; //Если высота левого потомка больше правого, то высота корня данного поддерева соответственно на 1 больше
		return h_r + 1; //Иначе высота корня данного поддерева больше высоты правого поддерева на 1
	}
	void levels() //Передаём высоту дерева в качестве параметра в ф-цию обхода по уровням
	{
		int u = height();
		for (int i = 0; i < u; i++) //по циклу обходим дерево u раз
		{
			levels(root, i);//передаём какой уровень нужно вывести
			cout << endl;// отступаем
		}
	}
	void levels(Node* temp, int i) //ф-ция нахождения нужного уровня
	{
		if (temp == NULL) return; //Если узел пустой, возвращаемся
		else
		{
			if (i == 0) //если u=0, то мы дошли до нужного уровня, выводим элемент
			{
				cout << temp->key << "  ";
				return;
			}
			if (temp->left) levels(temp->left, i - 1); //рекурсией уходим влево и уменьшаем число уровней, которое осталось пройти до нужного
			if (temp->right) levels(temp->right, i - 1); //рекурсией уходим вправо и уменьшаем число уровней, которое осталось пройти до нужного
		}
	}
	BinaryTree& operator= (BinaryTree& temp)//оператор присвоения
	{
		if (this == &temp) return *this;
		else
		{
			clear(this->root);
			copy(temp.root, this->root);
			return *this;
		}
	}
	Node* findNode(int k)//ф-ция нахождения узла
	{
		Node* p = findNode(root, k);//возвращаем нужный узел, либо NULL
		return p;//возвращаем узел
	}
	Node* findNode(Node* temp, int k)//ф-ция нахождения узла
	{
		if (temp == NULL)
			return temp;//если узел пуст, возвращаемся 
		if (k == temp->key)
			return temp;//если искомое значение совпадает с ключём текущего узла, мы нашли его, возвращаем его
		if (k < temp->key)
			temp = findNode(temp->left, k);//Если искомое значение меньше ключа текущего узла, идём в левое поддерево
		else
			if (k > temp->key)
				temp = findNode(temp->right, k);//Иначе в правое поддерево
		return temp;//возвращаем результат поисков
	}
	bool delNode(int k)//ф-ция удаления узла
	{
		Node* temp = findNode(k), *r = root;//Ищем узел в дереве 
		if (temp == NULL) return 0;
		if (temp == root)
		{
			root = delNode(temp);
			return 1;
		}
		while ((r->left != temp) && (r->right != temp))//пока мы не найдём путь от корня до удаляемого узла:
		{
			if (r->key > temp->key)r = r->left;//Если значение текущего узла больше нужного узла, двигаемся влево
			else r = r->right; //Иначе вправо
		}
		if (r->left == temp) r->left = delNode(temp);//Если левый потомок текущего узла = нужному, в левого потомка возвращаем результат удаления узла q
		else r->right = delNode(temp);//иначе, в правого потомка
		return 1;
	}
	Node* delNode(Node* temp)//удаление узла из дерева
	{
		Node* s, *r = temp;//s будет потомком текущего узла, r-предком
		if (temp->left == NULL && temp->right == NULL)//Если у удаляемого узла нет потомков, просто опустошаем его
		{
			delete temp;
			temp = NULL;
			return temp;
		}
		if (temp->right == NULL)//если он не имеет правого потомка, то на его место становится его левый потомок
		{
			r = temp;
			temp = temp->left;
			delete r;
			return temp;
		}
		if (temp->left == NULL)//если он не имеет левого потомка, то на его место становится его правый потомок
		{
			r = temp;
			temp = temp->right;
			delete r;
			return temp;
		}
		//Если есть и левый и правый потомок, выбираем ближайший по значению узел из правого поддерева
		s = temp->right;
		if (s->left == NULL)//Если у правого потомка удаляемого узла нет левого потомка, значит правый потомок наименьший из этого поддерева
		{
			temp->key = s->key;//Не использую присвоение, т.к. сотрутся данные о левом поддереве удаляемого узла, а это недопустимо
			temp->right = s->right;
			delete s;//очищаю память s
			return temp;
		}
		while (s->left != NULL)//Если левый потомок правого поддерева удаляемого узла не пуст, идём налево доконца, т.к. последний будет наименьшим по значению
		{
			r = s; s = s->left;//r-предок s
		}
		temp->key = s->key;//переназначаем новый узел на место удалённого
		r->left = s->right;
		delete s;//очищаем память
		return temp;
	}
	Node* rootFind() //Возвращение корня
	{
		return root;
	}
	void printTree(int l, Node* temp)// Вывод дерева в консоль
	{
		if (temp == NULL) return;
		printTree(l + 3, temp->right);
		for (int k = 0; k < l; k++) cout << " ";
		cout.width(2);
		cout << temp->key << "\n";
		printTree(l + 3, temp->left);
	}
	int count()//Возвращает количество узлов
	{
		return countNode(root) - 1;
	}
	int countNode(Node* temp)//Основная функция возвращения узлов
	{
		if (temp == NULL) //Если ветка не идет дальше, преедаем 1
			return 1;
		int c_l = countNode(temp->left);//подсчет левой части
		int c_r = countNode(temp->right);//подсчет правой части
		return c_l + c_r;//Вывод количества справа и слева
	}
	int inVector(vector<int> v) //передаём пустой вектор размера n и возвращаем число узлов в дерева
	{
		int k = 0; //обнуляем счётчик
		inVector_auxiliary(root, v, k); //вызываем основную функцию заполнения упорядоченного вектора
		return k; //возвращаем число элементов в векторе
	}
	void inVector_auxiliary(Node* p, vector<int> v, int& k)// заполняем вектор
	{
		if (p == NULL)
			return;//если узел пуст, добавлять нечего, возвращаемся
		inVector_auxiliary(p->left, v, k);//рекурсивно уходим влево, пока не дойдём до конца
		v.at(k) = p->key;//заносим в вектор значение ключа
		k++; //увеличиваем счётчик элементов
		inVector_auxiliary(p->right, v, k);//рекурсивно уходим вправо и продолжаем
	}
	bool isEmpty()//Возвращает true, если дерево пустое
	{
		if (root->key == 0 && root->left == NULL && root->right == NULL)
			return 1;
		return 0;
	}
	//Вспомогательные функции для узлов и дерева (используются только внутри класса)
	BinaryTree create(int n)//ф-ция создания дерева
	{
		int k;//вводимые с клавиатуры значения ключей
		cout << "Введите элементы" << endl;
		for (int i = 0; i < n; i++)
		{
			cin >> k;
			root = add_node(root, k);//добавление узлов в дерево
		}
		return *this;//возвращение конечного дерева
	}
	BinaryTree add_node(int x)//добавления узла в дерево
	{
		root = add_node(root, x);//возвращает корень с изменённой информацией о потомках и т.д.
		return *this;//возвращает конечное дерево
	}
	Node* add_node(Node* temp, int x)//ф-ция добавления узла в дерево
	{
		if (temp == NULL) //Если узел пуст, то добавляем наш узел в это место
		{
			Node* temp = new Node(x);//создаём узел, используя конструктор по умолчанию(к-значение ключа нашего узла)
			return temp;//возвращаем узел
		}
		if (x <= temp->key)
			temp->left = add_node(temp->left, x);//Если значение добавляемого узла меньше либо равно текущему, рекурсивно уходим в левое поддерево
		else
			temp->right = add_node(temp->right, x);//Иначе если значение добавляемого узла больше текущего, рекурсивно уходим в правое поддерево
		return temp;//возвращаем узел
	}
	void copy(Node* temp, Node*& root)
	{
		if (temp != NULL)
		{
			root = new Node;
			root->key = temp->key;
			root->left = NULL;
			root->right = NULL;
			copy(temp->left, root->left);
			copy(temp->right, root->right);
		}
	}
	void clear(Node* temp)
	{
		if (temp == NULL) return;
		clear(temp->left);
		clear(temp->right);
		delete temp;
		temp = NULL;
	}
};
int main()
{
	setlocale(LC_ALL, "Russian");
	int n = 13;
	int mas[13] = { 5,3,8,4,7,45,6,23,8,9,5,2,20 };
	BinaryTree BT(mas, n);
	Node* root = BT.rootFind();
	BT.printTree(0, root);
	cout << endl;
	cout << "Дерево пустое: ";
	if (BT.isEmpty())
		cout << "true";
	else
		cout << "false";
	cout << endl << "Высота дерева: ";
	int x = BT.height();
	cout << x << endl;
	cout << "Удалим узел с ключом 3, удален: ";
	if (BT.delNode(3))
		cout << "true";
	else
		cout << "false";
	cout << endl;
	BT.printTree(0, root);
	cout << endl;
	cout << "Количество узлов: " << BT.count();
	return 0;
}