#include<bits/stdc++.h>
using namespace std;



//both  used to display the home directory or current directory
void disDir(){
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    cout<<cwd<<endl;
}

void disDir2(){
    vector<char>cwd(1024);
    getcwd(cwd.data(),cwd.size());
    cout<<cwd.data();
}



int main(){

 fork() creates two processes: a parent and a child.
 Typically, the child process is used to execute commands,
while the parent process waits for the child to complete its task.



fork()
Purpose: Creates a new process by duplicating the current process.

Syntax: pid_t pid = fork();

What happens:
It returns 0 to the child process.

It returns the child’s PID to the parent process.

It returns -1 if the fork fails
pid_t pid = fork();

if (pid == 0) {
    // This is the child process
} else if (pid > 0) {
    // This is the parent process
} else {
    // fork() failed
}


                                                          1. External Commands
                                                        -----------------------

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main() {
    const char *args[] = {"ls", "-l", NULL};

    pid_t pid = fork();

    if (pid == 0) {
        execvp("ls", (char *const *)args); // Cast to match execvp's expected signature
        perror("exec failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork failed");
    }

    return 0;
}


The execvp("ls", args) call is used to run a shell-like command from your C or C++ program — in this case, it runs the ls command.

if (execvp("ls", args) == -1) {
   //failed to execute
}

You can use perror() to see what went wrong.
    
//program to take input from user it only execute( 1. External Commands)
string input;

while (true) {
    cout << "cpp-shell> ";
    getline(cin, input);

    if (input.empty()) continue;
    if (input == "exit") break;

    // Tokenize input
    istringstream iss(input);
    string token;
    vector<char*> args;

    while (iss >> token) {
        args.push_back(strdup(token.c_str()));
    }
    args.push_back(nullptr); // execvp needs a null-terminated array

    // Fork and execute
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args.data());
        perror("exec failed");
        exit(1);
    } else if (pid > 0) {
        // Parent process
        wait(NULL);
    } else {
        perror("fork failed");
    }

    // Free duplicated strings
    for (char* arg : args) {
        free(arg);
    }
}





✅ 1. External Commands
-----------------------
What It Is:
- Commands like ls, pwd, date, cat, etc. that exist as executable files on disk (e.g., /bin/ls).

Status: ✅ Works Now
- Using fork() + execvp() to run these.

Examples:
cpp-shell> ls
cpp-shell> cat myfile.txt

❌ 2. Shell Built-ins
---------------------
What It Is:
- Commands built into the shell, not separate executables.
- Examples: cd, exit, export, alias.

Status: ❌ Not Working Yet
- execvp() can't run them — must handle manually in C++.

What's Needed:
- Check command before fork.
- Use:  
    - chdir() for "cd"
    - exit(0) for "exit"

Example Code Snippet:
if (command == "cd") {
    chdir(args[1]);
    continue;
}

❌ 3. Pipes and Redirection
---------------------------
What It Is:
- Pipe (|): Output of one command goes to another (e.g., ls | grep cpp)
- Redirection: Send output to file (>), read from file (<)

Status: ❌ Not Working Yet

What's Needed:
- Use pipe(), dup2(), and multiple fork() calls.
- Parse input for |, >, < symbols.
- Redirect file descriptors using dup2().

Examples:
cpp-shell> ls | grep ".cpp"
cpp-shell> echo "hi" > out.txt

❌ 4. Background Jobs (&)
-------------------------
What It Is:
- Run command in background (non-blocking).

Example:
cpp-shell> sleep 5 &

Status: ❌ Not Working Yet

What's Needed:
- Detect & at the end of input.
- Remove & from args.
- Do not call wait() after fork().

Tip:
if (args.back() == "&") {
    args.pop_back();
    fork and execvp as usual;
    // Do not call wait()
}



creating Strating terminal animation
#include <sys/wait.h>
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
using namespace std;
#define clear() printf("\033[H\033[J")



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



                                          2. Shell Built-ins
                                        ---------------------


#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include<bits/stdc++.h>
using namespace std;
#define clear() printf("\033[H\033[J");



map<string,string>aliases;
vector<string>history;
//use to handdle in built commands
bool handle_inBuilt(vector<string>&args){
    string cmd=args[0];
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



int main()
{
  string input;
  
  while(true){
      cout<<"cpp-shell >";
      getline(cin,input);
      
      if(input.empty())continue;
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
      
      
      
      if(handle_inBuilt(args))continue;

      vector<char*>c_args;
      for(auto &it:args){
          c_args.push_back(const_cast<char*>(it.c_str()));
      }
      c_args.push_back(nullptr);
      
      pid_t pid=fork();
      
      if(pid==0){
          execvp(c_args[0],c_args.data());
          perror("faild to execute");
          exit(1);
      }else if(pid>0){
          wait(NULL);
      }else{
          perror("faild to fork");
    
       }
      
  }
    return 0;
}




                     (PIPES NOTES-->)




# Detailed Notes: Pipes and Their Implementation in C++ (with Code)

---

# 1. What are Pipes?

- A **pipe** is a **mechanism** for **inter-process communication (IPC)**.
- It allows **one-way communication**: one process writes, another reads.
- In Unix/Linux, created by the `pipe(fd)` system call.

**Analogy**: Think of a water pipe - one end puts water (write), another takes water (read).

**Basic Code:**
```cpp
#include <unistd.h>
int fd[2];
pipe(fd); // fd[0] - read end, fd[1] - write end
```

---

# 2. How Pipes Work Internally

- `pipe(fd)` creates two file descriptors:
  - `fd[0]` -> **Read end** of pipe
  - `fd[1]` -> **Write end** of pipe

**Example Code:**
```cpp
int fd[2];
if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
}
```

- Data written into `fd[1]` can be read from `fd[0]`.

---

# 3. How We Use Pipes in Shell Commands

Example command:
```bash
ls -l | wc -l
```
- `ls -l` outputs into the pipe.
- `wc -l` reads from the pipe.

**Code Simulation:**
```cpp
// After fork(), set up pipe between ls and wc
// Explained in section 4
```

---

# 4. Full Code Explanation: Step-by-Step

### Step-by-Step Shell Simulation Code:
```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to write-end
        close(fd[0]);
        close(fd[1]);
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);
        perror("execvp");
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO); // Redirect stdin to read-end
        close(fd[1]);
        close(fd[0]);
        char *args[] = {"wc", "-l", NULL};
        execvp(args[0], args);
        perror("execvp");
    }

    // Parent process
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}
```

---

# 5. Visual Flow Diagram

```plaintext
User types:   ls -l | wc -l

Parent:
 - Creates pipe
 - fork() -> Child 1 (ls -l)
    - STDOUT redirected to pipe[write-end]
 - fork() -> Child 2 (wc -l)
    - STDIN redirected to pipe[read-end]
 - Closes pipe
 - Waits for both
```

---

# 6. Important Functions Table

| Function                        | Purpose                               |
| ------------------------------- | ------------------------------------- |
| `pipe(fd)`                      | Create a pipe.                        |
| `fork()`                        | Create a child process.               |
| `dup2(oldfd, newfd)`             | Duplicate file descriptor (redirect). |
| `execvp(file, argv)`             | Execute a new program.                |
| `waitpid(pid, status, options)`  | Wait for specific child process.      |

**Example mini-codes:**
```cpp
pipe(fd);
if (fork() == 0) execvp(...);
waitpid(child_pid, NULL, 0);
```

---

# 7. Why Close File Descriptors After `dup2`

✅ After redirecting with `dup2()`, close the original `fd[0]` and `fd[1]`.

**Reason:**
- A pipe signals **EOF** (end of file) only when all write-ends are closed.
- If you don't close:
  - Reader **hangs forever** waiting for EOF.
  - Program **freezes**.

**Example:**
```cpp
dup2(fd[1], STDOUT_FILENO);
close(fd[0]);
close(fd[1]);
```

✅ Always **close unused pipe ends** to prevent memory leaks and deadlocks.

**Explanation:**
- Closing fd[0] in the writer.
- Closing fd[1] in the reader.
- Prevents unnecessary open file descriptors and system resource leaks.

---

# 8. How Parent Waits for Child Processes

✅ After forking:

- **Parent** must wait for both children.

Why?
- Otherwise, children become **zombie processes** (defunct but not cleaned).

Use:
```cpp
waitpid(pid1, nullptr, 0);
waitpid(pid2, nullptr, 0);
```

✅ This ensures all system resources are released.

**Zombie Process Example:**
```bash
ps aux | grep defunct
```
- Shows zombie processes if parent does not wait.

---

# 9. Internal Step-by-Step Process

| Step | Action         | Result                                              |
| ---- | -------------- | -------------------------------------------------- |
| 1    | Create pipe    | fd[0]: read-end, fd[1]: write-end                   |
| 2    | Fork Child 1   | Child 1 created                                     |
| 3    | Child 1        | Redirect STDOUT to fd[1], close pipe, exec `ls -l`  |
| 4    | Fork Child 2   | Child 2 created                                     |
| 5    | Child 2        | Redirect STDIN to fd[0], close pipe, exec `wc -l`   |
| 6    | Parent         | Close both ends of pipe, wait for children          |

**Summary View Code:**
```cpp
pipe(fd);
if (fork() == 0) { dup2(fd[1], STDOUT_FILENO); execvp(...); }
if (fork() == 0) { dup2(fd[0], STDIN_FILENO); execvp(...); }
close(fd[0]); close(fd[1]);
waitpid(...);
```

---

# 10. Summary

- **Pipe** connects output of one process to input of another.
- Use **pipe()**, **fork()**, **dup2()**, and **execvp()**.
- **Always close** unnecessary file descriptors.
- **Parent must wait** for children.

✅ With these basics, you can build simple shell programs!

---

# Bonus: Handling Multiple Pipes (optional)

Example:
```bash
ls | grep ".cpp" | sort | wc -l
```
- Involves chaining multiple pipes.
- Create pipe, fork, redirect, exec for each command.
- I can explain this if needed!


DATA FLOW--> ls -l | wc -l

[ ls -l ] --stdout--> [ pipe ] --stdin--> [ wc -l ]
            (fd[1])                (fd[0])

[ ls -l ] --output in pipe--> [ pipe ] --input from pipe--> [ wc -l ]
            (fd[1])                       (fd[0])



Overall Flow of Pipe Between Two Commands


1) Parent creates a pipe using pipe(fd):
    - fd[0]: Read end of the pipe
    - fd[1]: Write end of the pipe
2) First child (Child 1):
    - Created using fork()
    - Redirects STDOUT to fd[1] using dup2
    - Executes the left-side command (e.g., ls -l)
    - Output of the command is written into the pipe
3) Second child (Child 2):
    - Created using another fork()
    - Redirects STDIN to fd[0] using dup2
    - Executes the right-side command (e.g., wc -l)
    - Input for the command is read from the pipe (i.e., output of Child 1)
4) Parent process:
    - Closes both fd[0] and fd[1] after forking
    - Waits for both children to complete using wait()








            IMPLEMNETED PIPES'''


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


calling--->
if(input.size()==0)continue;
if(handle_pipes(input))continue;



'''
I/O Redirections


Shell I/O Redirection Explained


What is Redirection?
Redirection in the shell allows you to redirect the input or output of a command to or from a file instead of the default
stdin (keyboard) or stdout (screen).



                         Types of Redirection
          Symbol | Meaning | Example | Description
         --------|----------------------|-----------------------|--------------------------------------------
         < | Input Redirection | sort < input.txt | Takes input from a file
         > | Output Redirection | ls > out.txt | Writes output to a file (overwrites)
        >> | Append Redirection | echo Hello >> out.txt | Appends output to an existing file



How Redirection Works Internally
Uses file descriptors:



- stdin = 0
- stdout = 1
- stderr = 2

Redirect using dup2():
- dup2(fd, 1) to redirect stdout
- dup2(fd, 0) to redirect stdin


Shell Function and Logic
1. Detect Redirection Symbols in args[]
2. Open File with proper mode
3. In child process: use dup2() to redirect stdin or stdout
4. Execute command using execvp()



Examples
1. sort < unsorted.txt Input comes from a file
2. ls > files.txt Output written to a file (overwrite)
3. echo Hello >> logs.txt Output appended to a file



Summary Table
Operation | File Opening Mode | Duplication
----------|--------------------------------------|---------------
< | O_RDONLY | dup2(fd, 0)
> | O_WRONLY | O_CREAT | O_TRUNC | dup2(fd, 1)
>> | O_WRONLY | O_CREAT | O_APPEND | dup2(fd, 1)


'''


#include <fcntl.h>
int in_fd = -1, out_fd = -1;
bool append = false;

// Identify redirection operators
vector<string> realArgs;
for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "<") {
        if (i + 1 < args.size()) {
            in_fd = open(args[i + 1].c_str(), O_RDONLY);
            if (in_fd < 0) {
                perror("Failed to open input file");
                break;
            }
            ++i; // skip filename
        }
    } else if (args[i] == ">") {
        if (i + 1 < args.size()) {
            out_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
                perror("Failed to open output file");
                break;
            }
            ++i;
        }
    } else if (args[i] == ">>") {
        if (i + 1 < args.size()) {
            out_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (out_fd < 0) {
                perror("Failed to open output file");
                break;
            }
            append = true;
            ++i;
        }
    } else {
        realArgs.push_back(args[i]);
    }
}

vector<char*> c_args;
for (auto& it : realArgs) {
    c_args.push_back(const_cast<char*>(it.c_str()));
}
c_args.push_back(nullptr);

pid_t pid = fork();
if (pid == 0) {
    if (in_fd != -1) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != -1) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
    execvp(c_args[0], c_args.data());
    perror("Failed to execute command");
    exit(1);
} else if (pid > 0) {
    wait(NULL);
    if (in_fd != -1) close(in_fd);
    if (out_fd != -1) close(out_fd);
} else {
    perror("Failed to fork");
}




