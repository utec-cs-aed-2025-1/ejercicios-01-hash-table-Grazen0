#include "chainhash.h"
#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr)
    {
    }
};

// crear una lista desde un vector
ListNode *createList(const vector<int> &values)
{
    if (values.empty())
        return nullptr;

    ListNode *head = new ListNode(values[0]);
    ListNode *current = head;

    for (size_t i = 1; i < values.size(); i++) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }

    return head;
}

// imprimir una lista
void printList(ListNode *head, int maxNodes = 10)
{
    ListNode *current = head;
    int count = 0;

    cout << "[";
    while (current && count < maxNodes) {
        cout << current->val;
        if (current->next && count < maxNodes - 1)
            cout << " -> ";
        current = current->next;
        count++;
    }
    if (current)
        cout << " -> ...";
    cout << "]";
}

// encontrar un nodo por valor en una lista
ListNode *findNode(ListNode *head, int value)
{
    while (head) {
        if (head->val == value)
            return head;
        head = head->next;
    }
    return nullptr;
}

void connectLists(ListNode *listA, ListNode *&listB, int intersectVal)
{
    ListNode *intersectNode = findNode(listA, intersectVal);
    if (intersectNode == nullptr)
        return;

    ListNode **cur = &listB;
    while (*cur != nullptr) {
        if ((*cur)->val == intersectVal) {
            *cur = intersectNode;
            return;
        }

        cur = &(*cur)->next;
    }
}

ListNode *getIntersectionNode(ListNode *headA, ListNode *headB)
{
    ChainHash<ListNode *, bool> seen;

    ListNode *cur = headA;

    while (cur != nullptr) {
        seen.set(cur, true);
        cur = cur->next;
    }

    cur = headB;

    while (cur != nullptr) {
        if (seen.contains(cur))
            return cur;

        cur = cur->next;
    }

    return nullptr;
}

void testIntersection(const vector<int> &listA, const vector<int> &listB,
                      int intersectVal, const string &testName)
{
    cout << "\n" << testName << "\n";

    ListNode *headA = createList(listA);
    ListNode *headB = createList(listB);

    // crear interseccion
    if (intersectVal != -1) {
        connectLists(headA, headB, intersectVal);
    }

    cout << "Lista A: ";
    printList(headA);
    cout << "\nLista B: ";
    printList(headB);
    cout << "\n";

    // encontrar interseccion
    ListNode *intersection = getIntersectionNode(headA, headB);

    if (intersection) {
        cout << "Intersección encontrada en nodo con valor: "
             << intersection->val << "\n";
    } else {
        cout << "No hay intersección\n";
    }

    if (intersectVal == -1) {
        if (intersection == nullptr) {
            cout << "CORRECTO: No se esperaba intersección\n";
        } else {
            cout << "ERROR: Se encontró intersección cuando no debería\n";
        }
    } else {
        if (intersection && intersection->val == intersectVal) {
            cout << "CORRECTO: Intersección encontrada en el nodo esperado\n";
        } else {
            cout << "ERROR: Intersección incorrecta o no encontrada\n";
        }
    }
}

int main()
{
    cout << "INTERSECCION DE LISTAS ENLAZADAS\n";
    cout << "Problema: Encontrar el nodo donde dos listas se intersectan\n";

    // Caso 1: Intersección existe
    testIntersection({4, 1, 8, 4, 5}, {5, 6, 1, 8, 4, 5}, 8,
                     "Caso 1: Con intersección");

    // Caso 2: Sin intersección
    testIntersection({2, 6, 4}, {1, 5}, -1, "Caso 2: Sin intersección");

    // Caso 3: Intersección al inicio
    testIntersection({3, 7, 1}, {3, 7, 1}, 3, "Caso 3: Intersección al inicio");

    // Caso 4: Una lista vacía
    testIntersection({1, 2, 3}, {}, -1, "Caso 4: Lista B vacía");

    // Caso 5: Listas de diferente longitud con intersección
    testIntersection({1, 9, 1, 2, 4}, {3, 2, 4}, 2,
                     "Caso 5: Diferentes longitudes con intersección");

    return 0;
}
