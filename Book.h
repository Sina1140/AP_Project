#include <string>
#include <fstream>
#include <vector>
using namespace std;

class User{
friend class Book;
private:
    string username, password, f_name, l_name, birthdate;
    vector<string> listofbooks;
    bool loged_in = false;
public:
    User(){}
    User(string user, string pass, string fn, string ln, string birth):
                    username(user), password(pass), f_name(fn), l_name(ln), birthdate(birth){}
    
    string getname(){return username;}

    void newbooklist() // empties list of Users books
    {
        vector<string> newlist;
        listofbooks = newlist;
    }

    vector<string> booklist() //get list of users books
    {
        return listofbooks;
    }

    friend ifstream &operator>>( ifstream& input, User& obj) 
    { 
        string bk1, bk2;
        input >> obj.username >> obj.password >> obj.f_name >> obj.l_name
                >> obj.birthdate >> bk1 >> bk2;
        if (bk1 != "Null") obj.listofbooks.push_back(bk1);
        if (bk2 != "Null") obj.listofbooks.push_back(bk2);
        return input;          
    }

    friend ofstream &operator<<( ofstream& output, const User& obj) 
    { 
        string bk1, bk2;
        switch (obj.listofbooks.size())
        { 
        case 0:
            bk1 = "Null";
            bk2 = "Null";
            break;
        case 1:
            bk1 = obj.listofbooks[0];
            bk2 = "Null";
            break;
        case 2:
            bk1 = obj.listofbooks[0];
            bk2 = obj.listofbooks[1];
            break;            
        }
        output << obj.username << ' ' << obj.password << ' ' << obj.f_name << ' ' << obj.l_name
                << ' ' << obj.birthdate << ' ' << bk1 << ' ' << bk2 << endl;
        return output;          
    }

    bool is_loged_in(string uname, string pass){
        if (uname == username && pass == password){
            loged_in = true;
            return true;
        }
        else{
            return false;
        }
    }  


      
};

class Book{
    friend class User;
    private:
    string status[2], Title = "", Shelf_Num, Publisher, Pub_year, Subjects, authors, Edition, ISBN, length;
    
    public:
    Book(){}

    string get_title()
    {
        return Title;
    }

    bool isfree() //is this book available?
    {
        if (status[0] == "free") return true;
        else return false;
    }

    bool isTakenbyUser(User user) //Has the User taken this book?
    {
        if (status[1] == user.username){
            return true;
        } 
        return false;
    }

    bool match(string title = "Null", string shelf_num = "Null",string publisher = "Null",
    string pub_year = "Null", string subj = "Null",string author = "Null", string isbn ="Null")
    {
        
        if(title == Title) return true;
        else if (author == authors) return true;
        else if (subj == Subjects) return true;
        else if (publisher == Publisher) return true;
        else if (pub_year == Pub_year) return true;
        else if (isbn == ISBN) return true;
        else if (shelf_num == Shelf_Num) return true;
        else return false;
    }

    void getbook(User &person) //gives this book to the user
    {
        if (person.loged_in){
            status[0] = "taken";
            status[1] = person.username;
            person.listofbooks.push_back(Title);
        }
    }

    void returnbook(User &person) //takes back the book from the user
    {
        if (person.loged_in){
            status[0] = "free";
            status[1] = "Null";
            vector<string> temp = person.listofbooks;
            for (int i = 0; i < person.listofbooks.size(); i++){
                if (person.listofbooks.at(i) == Title) temp.erase(temp.begin() + i);
            }
            person.listofbooks = temp;
        }
    }

    friend ifstream &operator>>( ifstream& input, Book& obj) { 
        input >> obj.Title >> obj.Shelf_Num >> obj.authors >> obj.Edition
                >> obj.Publisher >> obj.Pub_year >> obj.ISBN >> obj.length >> obj.Subjects >> obj.status[0] >> obj.status[1];
        return input;          
    }

    friend ofstream &operator<<( ofstream& output, const Book& obj) { 
        output << obj.Title << ' ' << obj.Shelf_Num << ' ' << obj.authors << ' ' << obj.Edition
                << ' ' << obj.Publisher << ' ' << obj.Pub_year << ' ' << obj.ISBN << ' ' << obj.length 
                << ' ' << obj.Subjects << ' ' << obj.status[0] << ' ' << obj.status[1]<< endl;
        return output;          
    }

};