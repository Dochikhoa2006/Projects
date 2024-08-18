#include <bits/stdc++.h>

using namespace std; 
const int NOW = 2024;

struct Student
{
    string Term_I, Term_II;
    string GPA;
    string Date_of_Birth, Name;
    string Code_Student, Gmail;

    Student (): Term_I("N/A"), Term_II("N/A"), GPA("N/A"), Date_of_Birth("DD/MM/YYYY"), Name("N/A"), Code_Student("N/A"), Gmail("anonymous@Uni.com.vn") {};
};

struct Sort_By_Alphabet // if (false)
{
    bool operator () (const Student &st1, const Student &st2) const
    {
        string a = st1.Name;
        string b = st2.Name;
        return a < b;
    }
};

struct Sort_By_GPA // if (true)
{
    bool operator () (const Student &st1, const Student &st2) const
    {
        auto a = stod (st1.GPA);
        auto b = stod (st2.GPA);
        return a < b;
    }
};

class Node
{
    public :
        Student information;
        Node* next_ptr; 
        Node* prev_ptr;

        Student Update (Node* &head);
        Student GPA_Calculate (double term_1, double term_2);
        string Name_Check (string name);
        string Date_of_Birth_Check (string date_of_birth);
        string Automatical_Code_Creator (Node* &head);
        string Automatical_Gmail_Creator (string name, string code);

    public :
        int size (Node* &head);
        bool empty (Node* &head);
        void clear (Node* &head);
        int find_position (Node*, Node* &head);
        
        Node* New_Student (Node* &head);
        void insert_front (Node* &head);
        void insert_middle (int position, Node* &head);
        void insert_back (Node* &head);

        void delete_front (Node* &head);
        void delete_middle (int position, Node* &head);
        void delete_back (Node* &head);

        Student& access_front (Node* &head);
        Student& access_middle (int position, Node* &head); // need to type the input in main scope
        Student& access_back (Node* &head);

        void partition (int left, int middle, int right, bool order, Node* &head); // go together with Sort ()
        void Sort (bool order, Node* &head);

        void reverse (Node* &head);

        void output (Node* &head);
};

class Class
{
    public :
        string Class_Name;
        Node* head;

        Class (string str): Class_Name(str), head(nullptr) {}; // Initialize the 'head' Node or we can use 'this' pointer as the father pointer
};

// Count the number of Node(s)
int Node :: size (Node* &head) 
{
    Node* temp = head;
    int cnt = 0;
    while (temp != nullptr)
    {
        ++cnt;
        temp = temp -> next_ptr;
    }
    return cnt;
}

// Check the list empty or not ?
bool Node :: empty (Node* &head)
{
    return head == nullptr;
}

// Erase the 'head' Node => Clear the list
void Node :: clear (Node* &head)
{
    head -> next_ptr = nullptr;
}

// Find position
int Node :: find_position (Node* temp, Node* &head)
{
    if (temp == nullptr) return -1;
    int position = 1;
    Node* tmp = head;
    while (tmp != temp)
    {
        ++position;
        tmp = tmp -> next_ptr;
    }
    return position;
}

string Node :: Automatical_Gmail_Creator (string name, string code)
{
    string s;
    for (const char &ch : code)
    {
        if (isdigit (ch) && ch - '0') s += ch;
    }

    stringstream ss (name);
    string str, temp_1, temp_2;
    ss >> temp_1; 
    while (ss >> temp_2);

    str += temp_2 + '.' + temp_1 + s + "@BKUni.hcm.vn";
    return str;
}

string Node :: Automatical_Code_Creator (Node* &head)
{
    string str = "BKU-";
    string SIZE = to_string (size (head) + 1);
    for (int i = 1; i <= 5 - SIZE.size (); ++i)
    {
        str += '0';
    }
    str += SIZE;

    return str;
}

string Node :: Date_of_Birth_Check (string date_of_birth)
{
    string str = "N/A";
    if (date_of_birth.length () < 6) return str;
    if (isdigit (date_of_birth[2])) date_of_birth = '0' + date_of_birth;
    if (isdigit (date_of_birth[5])) date_of_birth.insert (3, 1, '0');

    return date_of_birth;
}

string Node :: Name_Check (string name)
{
    stringstream ss (name);
    string str, x;
    while (ss >> x)
    {
        for (char &ch : x) ch = tolower (ch);
        if (x.length ()) x[0] = toupper (x[0]);
        str += x + " ";
    }

    return str;
}

Student Node :: GPA_Calculate (double term_1, double term_2)
{
    Student stu_information;

    double average = (term_1 + term_2) / 2;
    stringstream ss;
    ss << fixed << setprecision (2) << term_1;
    stu_information.Term_I = ss.str ();
    ss << fixed << setprecision (2) << term_2;
    stu_information.Term_II = ss.str ();
    ss << fixed << setprecision (2) << average;
    stu_information.GPA = ss.str ();

    return stu_information;
}

Student Node :: Update (Node* &head)
{
    Student stu_information;

    double term_1, term_2;
    string name, date_of_birth;

    cout << "- - - - - - - - - - INTRUCTIONS - - - - - - - - - - - - - - \n";
    cout << "Term I grade: "; cin >> term_1;
    cout << "Term II grade: "; cin >> term_2; cin.ignore ();
    cout << "Name: "; getline (cin, name);
    cout << "Date of Birth: "; getline (cin, date_of_birth);
    cout << "Data has been updated !!\n";

    Student Grade = GPA_Calculate (term_1, term_2);
    name = Name_Check (name);
    date_of_birth = Date_of_Birth_Check (date_of_birth);
    string code_temp = Automatical_Code_Creator (head);
    string gmail = Automatical_Gmail_Creator (name, code_temp);

    stu_information.Term_I = Grade.Term_I;
    stu_information.Term_II = Grade.Term_II;
    stu_information.GPA = Grade.GPA;
    stu_information.Name = name;
    stu_information.Date_of_Birth = date_of_birth;
    stu_information.Code_Student = code_temp;
    stu_information.Gmail = gmail;

    return stu_information;
}

// Create a pointer to new Node
Node* Node :: New_Student (Node* &head)
{
    Student stu_information = Update (head);

    Node* new_Node = new Node ();
    new_Node -> information = stu_information;
    new_Node -> next_ptr = nullptr;
    new_Node -> prev_ptr = nullptr;

    return new_Node;
}

// Insert front
void Node :: insert_front (Node* &head)
{
    Node* temp = New_Student (head);
    if (head == nullptr)
    {
        head = temp;
        return;
    }
    head -> prev_ptr = temp;
    temp -> next_ptr = head;
    head = temp;
}

// Insert back
void Node :: insert_back (Node* &head)
{
    if (head == nullptr) 
    {
        insert_front (head);
        return;
    }
    Node* temp_1 = New_Student (head);
    Node* temp_2 = head;
    while (temp_2 -> next_ptr != nullptr)
    {
        temp_2 = temp_2 -> next_ptr;
    }
    temp_2 -> next_ptr = temp_1;
    temp_1 -> prev_ptr = temp_2;
}

// Insert middle
void Node :: insert_middle (int position, Node* &head)
{
    int n = size (head);
    if (position <= 0 || position > n + 1) return;
    if (position == 1) 
    {
        insert_front (head);
        return;
    }
    else if (position == n + 1)
    {
        insert_back (head);
        return;
    }
    else 
    {
        Node* temp_1 = New_Student (head);
        Node* temp_2 = head;
        for (int i = 1; i < position - 1; ++i)
        {
            temp_2 = temp_2 -> next_ptr;
        }
        temp_1 -> next_ptr = temp_2 -> next_ptr;
        temp_1 -> prev_ptr = temp_2;
        temp_2 -> next_ptr = temp_1;
        (temp_1 -> next_ptr) -> prev_ptr = temp_1;
    }
}

// Delete front
void Node :: delete_front (Node* &head)
{
    if (head == nullptr) return;
    head = head -> next_ptr;
    head -> prev_ptr = nullptr; 
}

// Delete back
void Node :: delete_back (Node* &head)
{   
    if (head == nullptr) return;
    if (head -> next_ptr == nullptr)
    {
        delete_front (head);
        return;
    } 

    Node* temp_1 = head;
    while (temp_1 -> next_ptr != nullptr)
    {
        temp_1 = temp_1 -> next_ptr;
    }
    temp_1 = temp_1 -> prev_ptr;
    temp_1 -> next_ptr = nullptr;
}

// Delete middle
void Node :: delete_middle (int position, Node* &head)
{
    int n = size (head);
    if (position <= 0 || position > n) return;
    if (position == 1)
    {
        delete_front (head);
        return;
    }
    else if (position == n)
    {
        delete_back (head);
        return;
    }
    else
    {
        Node* temp = head;
        for (int i = 1 ; i < position - 1; ++i)
        {
            temp = temp -> next_ptr;
        }
        temp -> next_ptr = (temp -> next_ptr) -> next_ptr;
        (temp -> next_ptr) -> prev_ptr = temp;
    }
}

// Accessing front 
Student& Node :: access_front (Node* &head)
{
    if (head == nullptr)
    {
        Student *s = new Student ();
        return *s;
    } 
    return head -> information;
}

// Accessing back
Student& Node :: access_back (Node* &head)
{
    if (head == nullptr)
    {
        Student *s = new Student ();
        return *s;
    } 
    Node* temp = head;
    while (temp -> next_ptr != nullptr)
    {
        temp = temp -> next_ptr;
    }
    return temp -> information;
}

// Accessing middle
Student& Node :: access_middle (int position, Node* &head)
{
    int n = size (head);
    if (position <= 0 || position > n)
    {
        Student *s = new Student ();
        return *s;
    } 
    Node* temp = head;
    for (int i = 1; i < position; ++i)
    {
        temp = temp -> next_ptr;
    }
    return temp -> information;
}

// Bottom - up merge sort
void Node :: partition (int left, int middle, int right, bool order, Node* &head)
{
    int left_size = middle - left + 1;
    int right_size = right - middle;
    int k = left + 1;

    vector < Student > L (left_size);
    vector < Student > R (right_size);   

    for (int i = 0; i < left_size; ++i)
    {
        Student stu = access_middle (k, head);
        L[i] = stu;
        ++k;
    }
    for (int j = 0; j < right_size; ++j)
    {
        Student stu = access_middle (k, head);
        R[j] = stu;
        ++k;
    }

    int i = 0, j = 0;
    k = left + 1;
    while (i < left_size && j < right_size)
    {
        if (order)
        {
            Sort_By_GPA cmp;
            if (cmp (L[i], R[j]))
            {
                access_middle (k, head) = L[i];
                ++k; ++i;
            }
            else 
            {
                access_middle (k, head) = R[j];
                ++k; ++j;
            }
        }
        else 
        {
            Sort_By_Alphabet cmp;
            if (cmp (L[i], R[j]))
            {
                access_middle (k, head) = L[i];
                ++k; ++i;
            }
            else 
            {
                access_middle (k, head) = R[j];
                ++k; ++j;
            }
        }
    }
    while (i < left_size)
    {
        access_middle (k, head) = L[i];
        ++k; ++i;
    }
    while (j < right_size)
    {
        access_middle (k, head) = R[j];
        ++k; ++j;
    }
}

// Sort information
void Node :: Sort (bool order, Node* &head)
{
    if (head == nullptr || head -> next_ptr == nullptr) return;
    int n = size (head);
    for (int step = 1; step < n; step *= 2)
    {
        for (int left = 0; left < n; left += 2 * step)
        {
            int middle = min (left + step - 1, n - 1);
            int right = min (left + 2 * step - 1, n - 1);
            partition (left, middle, right, order, head);
        }
    }
}

// Reverse
void Node :: reverse (Node* &head)
{
    Node* previous = nullptr;
    Node* current = head;
    while (head != nullptr)
    {
        head = head -> next_ptr;
        current -> next_ptr = previous;
        current -> prev_ptr = head;
        current = head;
        previous = current;
    }
    head = previous;
}

// Output
void Node :: output (Node* &head)
{
    Node* temp = head;
    int k = 1;
    while (temp != nullptr)
    {
        Student* stu = &(temp -> information);
        cout << "\n_________________________________________________\n";
        cout << "+ Student " << k << " :\n";
        cout << "Name: " << stu -> Name << endl;
        cout << "Term I: " << stu -> Term_I << endl;
        cout << "Term II: " << stu -> Term_II << endl;
        cout << "GPA score: " << stu -> GPA << endl;
        cout << "Student code: " << stu -> Code_Student << endl;
        cout << "Gmail: " << stu -> Gmail << endl;
        temp = temp -> next_ptr; 
        ++k;
    }
    cout << endl;
}

string Fixing_Node_Name (string a)
{
    stringstream ss (a);
    string str, x;
    while (ss >> x)
    {
        for (char &ch : x) ch = tolower (ch);
        if (x.length ()) x[0] = toupper (x[0]);
        str += x + " ";
    }

    return str;
}

string Fixing_Date_Of_Birth (string date_of_birth)
{
    string str = "N/A";
    if (date_of_birth.length () < 6) return str;
    if (isdigit (date_of_birth[2])) date_of_birth = '0' + date_of_birth;
    if (isdigit (date_of_birth[5])) date_of_birth.insert (3, 1, '0');

    return date_of_birth;
}

string Fixing_GPA (double term_1, double term_2)
{
    double average = (term_1 + term_2) / 2;
    stringstream ss;
    ss << fixed << setprecision (2) << average;
    string str = ss.str ();

    return str;
}

string Automatically_Gmail_Change (string name, string code)
{
    string s;
    for (const char &ch : code)
    {
        if (isdigit (ch) && ch - '0') s += ch;
    }

    stringstream ss (name);
    string str, temp_1, temp_2;
    ss >> temp_1; 
    while (ss >> temp_2);

    str += temp_2 + '.' + temp_1 + s + "@BKUni.hcm.vn";
    return str;
}

int main ()
{
    vector < Class* > school;

    int value = 1;
    while (value)
    {
    //                              SHOWING MENU
        cout << "- - - - - - - - - MENU - - - - - - - - -\n";
        cout << "1: Create New Class Document\n";
        cout << "2: Fix Class Name\n";
        cout << "3: Delete Class Document\n";
        cout << "4: Add New Student(s)\n";
        cout << "5: Fix Student Information\n";
        cout << "6: Delete Student Information\n";
        cout << "7: View Profile(s)\n";
        cout << "8: Ranking Top GPA\n";
        cout << "9: Exit the system\n";
        cout << "Enter a number to choose an option : ";

        cin >> value;


        //      value == 1   ===>   CREATE NEW Node DOCUMENT
        if (value == 1)
        {
            string str; 
            cout << "-----------------------------------------------------------------\n";
            cout << "Type the Node name: ";
            cin.ignore (); 
            getline (cin, str);
            str = Fixing_Node_Name (str);

            Class* cl = new Class (str);
            school.push_back (cl);
        }
        //      value == 2    ===>   FIXING Node NAME
        else if (value == 2)
        {
            if (school.empty ()) 
            {
                cout << "--->>> There is not any class to fix. Sorry !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the original class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else 
                {
                    string s;
                    cout << "What's name you want to change: ";
                    getline (cin, s);
                    s = Fixing_Node_Name (s);
                    (*cls) -> Class_Name = s;

                    cout << "Fixed Successfully ~~\n";
                    k = 0;
                }
            }
        }
        //      value == 3    ===>   DELETING Node NAME
        else if (value == 3)
        {
            if (school.empty ()) 
            {
                cout << "\n---> There is not any class to delete. Sorry !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else 
                {
                    school.erase (cls);
                    cout << "Deleted Successfully ~~\n";
                    k = 0;
                }
            }
        }
        //      value == 4    ===>   ADD NEW STUDENT
        else if (value == 4)
        {
            if (school.empty ())
            {
                cout << "\n---> You may have to create a new Node folder first. Thank you !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else 
                {
                    cout << "How many students needed to be added: ";
                    int a; cin >> a;
                    while (a--)
                    {
                        (*cls) -> head -> insert_back ((*cls) -> head);
                        cout << "Added Successfully ~~\n";
                    }
                    k = 0;
                }
            }
        }
        //      value == 5    ===>   FIX STUDENT INFORMATION
        else if (value == 5)
        {
            if (school.empty ())
            {
                cout << "\n---> You may have to create a new Node folder first. Thank you !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else 
                {
                    int j = 1;
                    while (j)
                    {
                        cout << "--> If you don't know the student name, please type 'N/A' or choose 'Add New Student' section\n";
                        cout << "Type the name : ";
                        string str; getline (cin, str);
                        str = Fixing_Node_Name (str);

                        bool findd = false;
                        Node* temp = (*cls) -> head;
                        do
                        {
                            if ((temp -> information).Name == str)
                            {
                                found = true;
                                break;
                            }
                            temp = temp -> next_ptr;
                        } while (temp != ((*cls) -> head));

                        if (!findd)
                        {
                            cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                            cout << "1: YES\n";
                            cout << "2: NO\n";
                            cout << "Enter a number: ";

                            int a; cin >> a;
                            if (a == 1) j = 1;
                            else j = 0;
                        }
                        else
                        {
                            int p = 1;
                            while (p)
                            {
                                cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ LIST ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ \n";
                                cout << "1: Change Name\n";
                                cout << "2: Change Date Of Birth\n";
                                cout << "3: Change Term I Score\n";
                                cout << "4: Change Term II Score\n";
                                cout << "5: Exit\n";
                                cout << "Enter a number: ";

                                int a; cin >> a; 
                                if (a == 1)
                                {
                                    cin.ignore ();
                                    string str; getline (cin, str);
                                    str = Fixing_Node_Name (str);
                                    string s = Automatically_Gmail_Change (str, (temp -> information).Code_Student);
                                    
                                    (temp -> information).Name = str;
                                    (temp -> information).Code_Student = s;
                                }
                                else if (a == 2)
                                {
                                    cin.ignore ();
                                    string str; getline (cin, str);
                                    str = Fixing_Date_Of_Birth (str);
                                    (temp -> information).Date_of_Birth = str;
                                }
                                else if (a == 3)
                                {
                                    double a; cin >> a;
                                    stringstream ss;
                                    ss << fixed << setprecision (2) << a;
                                    string s = ss.str ();

                                    double tmp_1 = stod (s);
                                    double tmp_2 = stod ((temp -> information).Term_II);
                                    string str = Fixing_GPA (tmp_1, tmp_2);
                                    (temp -> information).Term_I = s;
                                    (temp -> information).GPA = str;
                                }
                                else if (a == 4)
                                {
                                    double a; cin >> a;
                                    stringstream ss;
                                    ss << fixed << setprecision (2) << a;
                                    string s = ss.str ();

                                    double tmp_1 = stod ((temp -> information).Term_I);
                                    double tmp_2 = stod (s);
                                    string str = Fixing_GPA (tmp_1, tmp_2);
                                    (temp -> information).Term_II = s;
                                    (temp -> information).GPA = str;
                                }
                                else break;
                                p = 1;
                            }
                            cout << "Fixed Successfully ~~\n";
                            j = 0;
                        }
                    }
                    k = 0;
                }
            }
        }
        else if (value == 6)
        {
            if (school.empty ()) 
            {
                cout << "\n---> There is not any class to sort. Sorry !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else 
                {
                    cout << "--> If you don't know the student name, please type 'N/A' or choose 'Add New Student' section\n";
                    cout << "Type the name : ";
                    string str; getline (cin, str);
                    str = Fixing_Node_Name (str);

                    bool findd = false;
                    Node* temp = (*cls) -> head;
                    do
                    {
                        if ((temp -> information).Name == str)
                        {
                            found = true;
                            break;
                        }
                        temp = temp -> next_ptr;
                    } while (temp != ((*cls) -> head));

                    if (!findd)
                    {
                        cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                        cout << "1: YES\n";
                        cout << "2: NO\n";
                        cout << "Enter a number: ";

                        int a; cin >> a;
                        if (a == 1) k = 1;
                        else k = 0;
                    }
                    else
                    {
                        int position_ = (*cls) -> head -> find_position (temp, (*cls) -> head);
                        (*cls) -> head -> delete_middle (position_, (*cls) -> head);
                        k = 0;
                    }
                }
            }
        }
        else if (value == 7)
        {
            if (school.empty ()) 
            {
                cout << "\n---> There is not any class to sort. Sorry !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else
                {
                    (*cls) -> head -> output ((*cls) -> head);
                    k = 0;
                }
            }
        }
        else if (value == 8)
        {
            if (school.empty ()) 
            {
                cout << "\n---> There is not any class to sort. Sorry !!\n";
                continue;
            }

            int k = 1;
            while (k)
            {
                string str; 
                cout << "------------------------------------------------------------------\n";
                cout << "Type the class name: ";
                cin.ignore (); 
                getline (cin, str);
                str = Fixing_Node_Name (str);

                bool found = false;
                auto cls = school.begin ();
                for (; cls != school.end (); ++cls)
                {
                    if ((*cls) -> Class_Name == str)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Perhaps you have typed the wrong name, do you want another try ?\n";
                    cout << "1: YES\n";
                    cout << "2: NO\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1) k = 1;
                    else k = 0;
                }
                else 
                {
                    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ SORT TYPE ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n";
                    cout << "1: Sort By Name\n";
                    cout << "2: Sort By GPA\n";
                    cout << "Enter a number: ";

                    int a; cin >> a;
                    if (a == 1)
                    {
                       (*cls) -> head -> Sort (false, (*cls) -> head);
                       cout << "Sorted Successfully ~~\n";
                    }
                    else if (a == 2)
                    {
                        (*cls) -> head -> Sort (true, (*cls) -> head);
                        cout << "Sorted Successfully ~~\n";
                    }  
                    else cout << "Sorted Unsuccessfully ~~\n";
                    k = 0;
                }
            }
        }
        else
        {
            cout << "-----> Finish the program <-----";
            break;
        }

        cout << endl;
    }

    return 0;              
}

















































