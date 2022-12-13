#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <numeric>

#define LOGGING false

#if LOGGING == true
#define LOG(s) std::cout << s
#else
#define LOG(s)
#endif

// split method from here: https://www.reddit.com/r/cpp/comments/5dxnwm/comment/da8ewky/?utm_source=share&utm_medium=web2x&context=3
std::vector <std::string> split(const std::string &s, const char delimiter) {
    std::vector <std::string> result;

    std::size_t current = 0;
    std::size_t p = s.find_first_of(delimiter, 0);

    while (p != std::string::npos) {
        // emplace_back will call the constructor of std::string
        result.emplace_back(s, current, p - current);
        current = p + 1;
        p = s.find_first_of(delimiter, current);
    }

    result.emplace_back(s, current);
    return result;
}


enum NodeType : char {
    DIR = 'D',
    FILEENTRY = 'F'
};

std::ostream &operator<<(std::ostream &os, NodeType type) {
    return os << static_cast<char>(type);
}

struct TreeNode {
    long long size = 0;
    std::string name;
    NodeType type;
    std::vector <TreeNode> children;
    TreeNode *parent = nullptr;

    TreeNode(long long size, std::string name, NodeType type) : size(size), name(name), type(type) {
    }

    void add(TreeNode child) {

        if (this->type != NodeType::DIR) {
            throw std::runtime_error("Only dirs can have children!");
        }

        child.parent = this;
        this->children.push_back(child);
    }

    void print() const {
        this->print(0);
    }

    void print(int indent_size) const {
        const std::string indent(indent_size * 2, ' ');

        const auto parentName = (this->parent == nullptr) ? "NO PARENT" : this->parent->name;

        std::cout << indent << "- " << this->name << " (" << this->type << ", size=" << this->size << ", parent="
                  << parentName << ")" << "\n";

        for (const auto &child: this->children) {
            child.print(indent_size + 1);
        }
    }

    TreeNode *getDirectoryNode(const std::string &name) {
        for (auto &child: children) {
            if (child.name == name) {
                return &child;
            }
        }

        // by definition this should not happen since we will never navigate into a non existing folder.
        assert(false);
        return nullptr;
    }

    long long updateFileSize() {
        if (this->type == NodeType::FILEENTRY) {
            return this->size;
        }

        long long sizeSum = std::accumulate(this->children.begin(), this->children.end(), (long long) 0,
                                            [](int val, TreeNode &child) {
                                                return val + child.updateFileSize();
                                            });

        this->size = sizeSum;

        return sizeSum;
    }

    int printFoldersWithLessThan100k() {
        if (this->type == NodeType::FILEENTRY) {
            return 0;
        }

        int returnSize = 0;
        if (this->size <= 100000) {
            LOG("Folder: " << this->name << " has size: " << this->size << std::endl);
            returnSize += this->size;
        }

        return std::accumulate(this->children.begin(), this->children.end(), returnSize,
                               [](int val, TreeNode &node) { return val + node.printFoldersWithLessThan100k(); });
    }
};

void createFileTree(TreeNode &root) {
    auto currentPosition = &root;

    for (std::string line; std::getline(std::cin, line);) {
        if (line == "$ cd /") {
            LOG("Changing to root dir" << std::endl);
            currentPosition = &root;
        } else if (line == "$ cd ..") {
            LOG("Changing to parent dir" << std::endl);
            currentPosition = currentPosition->parent;
        } else if (line.rfind("$ cd ", 0) == 0) {
            // dir change
            const auto targetDir = line.substr(5);
            LOG("Changing to dir: " << targetDir << std::endl);

            // find dir in current and change dir
            currentPosition = currentPosition->getDirectoryNode(targetDir);
        } else if (line == "$ ls") {
            // do nothing
        } else if (line.rfind("dir ", 0) == 0) {
            // we a new dir
            currentPosition->add({0, line.substr(4), NodeType::DIR});
        } else {
            // we have a new file!
            // file line format "[size] [name]"
            const auto parts = split(line, ' ');
            const auto fileSize = std::stoi(parts[0]);

            currentPosition->add({fileSize, parts[1], NodeType::FILEENTRY});
        }
    }
}

int main() {

    TreeNode root{0, "/", NodeType::DIR};

    createFileTree(root);

    root.updateFileSize();
    root.print();
    
    std::cout << "\n";
    std::cout << "Sum of folders with size less eq than 100 000: " << root.printFoldersWithLessThan100k() << std::endl;

    return 0;
}