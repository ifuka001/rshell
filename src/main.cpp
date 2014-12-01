#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <stdlib.h>
#include <fcntl.h>
#include <vector>


static int sig_count = 0;
char *login_name;
char host_name[MAXHOSTNAMELEN];

using namespace std;


void killsignal(int signum)
{
	string cwd = "";
	char buf[BUFSIZ];
	if(getcwd(buf,sizeof(buf))== NULL)
	{
		perror("getcwd");
		strcpy(buf,cwd.c_str());
	}
	if(sig_count < 1)
	{
	cerr  << login_name <<  "@" << host_name << ":" << buf <<  "$ "; 
	}
	else
	{
		cerr << endl;
	}
}


//parses string according to " " space
void string_parsing(string String,vector<string> & char_array){
	char *token_1;
	token_1 = strtok(&String[0], " ");
	while(token_1 !=NULL){
	char_array.push_back(token_1);
	token_1 = strtok(NULL, " ");	
	}		
}

//gets called when pipe was used in command.
void pipe_2(vector<string> pipe_arr,int counter,int size)
{
	int fd[100][2];
	for(int i = 0;i<size-1;i++)
	{
		if(pipe(fd[i])<0)
		{
			perror("pipe");
			//exit(0);
			return;
		}
		
	}
		vector <string> temp;
		unsigned int n = 0;
		char *argv[100];
		int i = 0;
		//this is where it goes through each pipe and executes it.
		while(i < size -1)
		{
			bool in = false;
			bool out = false;
			bool app = false;
			string input = pipe_arr[i];
			//goes through to see what command user used.
			for(unsigned int s = 0; s < input.size(); ++s)
			{
			
				if(input[s] == '<')
				{
					in = true;	
				}
				if((input[s] == '>' && input[s+1] != '>') && (input[s]=='>' && input[s-1] != '>'))
				{	
					out = true;
				}
				if(input[s] == '>' && input[s+1] == '>')
				{
					app = true;
				}
			}
		
			if(in == true)
			{
					
				string usr = pipe_arr[i];
				char* tok = strtok(&usr[0],"<");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<");
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int inp = open(c,O_RDONLY);
				if(inp < 0)
				{
					perror("open");
					return;
				}
				if(counter == 0)
				{
					counter ++;
				}
				int pid = fork();
				if(pid == 0)
				{
						if( -1 == dup2(inp,0))
						{
							perror("dup2");
							exit(0);
						}
						if( -1 == dup2(fd[i][1],1))
						{
							perror("dup2");
							exit(0);
						}
						if( -1 == close(inp))
						{
							perror("close");
							exit(0);
						}
						
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(argv[0],argv))
							{
								perror("execvp");
								exit(1);
							}
				}
				else if(pid == -1)
				{
					perror("fork");
					exit(1);
				}
				
			}
			else if(out == true)
			{
			
				string usr = pipe_arr[i];
				char* tok = strtok(&usr[0],">");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,">");
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int out = open(c,O_WRONLY | O_TRUNC | O_CREAT ,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
					return;
				}
				if(counter == 0)
				{
					counter ++ ;
					int pid = fork();
					if(pid==0)
					{
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(*argv,argv))
						{
							perror("execvp");
							exit(1);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						exit(1);
					}
					
				}
				else
				{
					int pid = fork();
					if(pid==0)
					{
						if(-1 == dup2(fd[i-1][0],0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(*argv,argv))
						{
							perror("execvp");
							exit(0);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						exit(0);
					}
				}
			
		}
		else if(app == true)
		{
		
			string usr = pipe_arr[i];
			char* tok = strtok(&usr[0],">>");
			while(tok != NULL)
			{
				temp.push_back(tok);
				tok = strtok(NULL,">>");
			}
			vector <string> temp2;
			string input1 = temp[0];
			string_parsing(input1,temp2);
			for(n=0; n < temp2.size();n++)
			{
				argv[n] = new char[temp2[n].size()+1];
			}
			for(n=0;n<temp2.size();n++)
			{
				strcpy(argv[n],temp2[n].c_str());
			}
			argv[n] = NULL;
			int check = 0;
			while(temp[1][check] == ' ' )
			{
				temp[1].erase(0,1);
			}
			check = temp[1].size();
			string t = temp[1];
			while(t[t.length()-1] == ' ')
			{
				t = t.substr(0,t.size()-1);
					
			}
			const char* c = t.c_str();
			int out = open(c,O_WRONLY | O_CREAT | O_APPEND,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
				//	exit(0);
					return;
				}
				int pid = fork();
				if(pid==0)
				{
					if(-1 == dup2(out,1))
					{
						perror("dup2");
						exit(0);
					}
					if(-1 == dup2(fd[i-1][0],0))
					{
						perror("dup2");
						exit(0);
					}
					for(int i = 0 ; i < size-1 ; ++i)
					{
						if(-1 == close(fd[i][0]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[i][1]))
						{
							perror("close");
							exit(0);
						}
					}
					if(-1 == close(out))
					{
							perror("close");
							exit(0);
					}
					if(-1 == execvp(*argv,argv))
					{
						perror("execvp");
						exit(1);
					}
					
				}
				else if(pid == -1)
				{
					perror("fork");
					//exit(0);
					return;
				}
		}
			else
			{
				string_parsing(input,temp);
				for(n = 0; n < temp.size(); n++)
				{
					argv[n] = new char[temp[n].size()+1];
				}
				for(n = 0; n < temp.size(); n++)
				{
					strcpy(argv[n],temp[n].c_str());
				}
				argv[n] = NULL;
				if(counter == 0)
				{
					counter ++;
					int pid = fork();
					if(pid==0)
					{
						if(-1 == dup2(fd[i][1],1))
						{
							perror("dup2");
							exit(0);
						}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(*argv,argv))
						{
							perror("execvp");
							exit(0);
						}
						
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
				}
				else
				{
					int pid = fork();
					if(pid == 0)
					{
						if(-1 == dup2(fd[i-1][0],0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(fd[i][1],1))
						{
							perror("dup2");
							exit(0);
						}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(*argv,argv))
						{
							perror("execvp");
							exit(1);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
				}
			}
				++i;
				temp.clear();

				delete[] *argv;
		//		memset(argv,'\0',100);
		}
		//this is where the last pipe gets called. it's separated because it uses different file descriptor.
		
		
			bool in = false;
			bool out = false;
			bool app = false;
			string input = pipe_arr[size-1];
			for(unsigned int s = 0; s < input.size(); ++s)
			{
			
				if(input[s] == '<')
				{
					in = true;	
				}
				if((input[s] == '>' && input[s+1] != '>') && (input[s]=='>' && input[s-1] != '>'))
				{	
					out = true;
				}
				if(input[s] == '>' && input[s+1] == '>')
				{
					app = true;
				}
			}
		
			if(in == true)
			{
					
				string usr = pipe_arr[i];
				char* tok = strtok(&usr[0],"<");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<");
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int inp = open(c,O_RDONLY);
				if(inp < 0)
				{
					perror("open");
					return;
				}
				if(counter == 0)
				{
					counter ++;
				}
				int pid = fork();
					if(pid == 0)
					{
						if(-1 == dup2(inp,0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(fd[i][1],1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(inp))
						{
							perror("close");
							exit(0);
						}
						
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(argv[0],argv))
						{
							perror("execvp");
							exit(0);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
			}
			else if(out == true)
			{
			
				string usr = pipe_arr[i];
				char* tok = strtok(&usr[0],">");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,">");
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int out = open(c,O_WRONLY | O_TRUNC | O_CREAT ,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
			
				if(out < 0)
				{
					perror("open");
					//exit(0);
					return;
				
				}
				if(counter == 0)
				{
					counter ++ ;
					int pid = fork();
					if(pid ==0)
					{
						if(-1==dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(fd[i-1][0],0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(*argv,argv))
						{
							perror("execvp");
							exit(1);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
				}
				else
				{
					int pid = fork();
					if(pid==0)
					{
						if(-1 == dup2(fd[i-1][0],0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
						if(-1 == execvp(*argv,argv))
						{
							perror("execvp");
							exit(1);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
				}
			
		}
		else if(app == true)
		{
		
			string usr = pipe_arr[i];
			char* tok = strtok(&usr[0],">>");
			while(tok != NULL)
			{
				temp.push_back(tok);
				tok = strtok(NULL,">>");
			}
			vector <string> temp2;
			string input1 = temp[0];
			string_parsing(input1,temp2);
			for(n=0; n < temp2.size();n++)
			{
				argv[n] = new char[temp2[n].size()+1];
			}
			for(n=0;n<temp2.size();n++)
			{
				strcpy(argv[n],temp2[n].c_str());
			}
			argv[n] = NULL;
			int check = 0;
			while(temp[1][check] == ' ' )
			{
				temp[1].erase(0,1);
			}
			check = temp[1].size();
			string t = temp[1];
			while(t[t.length()-1] == ' ')
			{
				t = t.substr(0,t.size()-1);
					
			}
			const char* c = t.c_str();
			int out = open(c,O_WRONLY | O_CREAT | O_APPEND,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
					//exit(0);
					return;
				}
				int pid = fork();
				if(pid==0)
				{
					if(-1==dup2(out,1))
					{
						perror("dup2");
						exit(0);
					}
					if(-1 == dup2(fd[i-1][0],0))
					{
						perror("dup2");
						exit(0);
					}
					if(-1 == close(out))
					{
						perror("close");
						exit(0);
					}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
					if(-1 == execvp(*argv,argv))
					{
						perror("execvp");
						exit(1);
					}
				}
				else if(pid == -1)
				{
					perror("fork");
					//exit(0);
					return;
				}
		}
		else
		{
				string input = pipe_arr[size-1];
				string_parsing(input,temp);
				for(n = 0; n < temp.size(); n++)
				{
					argv[n] = new char[temp[n].size()+1];
				}
				for(n = 0; n < temp.size(); n++)
				{
					strcpy(argv[n],temp[n].c_str());
				}
				argv[n] = NULL;
				int pid = fork();
				if(pid == 0)
				{
					if(-1 == dup2(fd[i-1][0],0))
					{
						perror("dup2");
						exit(0);
					}
						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								exit(0);
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}
					if(-1==execvp(*argv,argv))
					{
						perror("execvp");
						exit(0);
					}
				}
				else if(pid == -1)
				{
					perror("fork");
					//exit(0);
					return;
				}

		}

			delete[] *argv;

						for(int i = 0 ; i < size-1 ; ++i)
						{
							if(-1 == close(fd[i][0]))
							{
								perror("close");
								//exit(0);
								return;
							}
							if(-1 == close(fd[i][1]))
							{
								perror("close");
								exit(0);
							}
						}

				for(int i = 0; i < size; i++)
				{
					if(wait(0)<0)
					{
						perror("wait");
						//exit(0);
						return;
					}
					
				}
}	

//this is where user only inputs i/o redirections.
void input_output(string usr_input)
{
	if(usr_input == "<" || usr_input == ">" || usr_input == ">>")
	{
		cout << "no arguments"<< endl;
		return;
	}
			int fd[2];
			int fd1[2];
			if(-1 == pipe(fd))
			{
				perror("pipe");
				//exit(0);
				return;
			}
			if(-1 == pipe(fd1))
			{
				perror("pipe");
				//exit(0);
				return;
			}
			int i =0;
			int o =0;
			int a =0;
			bool in = false;
			bool out = false;
			bool app = false;
			string input = usr_input; 
			vector <string> temp;
			unsigned int n = 0;
			char *argv[100];
			//checks for what i/o they used
			for(unsigned int s = 0; s < input.size(); ++s)
			{
			
				if(input[s] == '<')
				{
					in = true;
					i++;
				}
				if((input[s] == '>' && input[s+1] != '>') && (input[s]=='>' && input[s-1] != '>'))
				{	
					out = true;
					o++;
				}
				if(input[s] == '>' && input[s+1] == '>')
				{
					app = true;
					a++;
				}
			}
			if( i > 1 || o > 1 || a > 1)
			{
				cout << "you may not use more than one of the same i/o redirection" << endl;
				return;
			}
			if( app == true && out ==true)
			{
				cout << "does not support multiple output redirection" << endl;
				return;
			}
			//for < and > used
			if(in == true && out == true)
			{
			
				char* tok = strtok(&input[0],"<");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<>");
				}
				if(temp.size() <3)
				{
					cout << "not enough argument" << endl;
					return;
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				int check2 = 0;
				while(temp[2][check2] == ' ' )
				{
					temp[2].erase(0,1);
				}
				check2 = temp[2].size();
				string t1 = temp[2];
				while(t1[t1.length()-1] == ' ')
				{
					t1 = t1.substr(0,t1.size()-1);
						
				}
				const char* o = t1.c_str();
				const char* c = t.c_str();
				int in = open(c,O_RDONLY);
				if(in < 0)
				{
					perror("open");
					//exit(0);
					return;
				}
				int out = open(o,O_WRONLY | O_TRUNC | O_CREAT ,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
				//	exit(0);
					return;
				}
				int pid = fork();
					if(pid == 0)
					{
						if(-1 == dup2(in,0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(in))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[0]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[1]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd1[0]))
						{
							perror("close");
							exit(0);				
						}
						if(-1 == close(fd1[1]))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == execvp(argv[0],argv))
						{
							perror("execvp");
							exit(0);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
					//	exit(0);
						return;
					}
			}
			//goes in this statement when < and >> is used.
			else if(in == true && app == true)
			{
			
				char* tok = strtok(&input[0],"<");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<>>");
				}
				if(temp.size() < 3 )
				{
					cout << "not enough argument"<<endl;
					return;
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				int check2 = 0;
				while(temp[2][check2] == ' ' )
				{
					temp[2].erase(0,1);
				}
				check2 = temp[2].size();
				string t1 = temp[2];
				while(t1[t1.length()-1] == ' ')
				{
					t1 = t1.substr(0,t1.size()-1);
						
				}
				const char* o = t1.c_str();
				const char* c = t.c_str();
				int in = open(c,O_RDONLY);
				if(in < 0)
				{
					perror("open");
					//exit(0);
					return;
				}
				int out = open(o,O_WRONLY | O_APPEND | O_CREAT ,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
				//	exit(0);
					return;
				}
				int pid = fork();
				if(pid == 0)
				{
						if(-1 == dup2(in,0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(in))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[0]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[1]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd1[0]))
						{
							perror("close");
							exit(0);				
						}
						if(-1 == close(fd1[1]))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == execvp(argv[0],argv))
						{
							perror("execvp");
							exit(0);
						}
				}
				else if(pid == -1)
				{
					perror("fork");
					//exit(0);
					return;
				}
			}
			// this is when < is true.
			else if(in == true)
			{
				char* tok = strtok(&input[0],"<");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<>");
				}
				if(temp.size() < 2)
				{
					cout << "not enough argument"<<endl;
					return;
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int inp = open(c,O_RDONLY);
				if(inp < 0)
				{
					perror("open");
					//exit(0);
					return;
				}
				int pid = fork();
					if(pid == 0)
					{
						if(-1 == dup2(inp,0))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(inp))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == close(fd[0]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[1]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd1[0]))
						{
							perror("close");
							exit(0);				
						}
						if(-1 == close(fd1[1]))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == execvp(argv[0],argv))
						{
							perror("execvp");
							exit(0);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
			}
			//used when > is only command used.
			else if(out == true)
			{
				char* tok = strtok(&input[0],">");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<>");
				}
				if(temp.size() < 2)
				{
					cout << "not enough argument"<<endl;
					return;
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int out = open(c,O_WRONLY | O_TRUNC | O_CREAT ,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
					//exit(0);
					return;
				}
				int pid = fork();
					if(pid == 0)
					{
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == close(fd[0]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[1]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd1[0]))
						{
							perror("close");
							exit(0);				
						}
						if(-1 == close(fd1[1]))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == execvp(argv[0],argv))
						{
							perror("execvp");
							exit(0);
						}
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
			}
			//goes in when >> is the only i/o used.
			else if(app == true)
			{
				char* tok = strtok(&input[0],">>");
				while(tok != NULL)
				{
					temp.push_back(tok);
					tok = strtok(NULL,"<>>");
				}
				if(temp.size() < 2)
				{
					cout << "not enough argument"<<endl;
					return;
				}
				vector <string> temp2;
				string input1 = temp[0];
				string_parsing(input1,temp2);
				for(n=0; n < temp2.size();n++)
				{
					argv[n] = new char[temp2[n].size()+1];
				}
				for(n=0;n<temp2.size();n++)
				{
					strcpy(argv[n],temp2[n].c_str());
				}
				argv[n] = NULL;
				int check = 0;
				while(temp[1][check] == ' ' )
				{
					temp[1].erase(0,1);
				}
				check = temp[1].size();
				string t = temp[1];
				while(t[t.length()-1] == ' ')
				{
					t = t.substr(0,t.size()-1);
						
				}
				const char* c = t.c_str();
				int out = open(c,O_WRONLY | O_APPEND | O_CREAT ,S_IRUSR|S_IRGRP | S_IWGRP | S_IWUSR);
				if(out < 0)
				{
					perror("open");
					//exit(0);
					return;
				}
				int pid = fork();
					if(pid == 0)
					{
						if(-1 == dup2(out,1))
						{
							perror("dup2");
							exit(0);
						}
						if(-1 == close(out))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == close(fd[0]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd[1]))
						{
							perror("close");
							exit(0);
						}
						if(-1 == close(fd1[0]))
						{
							perror("close");
							exit(0);				
						}
						if(-1 == close(fd1[1]))
						{
							perror("close");
							exit(0);
						}
						
						if(-1 == execvp(argv[0],argv))
						{
							perror("execvp");
							exit(0);
						}	
					}
					else if(pid == -1)
					{
						perror("fork");
						//exit(0);
						return;
					}
			}
	temp.clear();
	delete[] *argv;
	if(wait(0)<0)
	{
		perror("wait");
		//exit(0);
		return;
	}
}


void change_dir(char *argv[])
{	
	const char *new_path;
	char buf[BUFSIZ];	
	if(getcwd(buf,sizeof(buf))== NULL)
	{
		perror("getcwd");
		return;
	}
	string old_path = string(buf);
	if(!argv[1])
	{
		cout << "No where to change directory to" << endl;
		return;
	}
	else if(strcmp(argv[1],".") == 0)
	{
		return;
	}
	else if(strcmp(argv[1],"..") == 0)
	{
		while(old_path[old_path.length()-1] != '/')
		{
			old_path = old_path.substr(0,old_path.size()-1);
		}
		
		old_path = old_path.substr(0,old_path.size()-1);
		new_path = old_path.c_str();
		if(chdir(new_path) == -1)
		{
			perror("chdir");
		return;
		}
		return;
	}
	string old_path2 = string(argv[1]);
	if(old_path2.at(0) == '/')
	{
		new_path = old_path2.c_str();
		if(chdir(new_path) == -1)
		{
			perror("chdir");
			return;
		}
		return;
	}
	else
	{
		old_path.append("/");
		old_path.append(old_path2);
		new_path = old_path.c_str();
		if(chdir(new_path) == -1)
		{
			perror("chdir");
			return;
		}
		return;
	}
	return;
}

void run_command(char *argv[])
{
	vector<string> temp;
	char * token;
	const char *var = "PATH";
	char *path = getenv(var);
	if(path == NULL)
	{
		perror("getenv");
		exit(0);
	}
	string var2 = string(path);
	token = strtok(&var2[0], ":");
	while(token != NULL)
	{
		temp.push_back(token);
		token = strtok(NULL,":");
	}
	for(unsigned int  i = 0; i < temp.size(); i++)
	{
		temp[i].append("/");
		temp[i].append(argv[0]);
		ifstream infile(temp[i].c_str());	
		if(infile.good())
		{
			argv[0] = new char[temp[i].size()+1];
			strcpy(argv[0],temp[i].c_str());
			break;
		}
		else
		{
			infile.close();
		}
	}
	if(execv(argv[0],argv) == -1)
	{
		perror("execv");
		exit(1);
	}
	

}




//this is where main part of rshell happens.
void commands()
{
	int status;
	vector<string> char_array;
	vector<string> pars_array;
	string cwd = "";

	//char *login_name;
	int n;
	int i;
	int pid;
	login_name = getlogin();
	//char host_name[MAXHOSTNAMELEN];
		
	if(!login_name)
	{
		perror("getlogin");//check to see if we can obtain login info
		strcpy(login_name,"");
	}
	else{
	}
	if(gethostname(host_name,sizeof host_name) == -1)
	{
		perror("hostname");//check to see if we can obtain host name.
		strcpy(host_name,"");
	}	
	string usr_input;
	string usr_input2;	
	

	while(usr_input != "exit"){
	beginning:
	char buf[BUFSIZ];
	if(getcwd(buf,sizeof(buf))== NULL)
	{
		perror("getcwd");
		strcpy(buf,cwd.c_str());
	}
	cout  << login_name <<  "@" << host_name << ":" << buf <<  "$ "; // outputs $ and waits for user input.
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
	for(str_ctr=0;str_ctr < trav_str;++str_ctr)
	{
												//this loop will go through to see what user has inputted (just single command,
												//separated by ; or && or ||. (NOTE: this bash is not compatible with
												//different arguments in one input. i.e. ls ; ls || ls && ls.
		sig_count++;	
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
				run_command(argv);
			}	
			else if(pid > 0){
				if(-1 == wait(0))
				{
					perror("There was an error with wait().");
				}	
			}			
			free(*argv);
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
				run_command(argv);
			}	
			if(wait(&status) == -1)
			{
				perror("wait");
				exit(0);
			}
			if(WIFEXITED(status)){
			if(WEXITSTATUS(status) != 0)
			{	
			free(*argv);
			goto end_of_while;
			}
			}
			
					
			free(*argv);
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
				run_command(argv);
			}	
			if(wait(&status) == -1)
			{
				perror("wait");
				exit(0);
			}
			if(WIFEXITED(status))
			{
				if(WEXITSTATUS(status) == 0)
				{
				
					free(*argv);
					goto end_of_while;
				}
			}
		
			free(*argv);
			char_array.clear();
			}
		char_array.clear();
		pars_array.clear();
		goto end_of_while;
		}
		else if(usr_input[str_ctr] == '|') // go to pipe_2 function.
		{
				if(usr_input == "|")
				{
					goto beginning;
				}
				vector<string> char_array;
				vector<string> pipe_array;
				char* token1;
				token1 = strtok(&usr_input[0], "|");
				while(token1 != NULL)
				{
					pipe_array.push_back(token1);
					token1 = strtok(NULL, "|");
				}
					int i = pipe_array.size();
					int c = 0;
					pipe_2(pipe_array,c,i);	
					pipe_array.clear();
					goto end_of_while;
		}
		}
		for(int w = 0; w < trav_str; ++w) // this for loop will check if user inputted i/o redirection.
		{
			if(usr_input[w] == '>' || usr_input[w] == '<' )
			{
				input_output(usr_input);
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
			if(strcmp(argv[0],"cd")==0)
			{
				change_dir(argv);
				for(n = 0; n <= i-1; ++n)
				{
					delete[] argv[n];
				}
				goto end_of_while;
			}
			else
			{
			pid = fork();//will fork program and let child run execvp and then waits for child process to tell parent it is done.
			if(pid == -1){
				perror("There was error with fork().");
				exit(1);}
			else if(pid == 0)
			{
				run_command(argv);
			}		
			else if(pid > 0){
			}	
			}
			if(-1 == wait(0))
			{
				perror("There was an error with wait().");
			}
			for(n = 0; n <= i-1; ++n)
			{
				delete[] argv[n];
			}
			
			char_array.clear();
			pars_array.clear();
			}
	end_of_while://this will go back to the beginning (while(!=exit)). It will also clear char_array and pars_array.
	if(char_array.size() > 0){
	char_array.clear();}
	if(pars_array.size() > 0){
	pars_array.clear();}	
	sig_count = 0;
}


}






int main()
{


	signal(SIGINT,killsignal);
	commands();
	return 0;
}
