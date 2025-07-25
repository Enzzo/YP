#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    int v = node->value;

    if (node->left != nullptr) {
        auto lv = node->left->value;
        if (!CheckTreeProperty(node->left.get()) || lv > v) {
            return false;
        }
    }
    if (node->right != nullptr) {
        auto rv = node->right->value;
        if (!CheckTreeProperty(node->right.get()) || v > rv) {
            return false;
        }
    }
    
    return true;
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    if (node->left) {
        return begin(node->left.get());
    }
    return node;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if (node->right) {
        return begin(node->right.get());
    }

    TreeNode<T>* next_node = node->parent;
    while (next_node != nullptr && next_node->value < node->value) {
        next_node = next_node->parent;
    }
    return next_node;
}

// ������� ������ ����� ���� � �������� ��������� � ���������
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {

    TreeNodePtr<int> result = std::make_unique<TreeNode<int>>(val, std::move(left), std::move(right));
    if(result->left)
        result->left->parent = result.get();
    if(result->right)
        result->right->parent = result.get();

    return result;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    // ������� DeleteTree �� �����. ���� ������ ����� ���������� �������
    // ��������� ������������ unique_ptr
}