#include <bits/stdc++.h>

using namespace std;

#define pii pair < int, int >
#define psi pair < string, int >
#define limit 10001

struct Node // Cirucular Doubly Linked List Concepts => Union : O(1)
{
    int value;
    int number;
    int degree;
    Node* parent;
    Node* child;
    Node* right;
    Node* left;
    bool marked; // for Decrease_Key

    Node (int k, int l): value(k), number(l), degree(0), parent(nullptr), child(nullptr), right(nullptr), left(nullptr), marked(false) {};
};

class Customize_Data_Structure // Accompanying with Dijkstra algorithm
{
    private :

        int total_node;
        Node* minimum_node;
        vector < Node* > save_pointer;

        Node* merge_tree (Node* h1, Node* h2)
        {
            if (h1 -> value > h2 -> value) swap (h1, h2);
            h2 -> parent = h1;
            ++h1 -> degree;

            h2 -> right -> left = h2 -> left;
            h2 -> left -> right = h2 -> right;

            if (h1 -> child == nullptr) h2 -> right = h2 -> left = h2;
            else
            {
                h2 -> right = h1 -> child;
                h2 -> left = h1 -> child -> left;
                h1 -> child -> left -> right = h2;
                h1 -> child -> left = h2;
            }
            h1 -> child = h2;

            return h1;
        }

        void cut_arc (Node* par, Node* head)
        {   
            head -> marked = false; // even the cut-node will also be unmarked
            --par -> degree;

            head -> parent = nullptr;
            if (head == head -> right) par -> child = nullptr;
            else 
            {
                par -> child = head -> right;
                head -> right -> left = head -> left;
                head -> left -> right = head -> right;
            }

            minimum_node -> right -> left = head;
            head -> left = minimum_node;
            head -> right = minimum_node -> right;
            minimum_node -> right = head;
            if (head -> value < minimum_node -> value) minimum_node = head;
        }

    public :
        Customize_Data_Structure (int SIZE): total_node(0), minimum_node(nullptr), save_pointer(SIZE, nullptr) {};

        void insert (int number, int number_node)
        {
            Node* new_node = new Node (number, number_node);
            if (minimum_node == nullptr)
            {
                minimum_node = new_node;
                minimum_node -> right = minimum_node;
                minimum_node -> left = minimum_node;
            }
            else
            {
                new_node -> right = minimum_node -> right;
                minimum_node -> right -> left = new_node;

                new_node -> left = minimum_node;
                minimum_node -> right = new_node;

                if (new_node -> value < minimum_node -> value) minimum_node = new_node;
            }
            save_pointer[number_node] = new_node;
            ++total_node;
        }

        void merge_heap (Customize_Data_Structure* h)
        {
            if (h -> minimum_node == nullptr) return;
            else if (minimum_node == nullptr)
            {
                minimum_node = h -> minimum_node;
                total_node = h -> total_node;
            }
            else
            {
                Node* root_of_h = h -> minimum_node;
                Node* temp_1 = root_of_h -> right;
                Node* temp_2 = minimum_node -> right;

                minimum_node -> right = temp_1;
                root_of_h -> right = temp_2;

                temp_1 -> left = minimum_node;
                temp_2 -> left = root_of_h; 

                if (root_of_h -> value < minimum_node -> value) minimum_node = root_of_h;
                total_node += h -> total_node;
            }
            h -> minimum_node = nullptr;
            h -> total_node = 0;
        }

        void decrease_key (Node* current_ptr, int new_value)
        { 
            if (current_ptr -> right == nullptr) return;

            current_ptr -> value = new_value;
            if (current_ptr -> parent == nullptr)
            {
                if (current_ptr -> value < minimum_node -> value) minimum_node = current_ptr;
            }
            else if (current_ptr -> value < current_ptr -> parent -> value)
            {
                Node* ptr_parent = nullptr;
                do
                {   
                    ptr_parent = current_ptr -> parent;
                    cut_arc (ptr_parent, current_ptr);
                    current_ptr = ptr_parent;
                } while (current_ptr -> parent != nullptr && current_ptr -> marked == true);
                ptr_parent -> marked = true;
            }
        }

        void delete_min ()
        {
            if (minimum_node == nullptr) return;

            --total_node;
            minimum_node -> degree = 0;
            if (minimum_node -> right == minimum_node)
            {
                if (minimum_node -> child != nullptr) 
                {
                    Node* ptr = minimum_node -> child;
                    minimum_node -> left = minimum_node -> right = minimum_node -> child = nullptr;
                    Node* current = minimum_node = ptr;
                    do
                    {
                        current -> parent = nullptr;
                        if (current -> value < minimum_node -> value) minimum_node = current;
                        current = current -> right;
                    } while (current != ptr);
                }
                else minimum_node = minimum_node -> child = minimum_node -> left = minimum_node -> right = nullptr;
                return;
            } 

            minimum_node -> left -> right = minimum_node -> right;
            minimum_node -> right -> left = minimum_node -> left;

            Node* prev = minimum_node -> left;
            Node* next = minimum_node -> right;
            minimum_node -> left = minimum_node -> right = nullptr;

            if (minimum_node -> child != nullptr)
            {  
                Node* child_temp_tail = minimum_node -> child;
                Node* child_temp_head = child_temp_tail -> right;
                do
                {
                    child_temp_head -> parent = nullptr;
                    child_temp_head = child_temp_head -> right;
                } while (child_temp_head != child_temp_tail -> left);
                prev -> right = child_temp_tail;
                child_temp_tail -> left = prev;
                child_temp_head -> right = next;
                next -> left = child_temp_head;

                minimum_node -> child = nullptr;
                prev = child_temp_head;
            }

            int size = log2 (total_node) + 1;
            vector < Node* > degree_count (size, nullptr);
            Node* current = minimum_node = next;
            Node* next_from_previous = nullptr;
            do 
            {
                next_from_previous = prev -> right;
                Node* upcoming = current -> right;
                while (degree_count[current -> degree] != nullptr)
                {
                    Node* temp = degree_count[current -> degree];
                    degree_count[current -> degree] = nullptr;
                    current = merge_tree (current, temp);
                }
                if (current -> value < minimum_node -> value) minimum_node = current;
                degree_count[current -> degree] = current;
                current = upcoming;
            } while (current != next_from_previous);
        }

        pair < int, int > min_heap ()
        {
            if (minimum_node == nullptr) throw runtime_error ("Heap doesn't exist !!\n");
            pair < int, int > pr = make_pair (minimum_node -> value, minimum_node -> number);
            return pr;
        }

        bool empty ()
        {
            if (minimum_node == nullptr) return true;
            return false;
        }

        void print_heap_recursively (Node* root)
        {
            if (root -> child == nullptr) return;
            Node* children = root -> child;
            do
            {
                cout << children -> value << " ";
                print_heap_recursively (children);
                children = children -> right;
            } while (children != root -> child);
        }

        Node* extract_pointer (int number_node)
        {
            if (save_pointer[number_node] == nullptr) throw runtime_error ("WRONG ~~");
            return save_pointer[number_node];
        }

        void output ()
        {
            if (minimum_node == nullptr)
            {
                cout << "Heap is empty\n";
                return;
            }
            Node* temp = minimum_node;
            do
            {
                cout << temp -> value << " ";
                print_heap_recursively (temp);
                temp = temp -> right;
            } while (temp != minimum_node);
            cout << endl;
        }
};

int n, m, departure, arrival;
vector < int > weight;
vector < int > previous;
vector < vector < pii >> graph;
Customize_Data_Structure pq (limit); // Represent 'priority_queue'

void init_ ()
{
    previous[departure] = departure;
    for (int i = 1; i <= n; ++i)
    {
        if (i != departure)
        {
            weight[i] = INT_MAX;
            pq.insert (INT_MAX, i);
        } 
    }
    weight[departure] = 0;
    pq.insert (0, departure);
}

void Dijkstra ()
{
    init_ ();

    while (!pq.empty ())
    {
        auto pr = pq.min_heap (); 
        pq.delete_min ();
        for (auto pt : graph[pr.second])
        {
            if (previous[pt.second]) continue;

            int new_weight = weight[pr.second] + pt.first;
            int curr_weght = weight[pt.second];

            if (new_weight < curr_weght)
            {
                pq.decrease_key (pq.extract_pointer (pt.second), new_weight);
                weight[pt.second] = new_weight;
                previous[pt.second] = pr.second;
            }
        }
    }
}

int main ()
{
    // Input 
    cin >> n >> m;
    graph.assign (n + 1, vector < pii > ()); // cretae matrix includes empty vector(s) every row
    previous.assign (n + 1, 0);
    weight.assign (n + 1, 0);
    for (int i = 0; i < m; ++i)
    {
        int x, y, z; cin >> x >> y >> z;
        graph[x].push_back ({z, y});
        graph[y].push_back ({z, x});
    }
    cin >> departure >> arrival;
    Dijkstra (); // Implement algorithm

    // Find path
    int k = arrival;
    vector < int > v;
    while (arrival != departure)
    {
        v.push_back (arrival);
        arrival = previous[arrival];
    }
    v.push_back (arrival);

    // Final output
    cout << "Weight : " << weight[k] << endl;
    for (int x : v) cout << x << " ";


    return 0;
}