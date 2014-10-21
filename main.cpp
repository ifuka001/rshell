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

	vector<string> char_array;
	vector<string> pars_array;
	char *login_name;
	int n;
	int i;
	int pid;
	login_name = getlogin();
	char host_name[MAXHOSTNAMELEN];
	gethostname(host_name,sizeof host_name);	
	string usr_input;
	string usr_input2;	
	


	while(usr_input != "exit"){
	cout << login_name << "@" << host_name << "$"; 
	getline(cin, usr_input);
	char * token;
	token = strtok(&usr_input[0], "#");
	usr_input = token;
	string usr_input2;

		
	int trav_str = usr_input.size();
	int str_ctr  = 0;
	for(str_ctr=0;str_ctr < trav_str;++str_ctr){
		
		
		if(usr_input == "exit"){ 
		cout << "Exiting..."<< endl;
		goto end_of_while;
		}
		
		if(usr_input[str_ctr] == ';'){
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
			int pid = fork();
			if(pid == -1){
				perror("there was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("there was an error");	
				}
				exit(1);
			}	
			else if(pid > 0){
				if(-1 == wait(0)){perror("there was an error with wait().");}	
			}			
			char_array.clear();
			}

		pars_array.clear();
		goto end_of_while;
		}
		


		else if(usr_input[str_ctr] == '&' && usr_input[str_ctr+1] == '&'){
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
			int pid = fork();
			if(pid == -1){
				perror("there was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("there was an error");	
					goto end_of_while;
					}
				exit(1);
			}	
			else if(pid > 0){
				if(-1 == wait(0)){perror("there was an error with wait().");}	
			}			
			char_array.clear();
			}

		pars_array.clear();
		goto end_of_while;
		}
		
		else if(usr_input[str_ctr] == '|' && usr_input[str_ctr+1] == '|'){
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
			int pid = fork();
			if(pid == -1){
				perror("there was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("there was an error");	
				}
				exit(1);
			}	
			else if(pid > 0){
				if(-1 == wait(0)){perror("there was an error with wait().");}	
			}			
			char_array.clear();
			}

		pars_array.clear();
		goto end_of_while;
		}
		}
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
			pid = fork();
			if(pid == -1){
				perror("there was error with fork().");
				exit(1);}
			else if(pid == 0){
				if(-1 == execvp(argv[0],argv)){
					perror("there was an error");	
				}
				exit(1);
			}		
			else if(pid > 0){
				if(-1 == wait(0)){perror("there was an error with wait().");}
			}
			char_array.clear();
			pars_array.clear();
	end_of_while:
	char_array.clear();
	
}


	return 0;		
}
