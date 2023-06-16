#include <iostream>
#include <map>
using namespace std;

struct Point {
    double x, y;
};

struct Segment {
    Point p1, p2;
};

bool onSegment(Point p, Point q, Point r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) {
        return true;
    }
    return false;
}

int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) {
        return 0; // collinear
    }
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool doIntersect(Segment s1, Segment s2) {
    Point p1 = s1.p1, q1 = s1.p2;
    Point p2 = s2.p1, q2 = s2.p2;

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    if (o1 == 0 && onSegment(p1, p2, q1)) {
        return true;
    }

    if (o2 == 0 && onSegment(p1, q2, q1)) {
        return true;
    }

    if (o3 == 0 && onSegment(p2, p1, q2)) {
        return true;
    }

    if (o4 == 0 && onSegment(p2, q1, q2)) {
        return true;
    }

    return false;
}

Point getIntersectionPoint(Segment s1, Segment s2) {
    double a1 = s1.p2.y - s1.p1.y;
    double b1 = s1.p1.x - s1.p2.x;
    double c1 = a1 * s1.p1.x + b1 * s1.p1.y;

    double a2 = s2.p2.y - s2.p1.y;
    double b2 = s2.p1.x - s2.p2.x;
    double c2 = a2 * s2.p1.x + b2 * s2.p1.y;

    double determinant = a1 * b2 - a2 * b1;
    if (determinant == 0) {
        // Segments are parallel or coincident, no intersection point
        return {0, 0};
    } else {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return {x, y};
    }
}

pair<int, int> findIntersectionSegment(map<pair<int, int>, Segment>& segments) {
    for (auto it1 = segments.begin(); it1 != segments.end(); ++it1) {
        for (auto it2 = next(it1); it2 != segments.end(); ++it2) {
            if (doIntersect(it1->second, it2->second)) {
                Point intersection = getIntersectionPoint(it1->second, it2->second);
                return {it1->first.first, it2->first.first};
            }
        }
    }
    return {-1, -1};
}
struct Node {
    pair<int, int> key;
    Node* left;
    Node* right;
    int height;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node* insert(Node* node, pair<int, int> key) {
    if (node == nullptr) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        // Duplicate keys are not allowed in this case
        return node;
    }

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key) {
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && key > node->right->key) {
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

bool search(Node* node, pair<int, int> key) {
    if (node == nullptr) {
        return false;
    }

    if (key < node->key) {
        return search(node->left, key);
    } else if (key > node->key) {
        return search(node->right, key);
    } else {
        return true;
    }
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

Node* deleteNode(Node* root, pair<int, int> key) {
    if (root == nullptr) {
        return root;
    }

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }

            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == nullptr) {
        return root;
    }

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0) {
        return rotateRight(root);
    }

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0) {
        return rotateLeft(root);
    }

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

pair<int, int> findIntersectionSegment(Node* root, map<pair<int, int>, Segment>& segments) {
    for (auto it1 = segments.begin(); it1 != segments.end(); ++it1) {
        for (auto it2 = next(it1); it2 != segments.end(); ++it2) {
            if (doIntersect(it1->second, it2->second)) {
                Point intersection = getIntersectionPoint(it1->second, it2->second);
                return {it1->first.first, it2->first.first};
            }
        }
    }
    return {-1, -1};
}

int main() {

    /*int n;
    cout << "Enter the number of segments: ";
    cin >> n;

    map<pair<int, int>, Segment> segments;
    cout << "Enter the segments coordinates:\n";
    for (int i = 1; i <= n; ++i) {
        double x1, y1, x2, y2;
        cout << "Segment " << i << " (x1 y1 x2 y2): ";
        cin >> x1 >> y1 >> x2 >> y2;
        segments[{i, i}] = {{x1, y1}, {x2, y2}};
    }

    pair<int, int> intersection = findIntersectionSegment(segments);
    if (intersection.first != -1) {
        cout << "Segments " << intersection.first << " and " << intersection.second << " intersect.\n";
    } else {
        cout << "No intersection point exists.\n";
    }

    return 0;*/
    int n;
    cout << "Enter the number of segments: ";
    cin >> n;

    Node* root = nullptr;
    map<pair<int, int>, Segment> segments;
    cout << "Enter the segments coordinates:\n";
    for (int i = 1; i <= n; ++i) {
        double x1, y1, x2, y2;
        cout << "Segment " << i << " (x1 y1 x2 y2): ";
        cin >> x1 >> y1 >> x2 >> y2;
        segments[{i, i}] = {{x1, y1}, {x2, y2}};
        root = insert(root, {i, i});
    }

    pair<int, int> intersection = findIntersectionSegment(root, segments);
    if (intersection.first != -1) {
        cout << "Segments " << intersection.first << " and " << intersection.second << " intersect.\n";
    } else {
        cout << "No intersection point exists.\n";
    }

    return 0;
}
