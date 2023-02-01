#include "Book.h"
#include <vector>
#include <iostream>
using namespace std;

void SignUp(){
  string use, pas, fn, ln, birth;
  ofstream userlog ("Users.txt", ios::app);
  cout << "Username:"; getline(cin, use);
  cout << "Password:"; getline(cin, pas);
  cout << "First name:"; getline(cin, fn);
  cout << "Last name:"; getline(cin, ln);
  cout << "Birthyear:"; getline(cin, birth);
  User new_user(use, pas, fn, ln, birth);
  new_user.newbooklist();
  userlog << new_user;
}

vector<Book> ListLibrary(string filename){
  vector<Book> library;
  Book temp;
  ifstream myfile (filename);
  if (myfile.is_open())
  {
    while (myfile >> temp)
    {
      library.push_back(temp);
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 
  return library;
}

vector<User> ListUsers(string filename){
  vector<User> users;
  User temp;
  ifstream myfile (filename);
  if (myfile.is_open())
  {
    while (myfile >> temp)
    {
      users.push_back(temp);
      temp.newbooklist();
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 
  return users;
}

User LogIn(string name, string pass, vector<User> *users){
  for (int i = 0; i < users -> size(); i++){
    if ((users -> at(i)).is_loged_in(name, pass)){
      return users -> at(i);
    }
  }
  cout << "Username or Password was Wrong!!!" << endl;
  User none;
  return none;
}

Book search(vector<Book> *library){
  vector<Book> result;
  string title, author, subj, publisher, pub_year, isbn, shelf_num;
  int choice;
  //cout << "Search________________"<< endl;
  cout << "Title: "; getline(cin, title); 
  cout << "Author: "; getline(cin, author); 
  cout << "Genre: "; getline(cin, subj); 
  cout << "Publisher: "; getline(cin, publisher); 
  cout << "Published Year: "; getline(cin, pub_year); 
  cout << "ISBN: "; getline(cin, isbn); 
  cout << "Shelf No.: "; getline(cin, shelf_num);

  for (int i = 0; i < library -> size(); i++){
    if (library -> at(i).match(title, shelf_num, publisher, pub_year, subj, author, isbn)){
      result.push_back(library -> at(i));
    }
  }
  if (result.size() == 0){
    cout << "Couldn't Find any Books!!!"<< endl;
    Book none;
    return none;
    }
  for (int i = 0; i < result.size(); i++){
    cout << i+1 << "." << ' ' << result[i].get_title() << endl;
  }
  cout << "Choose Your Book: "; cin >> choice;
  return result[choice-1];
}

void UpdateLibrary(vector<Book> *library, Book chosen){
  ofstream lib;
  lib.open("books.txt", ofstream::out | ofstream::trunc);
  for (int i = 0; i < library -> size(); i++){
    if (chosen.get_title() == (library -> at(i).get_title())){
      library ->at(i) = chosen;
    }
    lib << (library -> at(i));
  }
  lib.close();
}

void UpdateUsers(vector<User> *users, User curr){
  ofstream us;
  us.open("Users.txt", ofstream::out | ofstream::trunc);
  for (int i = 0; i < users -> size(); i++){
    if (curr.getname() == (users -> at(i).getname())){
      users ->at(i) = curr;
    }
    us << (users -> at(i));
  }
}

int main () 
{
  

  /*ofstream myfile ("example.txt");
  if (myfile.is_open())
  {
    myfile << "This is a line.\n";
    myfile << "This is another line.\n";
    myfile.close();
  }
  else cout << "Unable to open file";
  return 0;*/

   
  vector<Book> library = ListLibrary("books.txt");
  vector<User> users = ListUsers("Users.txt");
  string name, pass, choice = "";
  // Log-in Or Sign-up:
  lable1:
  while(true){
    cout << "1. Log in \n2. Sign up \n>> "; getline(cin >> ws, choice);
    if (choice == "1"){
      cout << "Username: "; getline(cin,name);
      cout << "Password: "; getline(cin,pass);
      break;
    }
    else{
      SignUp();
    }
  }

  User curr_user = LogIn(name, pass, &users);// Loging into Users Account
  while(true){
    if (curr_user.is_loged_in(name, pass)){
      Book chosen_book = search(&library); // search through the library
      string choice = "";
      if (chosen_book.get_title()!= ""){
        // if the book is available:
        if (chosen_book.isfree()){
          cout << "Get a Book? [y/n]> "; cin >> choice;
          if (choice == "y" && curr_user.booklist().size() < 2){
            chosen_book.getbook(curr_user);
            cout << "Enjoy!!!";
          }
          else cout << "You Can't Take anymore books!" << endl;
        }
        // if the book is taken by someone else:
        else if (!chosen_book.isTakenbyUser(curr_user)){
          cout << "The book is currently unavailable"<< endl;
        }
        //if the book has been taken by the current user:
        else if (chosen_book.isTakenbyUser(curr_user)){
          cout << "Return the Book? [y/n]> "; cin >> choice;
          if (choice == "y"){
            chosen_book.returnbook(curr_user);
            cout << "Thank you. The Book has Returned to the Library." << endl;
          }
        }  
      }
      UpdateLibrary(&library, chosen_book); // Update Book log
      UpdateUsers(&users, curr_user); // Update User log
    }
    else {goto lable1;}
    
    cout << "Do you have any other request?[y/n]>"; getline(cin >> ws, choice);
    if (choice == "y"){continue;}
    break;
  }
  return 0;
}