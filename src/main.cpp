#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <stdlib.h>
#include <vector>

using namespace std;

void string_parsing(string String,vector<string> & char_array){
	char *token_1;
	token_1 = strtok(&String[0], " ");
	while(token_1 !=NULL){
	char_array.push_back(token_1);
	token_1 = strtok(NULL, " ");	
	}		
}


int main(){
	int status;
	vector<string> char_array;
	vector<string> pars_array;
	char *login_name;
	int n;
	int i;
	int pid;
	login_name = getlogin();
	char host_name[MAXHOSTNAMELEN];
	if(gethostname(host_name,sizeof host_name) == -1){perror("hostname");//check to see if we can obtain host name.
	strcpy(host_name,"");
	}	
	string usr_input;
	string usr_input2;	
	

	while(usr_input != "exit"){
	beginning:
	if(!login_name){perror("getlogin");//check to see if we can obtain login info
	}
	else{
	printf("%s",login_name);
	}
	cout  << "@" << host_name << "$"; // outputs $ and waits for user input.
	getline(cin, usr_input);
	string usr_input2;
	if(usr_input.empty()){//Checks to see if user just clicked enter and not input anything.
		goto beginning;
		}
			
		char * token;
		token = strtok(&usr_input[0], "#");//this will remove the comment from user.
		usr_input = token;
	
	int trav_str = usr_input.size();
	int str_ctr  = 0;
	for(str_ctr=0;str_ctr < trav_str;++str_ctr){
												//this loop will go through to see what user has inputted (just single command,
												//separated by ; or && or ||. (NOTE: this bash is not compatible with
												//different arguments in one input. i.e. ls ; ls || ls && ls.
		
		if(usr_input == "exit"){ //will check if user wants to end program.
		cout << "Exiting..."<< endl;
		char_array.clear();
		pars_array.clear();
		goto end_of_while;
		}
		else if(usr_input[str_ctr] == ';'){//this will implement if the user has separated with a semicolon.

			token = strtok(&usr_input[0], ";");
				while(token != NULL){
					pars_array.push_back(token);
					token = strtok(NULL,";");
				}
			int pars_size = pars_array.size()-1;
			int pars_ctr  = 0;		
	
			for(pars_ctr =0; pars_ctr <= pars_size; pars_ctr++){
				usr_input2 = pars_array[pars_ctr];
				string_parsing(usr_input2,char_array);			
				int i = char_array.size();
				char * argv[100];
				int n = 0;
					for(n = 0; n <= i-1; ++n){
						argv[n] = new char[char_array[n].size()+1];
					}
			for(n = 0; n < i; ++n){
				strcpy(argv[n], char_array[n].c_str());
			}
			argv[n] = NULL;
			int pid = fork();//will fork program and let child run execvp and then waits for child process to tell parent it is done.
			if(pid == -1){
				perror("There was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("There was an error");	
				}
				exit(1);
			}	
			else if(pid > 0){
				if(-1 == wait(0)){perror("There was an error with wait().");}	
			}			
			char_array.clear();
			}
		char_array.clear();
		pars_array.clear();
		goto end_of_while;
		}
		


		else if(usr_input[str_ctr] == '&' && usr_input[str_ctr+1] == '&'){//this will be implemented if user separates with &&.

			token = strtok(&usr_input[0], "&&");
				while(token != NULL){
					pars_array.push_back(token);
					token = strtok(NULL,"&&");
				}
			
			int pars_size = pars_array.size();
			int pars_ctr  = 0;		
			
	
		for(pars_ctr =0; pars_ctr < pars_size; pars_ctr++){
			
				usr_input2 = pars_array[pars_ctr];
				
				string_parsing(usr_input2,char_array);			
				int i = char_array.size();
				char * argv[100];
				int n = 0;
					for(n = 0; n <= i-1; ++n){
						argv[n] = new char[char_array[n].size()+1];
					}
			for(n = 0; n < i; ++n){
				strcpy(argv[n], char_array[n].c_str());
			}
			argv[n] = NULL;
			int pid = fork();//will fork program and let child run execvp and then waits for child process to tell parent it is done.
		
			if(pid == -1){
				perror("There was error with fork().");
				exit(1);}
			else if(pid == 0){
				if( -1 == execvp(argv[0],argv)){
					perror("There was an error");	
					exit(1);
					}
			}	
			wait(&status);
			if(WIFEXITED(status)){
			if(WEXITSTATUS(status) != 0){
				
			goto end_of_while;
			}
			}
			
					
			char_array.clear();

			}

	
		char_array.clear();
		pars_array.clear();
		goto end_of_while;
		}
		
		else if(usr_input[str_ctr] == '|' && usr_input[str_ctr+1] == '|'){//This will be implemented if user inputs ||

			token = strtok(&usr_input[0], "||");
				while(token != NULL){
					pars_array.push_back(token);
					token = strtok(NULL,"||");
				}
				
				int pars_size = pars_array.size()-1;
				int pars_ctr  = 0;		
	
			for(pars_ctr =0; pars_ctr <= pars_size; pars_ctr++){
				usr_input2 = pars_array[pars_ctr];
				string_parsing(usr_input2,char_array);			
				int i = char_array.size();
				char * argv[100];
				int n = 0;
					for(n = 0; n <= i-1; ++n){
						argv[n] = new char[char_array[n].size()+1];
					}
			for(n = 0; n < i; ++n){
				strcpy(argv[n], char_array[n].c_str());
			}
			argv[n] = NULL;
			int pid = fork();//will fork program and let child run execvp and then waits for child process to tell parent it is done.
			if(pid == -1){
				perror("There was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("There was an error");	
					exit(1);
						}
					
			}	
			wait(&status);
			if(WIFEXITED(status)){
			if(WEXITSTATUS(status) == 0){
				
			goto end_of_while;
			}
			}
		
			char_array.clear();
			}
		char_array.clear();
		pars_array.clear();
		goto end_of_while;
		}
		}
		if(!usr_input.empty()){	//when user just inputs no argument.
	

			string_parsing(usr_input,char_array);
		
			i = char_array.size();
			char * argv[100];
			n = 0;
			for(n = 0; n <= i-1; ++n){
				argv[n] = new char[char_array[n].size()+1];
			}
			for(n = 0; n < i; ++n){
				strcpy(argv[n], char_array[n].c_str());
			}
			argv[n] = NULL;
			pid = fork();//will fork program and let child run execvp and then waits for child process to tell parent it is done.
			if(pid == -1){
				perror("There was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("There was an error");	
					}
				exit(1);
			}		
			else if(pid > 0){
				if(-1 == wait(0)){perror("There was an error with wait().");}
			}
			char_array.clear();
			pars_array.clear();
			}
	end_of_while://this will go back to the beginning (while(!=exit)). It will also clear char_array and pars_array.
	if(char_array.size() > 0){
	char_array.clear();}
	if(pars_array.size() > 0){
	pars_array.clear();}	
}


	return 0;		
}
