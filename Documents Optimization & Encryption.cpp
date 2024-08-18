#include <bits/stdc++.h>

using namespace std; 

#define pii pair < int, int >
#define psi pair < string, int >

struct node
{
    char ch;
    int fre;
    bool binary;
    node* top;
    node* left;
    node* right;

    node (int k): fre(k), ch('$'), binary(false), top(nullptr), left(nullptr), right(nullptr) {};
    node (int k, char ch): fre(k), ch(ch), top(nullptr), left(nullptr), right(nullptr) {};
};

struct Node
{
    node* headd; // Compression
    char ch;
    int value;
    int degree;
    Node* parent;
    Node* child; // Merge tree (same degree)
    Node* sibling; // Merge heap (different degree)

    Node (node* headd): headd(headd), value(headd -> fre), ch(headd -> ch), parent(nullptr), child(nullptr), sibling(nullptr), degree(0) {};
    Node (int k, char ch): headd(new node (k, ch)), value(k), ch(ch), parent(nullptr), child(nullptr), sibling(nullptr), degree(0) {};
};

class Customize_Heap_Structure // Min head
{
    private : 
        Node* head;

        Node* Merge_Tree (Node* h1, Node* h2)
        {
            if (h1 -> value > h2 -> value) swap (h1, h2); // pointer phá bỏ ràng buộc swap dễ dàng

            h2 -> sibling = h1 -> child;
            h2 -> parent = h1;
            h1 -> child = h2;
            h1 -> degree += 1;

            return h1;
        }

        Node* Merge_Heap (Node* h1, Node* h2)
        {
            Node* head_tmp = nullptr;
            Node* current = nullptr;
            Node* previous = nullptr;

            while (h1 || h2)
            {
                if (!h2)
                {
                    current = h1;
                    h1 = h1 -> sibling;
                }
                else if (!h1)
                {
                    current = h2;
                    h2 = h2 -> sibling;
                }
                else 
                {
                    if (h1 -> degree <= h2 -> degree)
                    {
                        current = h1;
                        h1 = h1 -> sibling;
                    }
                    else
                    {
                        current = h2;
                        h2 = h2 -> sibling;
                    }
                }
                if (head_tmp == nullptr) head_tmp = current;
                else previous -> sibling = current;
                previous = current;
            }

            return head_tmp;
        }

        void Consolidate () // re-build heap
        {
            if (head == nullptr || head -> sibling == nullptr) return;

            Node* previous = nullptr;
            Node* current = head;
            Node* next = head -> sibling;

            while (next != nullptr)
            {
                if (current -> degree != next -> degree || (next -> sibling != nullptr && current -> degree == next -> sibling -> degree))
                {
                    previous = current;
                    current = next;
                }
                else
                {   
                    Node* temp = next -> sibling;
                    current = Merge_Tree (current, next);
                    current -> sibling = temp;
                    if (previous == nullptr) head = current;
                    else previous -> sibling = current;
                }
                next = current -> sibling;
            }
        }

    public :
        Customize_Heap_Structure (): head(nullptr) {};

        void insert (int value, char ch, node* tmp)
        {
            Node* new_node;
            if (tmp == nullptr) new_node = new Node (value, ch);
            else new_node = new Node (tmp);

            if (head == nullptr) 
            {
                head = new_node;
                return;
            }
            head = Merge_Heap (head, new_node);
            Consolidate ();
        }

        void merge (Customize_Heap_Structure* h)
        {
            head = Merge_Heap (head, h -> head);
            Consolidate ();
        }

        void Change_Head (Node* min_node, Node** prev)
        {
            Node* curr = min_node -> child;
            Node* nextt = nullptr;
            while (curr != nullptr)
            {
                nextt = curr -> sibling;
                curr -> sibling = *prev;
                *prev = curr;
                curr = nextt;
            }
        }

        node* Extract_Min ()
        {
            Node* prev = nullptr;
            if (head == nullptr) throw runtime_error ("Head doesn't exist !!\n");
            else if (head -> sibling == nullptr)
            {
                node* tmp = head -> headd;
                Change_Head (head, &prev);
                head = prev;

                return tmp;
            }

            Node* previous = nullptr;
            Node* min_node = head;
            Node* temp = head;
            while (temp -> sibling != nullptr)
            {
                if (min_node -> value > temp -> sibling -> value)
                {
                    previous = temp;
                    min_node = temp -> sibling;
                } 
                temp = temp -> sibling;
            }

            Change_Head (min_node, &prev);
            if (previous == nullptr) head = min_node -> sibling;
            else previous -> sibling = min_node -> sibling;

            Customize_Heap_Structure* h = new Customize_Heap_Structure ();
            h -> head = prev;
            merge (h);

            return min_node -> headd;
        }

        bool empty ()
        {
            return head == nullptr;
        }

        int backtracking (Node* temp)
        {
            int cnt = 0;
            if (temp == nullptr) return cnt;

            Node* tmp = temp;
            cnt += backtracking (tmp -> child) + 1;
            while (temp -> sibling != nullptr)
            {
                Node* tmp = temp = temp -> sibling;
                cnt += backtracking (tmp -> child) + 1;
            }

            return cnt;
        }

        int size ()
        {
            int cnt = 0;
            Node* temp = head;
            while (temp != nullptr)
            {   
                cnt += backtracking (temp -> child) + 1;
                temp = temp -> sibling;
            }

            return cnt;
        }
};

class Compress_Structure
{
    private :
        Customize_Heap_Structure* head;
        node* root;
        vector < string > binary_character_;
        vector < string > list_binary_print_out;
        unordered_map < string, char > mp;

        void Bubble_down (node* head, string str)
        {
            if (head == nullptr) return;

            str += to_string (head -> binary);
            if (head -> ch != '$') 
            {
                binary_character_[head -> ch] = str;
                mp[str] = head -> ch;
            }
            
            Bubble_down (head -> left, str);
            Bubble_down (head -> right, str);
        }

    public :
        Compress_Structure (): head(new Customize_Heap_Structure ()), root(nullptr), binary_character_(256) {};

        void init_ (vector < int > vlist)
        {
            for (int x = 0; x < vlist.size (); ++x)
            {
                if (vlist[x] > 0)
                {
                    head -> insert (vlist[x], x, nullptr);
                } 
            }
        }

        void Build_Structure ()
        {
            while (head -> size () > 1)
            {
                node* a = head -> Extract_Min ();
                node* b = head -> Extract_Min ();

                node* temp = new node (a -> fre + b -> fre);
                a -> binary = false;
                b -> binary = true;

                temp -> left = a; 
                temp -> right = b;
                a -> top = b -> top = temp;

                root = temp;
                head -> insert (root -> fre, root -> ch, root);
            }
        }

        void Compress_File ()
        {
            cout << endl << "----> Binary Compressed File :";
            string str = "";
            Bubble_down (root, str);
            cout << endl;
        }

        void Decompress_File ()
        {
            cout << "\n----> Original File Text : \n";
            for (string str : list_binary_print_out)
            {
                if (str == "%") cout << endl;
                else cout << mp[str];
            }
        }

        void print_file_in_binary (vector < string > vstring)
        {
            for (string str : vstring)
            {
                for (char ch : str)
                {
                    list_binary_print_out.push_back (binary_character_[ch]);
                    cout << binary_character_[ch];
                }
                list_binary_print_out.push_back ("%");
                cout << endl;
            }
        }
};

class Encryption
{
    private :
        static const int SIZE = 1001;
        vector < list < string >> vlist;

    public :
        Encryption (): vlist(SIZE) {};

        int EncryptionFunction (string str)
        {
            const int k = 101;
            const int mod = 1e9 + 7;

            long long cnt = 0, p = 0;
            for (int i = str.length (); i >= 0; --i)
            {
                cnt += (str[i] + static_cast < long long > (pow (k, i))) % mod;
                ++p;
            }

            return cnt % SIZE;
        }

        bool isExist (string str)
        {
            int index  = EncryptionFunction (str);
            for (string s : vlist[index])
            {
                if (s == str) return true;
            }
            return false;
        }

        void insert__ (string str)
        {
            int index = EncryptionFunction (str);
            for (string s : vlist[index])
            {
                if (s == str) return;
            }
            vlist[index].push_back (str);
        }

        void delete__ (string str)
        {
            int index = EncryptionFunction (str);
            vlist[index].remove (str);
        }
};

vector < string > implement_file (string str, vector < int > &frequency)
{
    vector < string > v;
    ifstream my_file (str);
    string st;
    while (getline (my_file, st, ' '))
    {
        v.push_back (st);
    }    
    my_file.close ();

    frequency.assign (256, 0);
    for (string str : v)
    {
        for (char ch : str) ++frequency[ch];
    }  

    return v;
}

bool password (string str)
{
    cout << endl;

    Encryption* h = new Encryption ();
    h -> insert__ (str);

    int i = 3;
    while (i--)
    {
        cout << "---> Enter the key password to unlock file : ";
        string str; 
        getline (cin, str);
        if (h -> isExist (str)) return true;
        cout << "Try again !!\n";
    }
    cout << "\n + You have been wrong 3 times 😡\n";
    cout << " ~~ Break the program ~~\n";
    return false;
}

int main ()
{   
    cout << "\n ~ Type correctly the file name here : ";
    string str; getline (cin, str);

    vector < int > frequency;
    vector < string > vstring = implement_file (str, frequency);

    Compress_Structure* h = new Compress_Structure ();
    h -> init_ (frequency);
    h -> Build_Structure ();
    h -> Compress_File ();
    h -> print_file_in_binary (vstring);

    if (!password (str)) return 0; // Enter password with 3 chances
    h -> Decompress_File ();


    return 0;
}