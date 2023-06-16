#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include <vector>
#include <chrono>


class Node {
private:
    std::string key;
    int value;
    Node* next;

public:
    Node(const std::string& k, int v) : key(k), value(v), next(nullptr) {}

    std::string getKey() const {
        return key;
    }

    int getValue() const {
        return value;
    }

    Node* getNext() const {
        return next;
    }

    void setNext(Node* node) {
        next = node;
    }
    void setValue(int x){
        value=x;
    }
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    void insert(const std::string& key, int value) {
        Node* newNode = new Node(key, value);

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* currNode = head;
            while (currNode->getNext() != nullptr) {
                currNode = currNode->getNext();
            }
            currNode->setNext(newNode);
        }
    }

    void remove(const std::string& key) {
        Node* prevNode = nullptr;
        Node* currNode = head;

        while (currNode != nullptr && currNode->getKey() != key) {
            prevNode = currNode;
            currNode = currNode->getNext();
        }

        if (currNode == nullptr) {
            std::cout << "Element not found in the list." << std::endl;
            return;
        }

        if (prevNode == nullptr) {
            head = currNode->getNext();
        } else {
            prevNode->setNext(currNode->getNext());
        }

        delete currNode;
    }

    void print() const {
        Node* currNode = head;

        while (currNode != nullptr) {
            std::cout << "(" << currNode->getKey() << ", " << currNode->getValue() << ") ";
            currNode = currNode->getNext();
        }

        std::cout << std::endl;
    }
    int search(const std::string& key) const {
        Node* currNode = head;

        while (currNode != nullptr) {
            if (currNode->getKey() == key) {
                return currNode->getValue();
            }
            currNode = currNode->getNext();
        }


        return -1;
    }
};

struct SkipListElement {
    std::string key;
    int value;
    std::vector<SkipListElement*> next;

    SkipListElement(const std::string& k, int v, int levels) : key(k), value(v), next(levels, nullptr) {}
};

class SkipList {
private:
    int maxLevels;
    int currentLevels;
    SkipListElement* head;

    int getRandomLevels() {
        int levels = 1;
        while ((std::rand() % 2) && levels < maxLevels)
            levels++;
        return levels;
    }

public:
    SkipList(int levels) : maxLevels(levels), currentLevels(1), head(nullptr) {
        head = new SkipListElement("", 0, maxLevels);
    }

    void insert(const std::string& key, int value) {
        int levels = getRandomLevels();
        SkipListElement* newNode = new SkipListElement(key, value, levels);
        std::vector<SkipListElement*> update(maxLevels, nullptr);
        SkipListElement* curr = head;

        for (int i = currentLevels - 1; i >= 0; i--) {
            while (curr->next[i] && curr->next[i]->key.compare(key) < 0) {
                curr = curr->next[i];
            }
            update[i] = curr;
        }

        curr = curr->next[0];

        if (curr && curr->key == key) {
            curr->value = value;
            delete newNode;
            return;
        }

        if (levels > currentLevels) {
            for (int i = currentLevels; i < levels; i++) {
                update[i] = head;
            }
            currentLevels = levels;
        }

        for (int i = 0; i < levels; i++) {
            newNode->next[i] = update[i]->next[i];
            update[i]->next[i] = newNode;
        }
    }

    void remove(const std::string& key) {
        std::vector<SkipListElement*> update(maxLevels, nullptr);
        SkipListElement* curr = head;

        for (int i = currentLevels - 1; i >= 0; i--) {
            while (curr->next[i] && curr->next[i]->key < key) {
                curr = curr->next[i];
            }
            update[i] = curr;
        }

        if (curr->next[0] && curr->next[0]->key == key) {
            SkipListElement* toRemove = curr->next[0];
            for (int i = 0; i < currentLevels; i++) {
                if (update[i]->next[i] != toRemove)
                    break;
                update[i]->next[i] = toRemove->next[i];
            }

            delete toRemove;

            while (currentLevels > 1 && head->next[currentLevels - 1] == nullptr) {
                currentLevels--;
            }
        }
    }

    void print() {
        SkipListElement* curr = head->next[0];
        while (curr) {
            std::cout << "(" << curr->key << ", " << curr->value << " )" << std::endl;
            curr = curr->next[0];
        }
    }

    bool search(const std::string& key) {
        SkipListElement* curr = head;
        for (int i = currentLevels - 1; i >= 0; i--) {
            while (curr->next[i] && curr->next[i]->key < key) {
                curr = curr->next[i];
            }
        }

        curr = curr->next[0];

        if (curr && curr->key == key) {
            return true;
        }

        return false;
    }
};
std::string generateRandomName() {
    std::string vowels = "aeiou";
    std::string consonants = "bcdfghjklmnpqrstvwxyz";

    std::string name;
    int length = rand() % 5 + 5;
    for (int i = 0; i < length; i++) {
        if (i % 2 == 0) {
            int index = rand() % consonants.length();
            name += consonants[index];
        } else {

            int index = rand() % vowels.length();
            name += vowels[index];
        }
    }

    return name;
}

long long generateRandomNumber() {
    int min=100000;
    int max=999999;
    return min + rand() % (max - min + 1);
}

int generateRandomIndex(int max) {
    return rand() % (max);
}




int main() {
    std::vector<std::string> names;
    std::vector<int> phoneNumbers;
    LinkedList linkedList;
    SkipList skipList(10);

    struct NameEntry {
        std::string name;
        int bit;
    };

    std::vector<NameEntry> nameVector;



    for (int i = 0; i < 10000; i++) {
        std::string name = generateRandomName();
        int phoneNumber = generateRandomNumber();
        names.push_back(name);
        phoneNumbers.push_back(phoneNumber);
    }
    for (const auto& name : names) {
        nameVector.push_back({ name, 1 });
    }
    auto startLinkedListCreation = std::chrono::steady_clock::now();
    for(int i=0;i<10000;i++){
        linkedList.insert(names[i], phoneNumbers[i]);
    }
    auto endLinkedListCreation = std::chrono::steady_clock::now();
    auto linkedListCreationTime = std::chrono::duration_cast<std::chrono::milliseconds>(endLinkedListCreation - startLinkedListCreation).count();
    std::cout<<"Linked list creation time:"<<linkedListCreationTime<<" ms"<<"\n";
    auto startSkipListCreation = std::chrono::steady_clock::now();
    for(int i=0;i<10000;i++){
        skipList.insert(names[i],phoneNumbers[i]);
    }
    auto endSkipListCreation = std::chrono::steady_clock::now();
    auto SkipListCreationTime = std::chrono::duration_cast<std::chrono::milliseconds>(endSkipListCreation - startSkipListCreation).count();
    std::cout<<"Skip list creation tine:"<<SkipListCreationTime<<" ms"<<std::endl;

    for (const auto& name : names) {
        nameVector.push_back({ name, 1 });
    }
    int numSearches = 10000;
    int c=0;
    auto startLinkedListSearch = std::chrono::steady_clock::now();
    for (int i = 0; i < numSearches; i++) {
        int randomIndex = generateRandomIndex(10000);

        while(nameVector[randomIndex].bit!=1){
            randomIndex = generateRandomIndex(10000);
        }
        const std::string& name = nameVector[randomIndex].name;
        nameVector[randomIndex].bit=0;
        linkedList.search(name);
    }
    for(int i=0;i<10000;i++){
        nameVector[i].bit=1;
    }
    auto endLinkedListSearch = std::chrono::steady_clock::now();
    auto linkedListSearchTime = std::chrono::duration_cast<std::chrono::milliseconds>(endLinkedListSearch - startLinkedListSearch).count();
    std::cout << "Linked List search time: " << linkedListSearchTime << " ms" << std::endl;
    auto startSkipListSearch = std::chrono::steady_clock::now();
    for (int i = 0; i < numSearches; i++) {
        int randomIndex = generateRandomIndex(10000);
        while(nameVector[randomIndex].bit!=1){
            randomIndex = generateRandomIndex(10000);
        }
        const std::string& name = nameVector[randomIndex].name;
        nameVector[randomIndex].bit=0;
        skipList.search(name);
    }
    auto endSkipListSearch = std::chrono::steady_clock::now();
    auto skipListSearchTime = std::chrono::duration_cast<std::chrono::milliseconds>(endSkipListSearch - startSkipListSearch).count();
    std::cout << "Skip List search time: " << skipListSearchTime << " ms" << std::endl;

    for(int i=0;i<10000;i++){
        nameVector[i].bit=1;
    }
    std::vector<int> deletionIndices;
    for (int i = 0; i < 10000; i++) {
        int randomIndex = generateRandomIndex(10000);
        while(nameVector[randomIndex].bit!=1)
            randomIndex = generateRandomIndex(10000);
        deletionIndices.push_back(i);
        nameVector[randomIndex].bit=0;

    }
    auto startLinkedListDeletion = std::chrono::steady_clock::now();
    for (const auto& index : deletionIndices) {
        const std::string& name = nameVector[index].name;
        linkedList.remove(name);
    }
    auto endLinkedListDeletion = std::chrono::steady_clock::now();
    auto linkedListDeletionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endLinkedListDeletion - startLinkedListDeletion).count();


    auto startSkipListDeletion = std::chrono::steady_clock::now();
    for (const auto& index : deletionIndices) {
        const std::string& name = nameVector[index].name;
        skipList.remove(name);
    }
    auto endSkipListDeletion = std::chrono::steady_clock::now();
    auto skipListDeletionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endSkipListDeletion - startSkipListDeletion).count();

    std::cout << "Linked List deletion time: " << linkedListDeletionTime << " ms" << std::endl;
    std::cout << "Skip List deletion time: " << skipListDeletionTime << " ms" << std::endl;
    return 0;
}
