#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
template <typename T>
class node
{
public:
    std::shared_ptr<node<T>> left = nullptr;
    std::shared_ptr<node<T>> right = nullptr;
    T value;

    node<T>(T val) : value(val) {   }
};
template <typename T>
class Tree
{
    int size = 0;
    std::shared_ptr<node<T>> root;

    void Traverse_(  std::shared_ptr<node<T>>& current, void func(  std::shared_ptr<node<T>>))
    {
        if(current==nullptr) return;
        func(current);
        Traverse_( current->right, func);

        Traverse_( current->left, func);
    }
    void copy_element( std::shared_ptr<node<T>> node_ ){
        if( !node_ ) return;
        this->AddNode( node_->value );
        this->copy_element( node_->left );
        this->copy_element( node_->right );

    }

    void Serialize_(std::ofstream& file,std::shared_ptr<node<T>>& current )
    {
        if(!current) return;
        std::stringstream ss;
        ss << current->value<<std::endl;
        file<<ss.str();
        Serialize_(file, current->left);
        Serialize_(file, current->right);
    }
    void Deserialize_(std::ifstream& file,std::shared_ptr<node<T>>& current )
    {
        while(!file.eof())
        {
            std::stringstream ss;
            T value;
            std::string Fvalue;
            std::getline(file, Fvalue);
            if(Fvalue!="")
            {
                ss<<Fvalue;
                ss>>value;
                this->AddNode(value);
            }

        }
    }
public:

    void AddNode(T val ){
        if(root == nullptr)
        {
            root = std::make_shared<node<T>>(val);
            root->left = nullptr;
            root->right = nullptr;
            return;
        }
        std::shared_ptr<node<T>> current = root;
        std::shared_ptr<node<T>> parent = nullptr;
        while(current != nullptr)
        {
            parent = current;
            if (val > current->value) {
                current = current->right;

            } else{
                current = current->left;
            }
        }
        current = std::make_shared<node<T>>(val);
        current->left = nullptr;
        current->right = nullptr;
        if(val > parent->value) parent->right = std::move(current);
        else parent->left = std::move(current);
        size++;
    }
    void Traverse(void func(  std::shared_ptr<node<T>>))
    {
        Traverse_(root, func);
    }
    std::shared_ptr<node<T>> Search(T val)
    {
        std::shared_ptr<node<T>> current = root;
        while(current != nullptr)
        {
            if(val == current->value) break;
            if (val > current->value) {
                current = current->right;

            } else
                {
                current = current->left;
            }
        }
        return current;
    }

    void Serialize(std::ofstream& file)
    {
        Serialize_(file, root);
    }
    void Deserialize(std::ifstream& file)
    {
        Deserialize_(file, root);
    }

    // Constructors & operators
    Tree& operator= ( const Tree & other)
    {
        root = std::make_shared<node<T>>(other.root->value);
        size = other.size;
        this->copy_element(other.root);

        return *this;
    }
    Tree& operator= (Tree&& other)
     {
        root = std::move(other.root);

        size = other.size;

        other.size = 0;
        return *this;
    }
    Tree(){
        root = nullptr;
    }
    Tree(T rootval){
        root = std::make_shared<node<T>>(rootval);
        size = 1;
    }
    Tree(Tree && other){
        root = std::move(other.root);
        size = other.size;
        other.size = 0;
    }

    explicit Tree(Tree & other) {

        root = std::make_shared<node<T>>(other.root->value);
        size = other.size;
        this->copy_element(other.root);

    }

};




int main() {
    std::cout << "Hello, World!" << std::endl;
    Tree<int> tree(9);

    tree.AddNode(8);
    tree.AddNode(12);
    tree.AddNode(11);
    tree.AddNode(13);
    tree.AddNode(5);
    tree.AddNode(6);
    tree.AddNode(3);
    tree.AddNode(4);
    tree.AddNode(1);

    auto tree2 = std::move(tree);
    std::cout<<"searching ..."<<std::endl;
    std::shared_ptr<node<int>> temp = tree2.Search(11);

    (bool)temp ? std::cout<<temp->value<<std::endl : std::cout<<"Not found"<<std::endl;
    Tree<int> CopyTree(tree2);
    CopyTree.Traverse([](auto node){std::cout<<node->value<<std::endl; });
    tree2.Traverse([](auto node){std::cout<<node->value<<std::endl; });
    std::ofstream File("tree1.txt");
    {

        if (File.is_open()) {
            tree2.Serialize(File);

            File.close();
            std::cout << "SAVED" << std::endl;
        }
    }
    Tree<int> tree3;
    {
        std::ifstream Input("tree1.txt");
        if (Input.is_open()) {
            tree3.Deserialize(Input);
            Input.close();
        }
    }


    {
        std::ofstream File2("tree2.txt");
        if (File2.is_open()) {
            tree3.Serialize(File2);

            File2.close();
            std::cout << "SAVED" << std::endl;
        }
    }

    std::cout << "-------------------------" << std::endl;

    Tree<int> tree4;
    tree4 = tree2;
    tree2.Traverse([](auto node){std::cout<<node->value<<std::endl; });
    tree4.Traverse([](auto node){std::cout<<node->value<<std::endl; });

    return 0;
}
