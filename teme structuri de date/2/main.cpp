#include <iostream>

class Node {
private:
    int data;
    Node* next;

public:
    Node(int value) : data(value), next(nullptr) {}
    friend class CircularList;
    int get_data(){
        return data;
    }
};

class CircularList {
private:
    Node* head;

public:
    CircularList() : head(nullptr) {}

    void insert(int value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
            head->next = head;
        } else {
            Node* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->next = head;
        }
    }

    void print() {
        if (head == nullptr) {
            std::cout << "List is empty." << std::endl;
            return;
        }

        Node* temp = head;
        do {
            std::cout << temp->data << " ";
            temp = temp->next;
        } while (temp != head);
        std::cout << std::endl;
    }

    void remove(int value) {
        if (head == nullptr) {
            std::cout << "List is empty." << std::endl;
            return;
        }

        Node* current = head;
        Node* previous = nullptr;

        do {
            if (current->data == value) {
                if (previous != nullptr) {
                    previous->next = current->next;
                } else {
                    // Removing the head node
                    Node* lastNode = head;
                    while (lastNode->next != head) {
                        lastNode = lastNode->next;
                    }
                    lastNode->next = head->next;
                    head = head->next;
                }
                delete current;
                std::cout << "Element " << value << " removed from the list." << std::endl;
                return;
            }

            previous = current;
            current = current->next;
        } while (current != head);

        std::cout << "Element " << value << " not found in the list." << std::endl;
    }

    Node* findThirdFrom(Node* startNode) {
        if (head == nullptr) {
            std::cout << "List is empty." << std::endl;
            return nullptr;
        }

        Node* current = startNode;
        for (int i = 0; i < 2; ++i) {
            if (current->next == head) {
                std::cout << "Node doesn't have a third element from it." << std::endl;
                return nullptr;
            }
            current = current->next;
        }

        return current;
    }

    Node* getHead() const {
        return head;
    }

    Node* josephus(int startNodeIndex, int k) {
        if (head == nullptr) {
            std::cout << "List is empty." << std::endl;
            return nullptr;
        }

        Node* current = head;
        Node* previous = nullptr;

        // Finding the start node
        for (int i = 0; i < startNodeIndex; ++i) {
            previous = current;
            current = current->next;

            // We check if the start node is valid
            if (current == nullptr) {
                std::cout << "Invalid start node index." << std::endl;
                return nullptr;
            }
        }


        while (current->next != current) {

            for (int i = 0; i < k - 1; ++i) {
                previous = current;
                current = current->next;
            }


            Node* eliminatedNode = current;
            previous->next = current->next;
            current = current->next;
            delete eliminatedNode;
        }

        return current;
    }


private:
    Node* getNodeByIndex(int index) {
        if (head == nullptr) {
            return nullptr;
        }

        Node* current = head;
        int count = 0;
        do {
            if (count == index) {
                return current;
            }
            current = current->next;
            count++;
        } while (current != head);

        return nullptr;
    }
};

int main() {
    CircularList list;


    for (int i = 1; i < 21; ++i) {
        list.insert(i);
    }


    std::cout << "Elements in the list: ";
    list.print();


    int startingNodeIndex;
    std::cout << "Enter the index of the starting node: ";
    std::cin >> startingNodeIndex;

    int k;
    std::cout << "Enter the value of k: ";
    std::cin >> k;

    Node* lastSurvivor = list.josephus(startingNodeIndex, k);
    if (lastSurvivor != nullptr) {
        std::cout << "Last node to survive: " << lastSurvivor->get_data() << std::endl;
    }

    return 0;
}
