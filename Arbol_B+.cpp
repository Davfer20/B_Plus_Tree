#include <iostream>
using namespace std;

class Node
{
    int *keys;
    int tam;
    Node **listaNodos; // Lista de punteros
    int n;             // Es el orden
    bool leaf;

public:
    Node(int t1, bool leaf1);

    void insertNonFull(int k);
    void splitChild(int i, Node *y);
    void display();

    friend class BTree;
};

class BTree
{
private:
    Node *root;
    int tam;

public:
    BTree(int pTam)
    {
        root = NULL;
        this->tam = pTam;
    }

    void display()
    {
        if (root != NULL)
            root->display();
    }

    void insert(int k);
};

Node::Node(int t1, bool leaf1)
{
    tam = t1;
    leaf = leaf1;

    keys = new int[2 * tam - 1]; // Calcula el tamaño de la lista de nodos
    listaNodos = new Node *[2 * tam];
    n = 0;
}

void Node::display()
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            listaNodos[i]->display();
        cout << " " << keys[i];
    }

    if (leaf == false)
    {
        listaNodos[i]->display();
    }
}

void BTree::insert(int k)
{
    if (root == NULL)
    {
        root = new Node(tam, true);
        root->keys[0] = k;
        root->n = 1; // Se le asigna
    }
    else
    {
        if (root->n == 2 * tam - 1) // Las holas intermedias tienen como mínimo (m-1)/2(Parte entera), entonces pasa si rompe el tamaño
        {
            Node *newNode = new Node(tam, false);

            newNode->listaNodos[0] = root;

            newNode->splitChild(0, root);

            int i = 0;
            if (newNode->keys[0] < k)
                i++;
            newNode->listaNodos[i]->insertNonFull(k);

            root = newNode;
        }
        else
            root->insertNonFull(k);
    }
}

void Node::insertNonFull(int k)
{
    int i = n - 1; // Para el tamaño maximo

    if (leaf == true)
    {
        while (i >= 0 && keys[i] > k)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        n = n + 1; // Sube el nivel si sobrepasa el tamaño
    }
    else
    {
        while (i >= 0 && keys[i] > k) // Busca un lugar donde se debe insertar
            i--;

        if (listaNodos[i + 1]->n == 2 * tam - 1)
        {
            splitChild(i + 1, listaNodos[i + 1]);

            if (keys[i + 1] < k)
                i++;
        }
        listaNodos[i + 1]->insertNonFull(k);
    }
}

void Node::splitChild(int i, Node *y) // y es el root
{
    Node *nuevoNodoTemp = new Node(y->tam, y->leaf); // Se crea con el tamaño
    nuevoNodoTemp->n = tam - 1;

    for (int j = 0; j < tam - 1; j++)
        nuevoNodoTemp->keys[j] = y->keys[j + tam];

    if (y->leaf == false)
    {
        for (int j = 0; j < tam; j++)
            nuevoNodoTemp->listaNodos[j] = y->listaNodos[j + tam];
    }

    y->n = tam - 1;
    for (int j = n; j >= i + 1; j--)
        listaNodos[j + 1] = listaNodos[j];

    listaNodos[i + 1] = nuevoNodoTemp;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[tam - 1];
    n = n + 1;
}

int main()
{
    BTree tree(1);
    tree.insert(10);
    tree.insert(11);
    tree.insert(8);
    tree.insert(15);
    tree.insert(9);
    tree.insert(17);
    tree.insert(18);
    tree.insert(20);
    tree.insert(23);
    tree.insert(16);

    cout << "The B-tree is: \n";
    tree.display();
}