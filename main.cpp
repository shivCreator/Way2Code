#include<iostream>
#include<fstream>
#include<string.h>
#include<stdio.h>
#include<iomanip>
#include<stdlib.h>
#include<conio.h>

#define horizontal_line cout<<setw(50)<<"============================================\n"
#define COUT cout<<"\t"
#define wordlen 20
#define meanlen 100
#define uselen 100
#define c_1 ")add word"
#define c_0 ")exit"
#define c_2 ")search"
#define dictFile "wm.txt"

using namespace std;

void command(char c[20]);

struct Node{
    char word[wordlen];
    Node* next;
};

class UtilityFile{
    Node* top;
    const char* file;
    unsigned int Size;
    
public:
    UtilityFile(const char *filename):top(nullptr), file(filename), Size(0)
    {
        ifstream dataIn(file);
        if(!dataIn){
            COUT << "Could not load UtilityFile" << endl;
            return ;
        }
        char word[wordlen];
        while(!dataIn.eof())
        {
            dataIn.getline(word, wordlen-1, '\n');
            push(word);
        }
        dataIn.close();
//        COUT << "UtilityFile loaded successfully" << endl;
//        getch();
    }
    ~UtilityFile()
    {
        ofstream dataOut(file);
        if(!dataOut){
            COUT << "Could not save history" << endl;
            return ;
        }
        for(int i = Size; i > 0; i--){
            char data[wordlen];
            pop(data);
            dataOut << data << '\n';
        }
        dataOut.close();
//        COUT << "UtilityFile saved successfully" << endl;
//        getch();
    }
    
    Node* createNode(char* word){
        Node* ptr = new Node();
        strcpy(ptr->word, word);
        ptr->next=nullptr;
        return ptr;
    }
    
    void push(char* word){
        Size++;
        Node* ptr = top;
//        for(Node* parent=top;ptr!=nullptr;parent=ptr, ptr=ptr->next){
//            if(strcmp(word, ptr->word)==0){
//                if(ptr==top)
//                    pop(word);
//                else{
//                    parent->next=ptr->next;
//                    delete ptr;
//                    ptr=parent;
//                }
//            }
//        }
        if(Size > 20){
            // find last node pr element
            for(ptr=top; ptr->next!=nullptr; ptr=ptr->next)
                ;// empty statement captured by for loop
            
            // out of for loop
            // delete last element
            delete ptr;
        }
        Node* temp = top;
        top = createNode(word);
        top->next = temp;
    }
    
    void pop(char word[wordlen]){
        Size--;
        Node* temp = top;
        top = top->next;
        strcpy(word, temp->word);
        delete temp;
    }
    
    void show(){
        for(Node* ptr = top; ptr != nullptr; ptr=ptr->next)
            COUT << ptr->word << endl;
    }
    void getTop(char* atTop) {
        strcpy(atTop, top->word);
    }
};



ifstream datafileIn(dictFile);   ofstream datafileOut(dictFile, ios::app);
UtilityFile History("history.dicto");
UtilityFile Bookmarks("bookmarks.dicto");



class dict{
    char Word[wordlen];
    char meaning[meanlen];
    char usage[uselen];
public:
    void addWord(const char w[wordlen], const char m[meanlen], const char u[uselen]){
        ///cout<<"addWord called\n";
        if(strcmp(w, "")==0){
            return;
        }
        
        strcpy(Word , w);
        strcpy(meaning , m);
        strcpy(usage , u);
        
        ifstream datafileIn(dictFile);
        dict d;
        if(!datafileIn){COUT<<"addWord:operation failed during checking\n"; return;}
        while(!datafileIn.eof()){
            datafileIn.read((char*)&d, sizeof(dict));
            if(strcmp(w, d.Word)==0){
                COUT<<w<<" already present in the dictionary\n";
                return;
            }
        }
        
        // if not already present, add word to dictionary
        ofstream datafileOut(dictFile, ios::app);
        
        if(!datafileOut){COUT<<"addWord:operation failed during insertion\n"; return;}
        
        ///cout<<"copying into dictionary";
        datafileOut.write((char*)this, sizeof(dict));
        
        datafileOut.close();
    }
    bool Search(const char* w){
        if(strcmp(w, "")==0)
            return false;
        
        ifstream datafileIn(dictFile);
        if(!datafileIn){COUT<<"fin not working\n"; return false;}
        datafileIn.seekg(0,ios::beg);
        dict d;
        while(!datafileIn.eof()){
            datafileIn.read((char*)&d, sizeof(dict));
            if(strcmp(d.Word, w) == 0){
                COUT <<"Meaning: " << d.meaning << '\n';
                COUT <<"Usage: " << d.usage << '\n';
                return true;
            }
        }
        COUT<<"\""<<w<<"\""<<" not found!\n";
        return false;
    }
    char* getWord(){return this->Word;}
    char* getMeaning(){return this->meaning;}
    char* getUsage(){return this->usage;}
};

void initilize_dictinary_from(const char* filename) {
    // input from database
    ifstream file(filename);
    // input from dictionary
    ifstream fin(dictFile);
    // output to dictionary
    ofstream fout(dictFile, ios::app);
    
    if(!file) {
        COUT << "Unable to open data file for initializing";
        return ;
    }
    if(!fout) {
        COUT << "Unable to open dictionary output file";
        return ;
    }
    if(!fin) {
        COUT << "Unable to open dictionary input file";
        return ;
    }
    
    
    while(!file.eof()) {
        dict d; // word from database
        
        file.getline(d.getWord(), wordlen-1, '\n');
        file.getline(d.getMeaning(), meanlen-1, '\n');
        file.getline(d.getUsage(), uselen-1, '\n');
        file.ignore(INT_MAX, '\n');
        
        dict d2;    // word from dictionary
        bool moveNext = false;
        
        fin.seekg(0, ios::beg);
        while(1){
          fin.read((char*)&d2, sizeof(dict));
          // if word from database is already in the dictionary
          if(strcmp(d.getWord(), d2.getWord())==0){
            moveNext = true;
            break;
          }
          
          if(fin.eof())
            break;
        }
        if(moveNext)
            continue;
        
        // write word to dictionary if not already present
        fout.write((char*)&d, sizeof(dict));
    }
    fin.close();
    file.close();
    fout.close();
}

int main(){
    char W[wordlen];    
    
    initilize_dictinary_from("dictDatafile.dicto");
    while(1){
        system("cls");
        cout<<'\n';
        horizontal_line;
        COUT<<setw(15)<<"SIMPLE DICTIONARY\n";
        horizontal_line;
        COUT<<"WORD:\t"; cin.getline(W,wordlen,'\n');
        
        if(strcmp(c_0, W)==0)
        {
            system("cls");
            horizontal_line;
            cout<<'\n';
            COUT<<setw(20)<<"EXIT\n";
            horizontal_line;
            
            COUT << "Do you want to quit(y/n)? ";
            cin >> W[0];
            cout<<'\n';
            horizontal_line;
            if(W[0]=='y'||W[0]=='Y')
                return 0;
            else{
                W[0]=getchar();
                continue;
            }
        }
        command(W);
        horizontal_line;
        cout<<'\n';
        COUT << "Press any key to continue: ";
        getch();
    }
    return 0;
}

void command(char c[20]){
    char w[wordlen], m[meanlen], u[uselen];
    dict d;
    int id=100;
       
    if(strcmp(c, c_1)==0){
        id=1;
    }else if(strcmp(c, ")show history")==0){
        id=2;
    }
    else if(strcmp(c, ")bookmark")==0)
    {
        id=3;
    }
    else if(strcmp(c, ")show bookmarks")==0){
        id=4;
    }
    
    switch(id){
    case 1:
        system("cls");
        horizontal_line;
        COUT<<setw(20)<<"ADD WORD"<<endl;
        horizontal_line;
        cout<<'\n';
        COUT<<"Word: "; cin.getline(w,wordlen,'\n');
        COUT<<"Meaning: "; cin.getline(m,meanlen,'\n');
        COUT<<"Usage: "; cin.getline(u,uselen,'\n');
        d.addWord(w,m,u);
        break;
    case 2:
        system("cls");
        COUT<<setw(20)<<"HISTORY\n";
        horizontal_line;
        History.show();
        break;
    case 3:
        History.getTop(w);
        Bookmarks.push(w);
        break;
    case 4:
        system("cls");
        COUT<<setw(20)<<"BOOKMARK\n";
        horizontal_line;
        Bookmarks.show();
        break;
    default:
        if(d.Search(c))
            History.push(c);
    }
}


