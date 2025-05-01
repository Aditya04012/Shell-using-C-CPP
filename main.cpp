
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include<bits/stdc++.h>
#include <fcntl.h>  
using namespace std;
#define clear() printf("\033[H\033[J");
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bold text
#define BOLD        "\033[1m"
#define BOLDRED     "\033[1;31m"
#define BOLDGREEN   "\033[1;32m"
#define BOLDYELLOW  "\033[1;33m"
#define BOLDBLUE    "\033[1;34m"
#define BOLDMAGENTA "\033[1;35m"
#define BOLDCYAN    "\033[1;36m"
#define BOLDWHITE   "\033[1;37m"



map<string,string>aliases;
vector<string>history;





void init_shell(){
      clear();
    cout<<RED<<"*************************************"<<RESET<<endl;
    cout<<RED<<"*************************************"<<RESET<<endl;
    cout<<endl;
    cout<<CYAN<<"Linux Shell made By Aditya Bhatnagar"<<RESET<<endl;
    cout<<endl;
    
    char* username = getenv("USER");
    if(username==NULL){
        username = getenv("USERNAME"); 
    }
    
    if(username) cout<<CYAN<<"USER is @ :"<<username<<RESET<<endl;
    else cout<<CYAN<<"USER NOT FOUND"<<RESET<<endl;
    
     
     
     
    cout<<endl;
     cout<<RED<<"*************************************"<<RESET<<endl;
    cout<<RED<<"*************************************"<<RESET<<endl;
    sleep(3);
    clear();
}
void disDir2(){
    vector<char>cwd(1024);
    getcwd(cwd.data(),cwd.size());
    cout<<cwd.data();
}




//use to handdle in built commands
bool handle_inBuilt(vector<string>&args){
    string cmd=args[0];
    
    
    
    if(cmd=="clear"){
        cout<<"\033[H\033[J";
    }
    
    
    
    //exit command
    if(cmd=="exit"){
        exit(0);
    }
    //cd command can be created by chdir()>>change directory function
    //wich take parameter---> chdir(path)
    if(cmd=="cd"){
        if(args.size()<2){
            cout<<"cd: missing argument" << endl;
        }else{
            if(chdir(args[1].c_str())!=0){
                perror("cd failed");
            }
        }
        return true;
    }
    
   /* if(cmd=="alias"){
        if(args.size()<2){
            cout<<"alias: missing argument" << endl;
        }else{
           string command=args[1];
           aliases[command]=command;
           cout<<"Alias created: " << command <<endl;
        }
        return true;
    }
    */
    //creating alias of short form
    /*
    1) ["alisa","ls","-l"]
      create tow string key and value
      key=alias value="ls -l"

    2) store this key val pair in map


    3)when u enter key it will search in map(real command) and execute command by-->
     
      string cmd=args[0];
      if(aliases.find(cmd)!=aliases.end()){
          string aliasCommand=aliases[cmd];
          
          stringstream ss2(aliasCommand);
          vector<string>aliasArgs;
          
          while(ss2>>token){
              aliasArgs.push_back(token);
          }
          
          
          for(size_t i=1;i<args.size();i++){
              aliasArgs.push_back(args[i]);
          }
          
          args=aliasArgs;
          cmd=args[0];
      }

    
    */
    if (cmd == "alias") {
    if (args.size() < 2) {
        cout << "alias: missing argument" << endl;
    } else {
        string input;
        for (size_t i = 1; i < args.size(); ++i) {
            if (i > 1) input += " ";
            input += args[i];
        }

        size_t eqPos = input.find('=');

        if (eqPos == string::npos) {
            cout << "alias: invalid format. Use alias name='command'" << endl;
        } else {
            string name = input.substr(0, eqPos);
            string value = input.substr(eqPos + 1);

           
            if (!value.empty() && value.front() == '\'' && value.back() == '\'') {
                value = value.substr(1, value.length() - 2);
            }

            aliases[name] = value;
            cout << "Alias created: " << name << " -> " << value << endl;
        }
    }
    return true;
}

//History

/*
1)created vector<string>history and store input string
and when call history command it will display history
*/
    if(cmd=="history"){
        int i=0;
        cout<<"History :"<<endl;
        for(auto it:history){
            i++; 
            cout<<it<<"            ";
            if(i%4==0){
                cout<<endl;
            }
        }
        cout<<endl;
        return true;
    }
    
    //unset
    /*
    1)unset command use to unset the set variable of command using unsetenv();


    */
    if(cmd=="unset"){
        if(args.size()<2){
             cout<<"unset: missing argument" << endl;
        }else{
            string var=args[1];
            unsetenv(var.c_str());
            cout<<"unset " << var<< endl;
        }
        return true;
    }
    
    //
    
    if(cmd=="export"){
        if(args.size()<2){
             cout<<"set: missing argument" << endl;
        }else{
            string var=args[1];
            size_t pos=var.find("=");
            if(pos!=string::npos){
                string key=var.substr(0,pos);
                string value=var.substr(pos+1);
                
                setenv(key.c_str(),value.c_str(),1);
                cout<<"exported " << key << " = " << value <<endl;
            }else{
                cout<<"export: Invalid syntax"<<endl;
            }
        }
        return true;
    }
    return false;
}


bool handle_pipes(string& input){

    //find partition with |
    //cat file.txt | wc -l
    size_t pos=input.find('|');

    //not found means no pipes return false
     if (pos == string::npos) return false;
     //create left and right command
     //left --->"cat file.txt"  , right--->"wc -l"
      string leftCmd = input.substr(0, pos);
    string rightCmd = input.substr(pos + 1);
      
    //use regular expression to avoid extra space in the commands

    leftCmd = regex_replace(leftCmd, regex("^ +| +$"), "");
    rightCmd = regex_replace(rightCmd, regex("^ +| +$"), "");
    

    //creating commands
    vector<string>leftArgs,rightArgs;
    
    stringstream ss1(leftCmd),ss2(rightCmd);
    
    string token;
    while(ss1>>token)leftArgs.push_back(token);
    while(ss2>>token)rightArgs.push_back(token);
      //leftAgrs['cat','file.txt']
      //rightArgs['wc','-l']
    


      //checking if left command has any alias or not
    string cmd1=leftArgs[0];
    if(aliases.find(cmd1)!=aliases.end()){
        string leftOrignalCmd=aliases[cmd1];
        
        stringstream ss3(leftOrignalCmd);
        vector<string>temp;
        while(ss3>>token)temp.push_back(token);
         temp.insert(temp.end(), leftArgs.begin() + 1, leftArgs.end());
         leftArgs = temp;
          
    }
    //checking right command have alias or not
    string cmd2=rightArgs[0];
    if(aliases.find(cmd2)!=aliases.end()){
        stringstream ss4(aliases[cmd2]);
        vector<string>temp;
        while(ss4>>token)temp.push_back(token);
         temp.insert(temp.end(), rightArgs.begin() + 1, rightArgs.end());
         rightArgs = temp;
        
    }
    
    
    
    //creating pipe of 2 for input and output end
  //  fd[0]: read-end, fd[1]: write-end 
    int fd[2];
    if(pipe(fd)==-1){
        perror("piped failed");
        return true;
    }
    
    
     //creating pid1 which will handle output end(write end)
    

     //child one created 
     //Redirect STDOUT to fd[1], close pipe, exec `ls -l`
    pid_t pid1=fork();
    if(pid1==0){
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        vector<char*>c_args1;
        for(auto& it:leftArgs)c_args1.push_back(const_cast<char*>(it.c_str()));
        c_args1.push_back(NULL);
        execvp(c_args1[0],c_args1.data());
        
        perror("exec left");
        exit(1);
    }

    //child 2 created
    //Redirect STDIN to fd[0], close pipe, exec `wc -l`
    pid_t pid2=fork();


    if(pid2==0){
        dup2(fd[0],STDIN_FILENO);
         close(fd[0]);
        close(fd[1]);
        vector<char*>c_args2;
        for(auto& it:rightArgs)c_args2.push_back(const_cast<char*>(it.c_str()));
        c_args2.push_back(NULL);
        execvp(c_args2[0],c_args2.data());
        
        perror("exec left");
        exit(1);
        
    }
    
    
    //close boyh the pipe and parent will wait for the child
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return true;
    
}




int main()
{
  string input;
   init_shell();
  while(true){
      disDir2();
      cout<<">";
      getline(cin,input);
      
      if(input.empty())continue;
      //handle pipes commands
      if(handle_pipes(input))continue;
      
      stringstream ss(input);
      
      
      vector<string>args;
      string token;
      while(ss>>token){
          args.push_back(token);
      }
      
      history.push_back(input);
      
      //initial-> alias ll="ls -l"
      //for alias command (ll)
      
      
      
      
      
      string cmd=args[0];
      if(aliases.find(cmd)!=aliases.end()){
        //if present then taking orignal command from map
          string aliasCommand=aliases[cmd];
          // ls -l
          stringstream ss2(aliasCommand);
          vector<string>aliasArgs;
          
          while(ss2>>token){
              aliasArgs.push_back(token);
          }
        //  ["ls","-l"]
          
          for(size_t i=1;i<args.size();i++){
              aliasArgs.push_back(args[i]);
          }
          
          args=aliasArgs;
          //args["ls","-l"]
          cmd=args[0];
          //cmd='ls'
      }
      
      
      //handle handle_inBuilt commands
      if(handle_inBuilt(args))continue;
      
      
      //handle redirections
      
      int in_fd=-1,out_fd=-1;
      bool append=false;
      
      vector<string>realArgs;
      
      for(size_t i=0;i<args.size();i++){
          if(args[i]=="<"){
              
              if(i+1<args.size()){
                  in_fd=open(args[i+1].c_str(),O_RDONLY);
                  if(in_fd<0){
                       perror("Failed to open input file");
                     break;
                  }
                  i++;
              }
              
          }else if(args[i]==">"){
              if(i+1<args.size()){
                  out_fd=open(args[i+1].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 0644);
                  if(out_fd<0){
                        perror("Failed to open output file");
                         break;
                  }
                  i++;
              }
          }else if(args[i]==">>"){
              if(i+1<args.size()){
                  out_fd=open(args[i+1].c_str(),O_WRONLY | O_CREAT | O_APPEND, 0644);
                  if(out_fd<0){
                        perror("Failed to open output file");
                         break;
                  }
                  append=true;
                  i++;
              }
          }else{
              realArgs.push_back(args[i]);
          }
      }
      
      
      

      vector<char*>c_args;
      for(auto &it:realArgs){
          c_args.push_back(const_cast<char*>(it.c_str()));
      }
      c_args.push_back(nullptr);
      
      pid_t pid=fork();
      
      if(pid==0){
          
            if (in_fd != -1) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != -1) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
          execvp(c_args[0],c_args.data());
          perror("faild to execute");
          exit(1);
      }else if(pid>0){
          wait(NULL);
          
          if (in_fd != -1) close(in_fd);
    if (out_fd != -1) close(out_fd);
      }else{
          perror("faild to fork");
    
       }
      
  }
    return 0;
}

