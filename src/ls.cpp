#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdio>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <sys/sysmacros.h>

#define GREEN	 "\033[1;32m"
#define CYAN 	 "\033[1;36m"
#define BLUE	 "\033[1;34m"
#define GREYBG	 "\033[100m"
#define RESET	 "\033[0m"

using namespace std;


bool compareNoCase (string first, string second)
{
	unsigned long i=0;
	if(first[0] == '.')
	{
		first.erase(0,1);
	}
	if(second[0] == '.')
	{
		second.erase(0,1);
	}
  	while ((i < first.length()) && (i < second.length()))
	{
		if(first[i] == '.')
		{
			first.erase(i,1);
		}
		if(second[i] == '.')
		{
			second.erase(i,1);
		}
	   if (tolower (first[i]) > tolower (second[i])) return true;
	   else if (tolower (first[i]) < tolower (second[i])) return false;
	    i++;
   }
						  
	if (first.length() > second.length()) return true;
	else return false;
}							


void print_l(struct stat s)
{
	if(s.st_mode & S_IFDIR)
	{
		cout << 'd';
	}
	else if(S_ISLNK(s.st_mode))
	{
		cout << 'l';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IRUSR)
	{
		cout << 'r';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IWUSR)
	{
		cout << 'w';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IXUSR)
	{
		cout << 'x';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IRGRP)
	{
		cout << 'r';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IWGRP)
	{
		cout << 'w';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IXGRP)
	{
		cout << 'x';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IROTH)
	{
		cout << 'r';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IWOTH)
	{
		cout << 'w';
	}
	else
	{
		cout << '-';
	}
	if(s.st_mode & S_IXOTH)
	{
		cout << 'x';
	}
	else
	{
		cout << '-';
	}
	cout << " ";
	cout << right << setw(3)<< s.st_nlink << " ";

	struct passwd *pwd;
	if((pwd = getpwuid(s.st_uid)) == NULL)
	{
		perror("getpwuid");
		exit(0);
	}
	else
	{
		cout << pwd->pw_name << " ";
	}
			

	struct group *group;
	if((group = getgrgid(s.st_gid)) == NULL)
	{
		perror("getgrgid");
		exit(0);
	}
	else
	{
	  cout << group->gr_name << " ";
	}
			
				
	cout << right << setw(5) << s.st_size << " ";
		
	char buf[30];
	struct tm *time;
	time = localtime (&(s.st_mtime));
	if(strftime(buf,sizeof(buf),"%b %d %H:%M",time) == 0)
	{
		perror("strftime()");
	}
	cout << buf << " ";

}


void print_dir(vector<string>args,vector<string>directories)
{
	vector<string> directory;
	//vector used for -R flag
	vector<string> temp2;
	//this will check if there's one directory or 2. (for output purpose)
	int check_first = 0;
	if(directories.size() == 1)
	{
		check_first = 0;
	}
	else
	{
		check_first ++;
	}


	int a = 0;
	int l = 0;
	int R = 0;

	//set a,l,R flags for future ifstatements.
	for(unsigned int i = 0; i < args.size();i++)
	{
		if(args[i]=="l")
		{
			l = 1;
		}
		if(args[i]=="a")
		{
			a = 1;
		}
		if(args[i]=="R")
		{
			R = 1;
		 }
	}
	if(R == 1)
	{
		check_first ++;
	}
	
	//this is the loop to go through each directory.
	while(!directories.empty())
	{
		//this is the loop for where argument passed through was not a directory.
		string currentdir = directories.back();
		struct stat s;
		if(lstat(currentdir.c_str(),&s)<0)
		{
			perror("Stat");
			exit(0);
		}
		if(S_ISREG(s.st_mode) || S_ISLNK(s.st_mode))
		{

			if(l == 1)
			{
				//flag is set to l so it'll print out the long form with color according to file.
				print_l(s);
				if(S_ISLNK(s.st_mode))
				{
					cout << CYAN << currentdir << RESET << endl;
				}
				else if(s.st_mode & S_IXUSR || s.st_mode & S_IXOTH || s.st_mode & S_IXGRP)
				{
					cout << GREEN << currentdir << RESET << endl;
				}
				else if(S_ISDIR(s.st_mode))
				{
					if(currentdir.at(0) == '.')
					{
						cout <<BLUE << GREYBG << currentdir  << RESET << endl;
					}
					cout << BLUE << currentdir << RESET << endl;
				}
				else
				{
					cout << currentdir << endl;
				}
				directories.pop_back();				
			}//this is when it's not long format.
			else if(l == 0 &&  (S_ISREG(s.st_mode) || S_ISLNK(s.st_mode)) )
			{
						if(S_ISLNK(s.st_mode))
						{
							cout <<  CYAN << currentdir << RESET << endl ;	
						}
						else if( !(s.st_mode & S_IFDIR) && ((s.st_mode & S_IXUSR) | (s.st_mode & S_IXOTH) | (s.st_mode & S_IXGRP)))
						{
							cout << GREEN << currentdir << RESET << endl ;
						}
						else
						{
							cout << currentdir << endl;
						}
				directories.pop_back();
			}
		}
		else	//this else is where the directories gets printed according to the flag.
		{
			vector<string> temp;
			int counter = 0;
			const char *dirName = directories.back().c_str();
			DIR * dirp;
			dirent *direntp;
			if(check_first == 0)
			{
			 check_first ++;
			}
			else
			{
			cout << currentdir << ":"  << endl;	
			}
			if(!(dirp = opendir(dirName)))
			{
				perror("opendir");
				exit(0);
			}
			//reads directory.
			if(!(direntp = readdir(dirp)))
			{
				perror("reddir");
				exit(0);
			}
			do
			{
				temp.push_back(direntp->d_name);
			}while((direntp = readdir(dirp)));
			// checks to see if . files needs to be outputted with -a flag
			sort(temp.begin(),temp.end(),compareNoCase);	
			int total_size = 0;
			struct stat total;
			//this will print out the "total" for the -l flag.
			for(unsigned int n = 0; n < temp.size(); ++n)
			{
				string temp_path = currentdir;
				temp_path.append("/");
				temp_path.append(temp[n]);
				if(lstat(temp_path.c_str(),&total) < 0)
				{
					perror("stat");
					exit(0);
				}
				if(a == 0 && temp[n].at(0) == '.')
				{
				}
				else if(S_ISLNK(total.st_size))
				{

				}
				else
				{
				total_size = total_size + total.st_blocks;
				}
			}
			total_size = total_size/2;
			if(R == 0)
			{
				sort(temp.begin(),temp.end(),compareNoCase);	
			}

			if(l == 1)
			{
				cout <<  "total " << total_size << endl;
			}
			//temp will contain the files/directories that was inside directories.
			while(!temp.empty())
			{
				//this will add path necessary for outputting other subdirectories.
				string path = currentdir;
				path.append("/");
				path.append(temp.back());
				//this is for when -a flag is not set it would not pass in the hidden files.
				if(a == 0 && temp.back().at(0) == '.')
				{
					temp.pop_back();
					continue;
				}
				
				struct stat t;
				if(lstat(path.c_str(),&t) < 0)
				{
					perror("stat");
					exit(0);
				}
				//checks for -R flag
				if( R == 1 )		
				{
					if(temp.back().length() == 1 && temp.back().at(0) == '.')
					{
						goto rest_of_loop;
					}
					if(temp.back().at(0) == '.' && temp.back().at(1) == '.')
					{
						goto rest_of_loop;
						
					}
					if(S_ISDIR(t.st_mode))
					{
					//if path contained a directory and -R flag was set, it will go in temp2 to be called in print_dir after it outputs the currentdirectory.
					temp2.insert(temp2.begin(),path);
					}
				}
				rest_of_loop:
				//-l flag output.
				if(l == 1)
				{
					
					print_l(t);
					
					if(S_ISLNK(t.st_mode))
					{
						cout << CYAN << temp.back() << RESET << endl;	
					}
					else if( !(t.st_mode & S_IFDIR) && ((t.st_mode & S_IXUSR) | (t.st_mode & S_IXOTH) | (t.st_mode & S_IXGRP)))
					{
						cout << GREEN << temp.back() << RESET << endl;
					}
					else if(S_ISDIR(t.st_mode))
					{
						if(temp.back().at(0) == '.')
						{
							cout <<BLUE << GREYBG << temp.back()  << RESET << endl;
						}
						else
						{
						cout << BLUE << temp.back() << RESET << endl;
						}
					}
					else if(temp.back().at(0) == '.')
					{
						cout << GREYBG << temp.back() << RESET << endl;
					}
					else
					{
						cout << temp.back() << endl;
					}
				}
				else
				{
					//this is for -a flag and no flag. outputs with 4 columns
					if(counter == 4)
					{
						cout << endl;
						counter = 0;
					
					}
					{
						counter++;
						if(S_ISLNK(t.st_mode))
						{
							cout <<  CYAN <<  left << setw(20) << temp.back() ;	
						}
						else if( !(t.st_mode & S_IFDIR) && ((t.st_mode & S_IXUSR) | (t.st_mode & S_IXOTH) | (t.st_mode & S_IXGRP)))
						{
							string temporary = GREEN + temp.back();
							cout << GREEN << left << setw(20) << temp.back() ;
						}
						else if(S_ISDIR(t.st_mode))
						{
							if(temp.back().at(0) == '.')
							{
							cout << GREYBG;
							cout << BLUE;
							cout << left <<  setw(20) << temp.back() ;
							cout << RESET;
							}
							else
							{
							cout << BLUE;
							cout << left << setw(20) << temp.back();
							}
						}
						else if(temp.back().at(0) == '.')
						{
							cout << GREYBG << left << setw(20)  << temp.back();
							cout << RESET;
						}
						else
						{
							cout << left <<  setw(20) << temp.back() ;
						}
						cout << RESET;
					}
				}
		
			temp.pop_back();
			}
			
			cout << endl << endl;
			closedir(dirp);
			directories.pop_back();
			if(temp2.size() != 0)
			{
				print_dir(args,temp2);
				temp2.clear();	
				//if directories is empty, it will just end print_dir
				if(directories.empty())
				{
					return;
				}
			}
		}
	}

}





int main(int argc, char* argv[])
{
	vector<string> arg_temp;
	vector<string> args;
	vector<string> directories;

	for(int i = 0 ; i < argc ; i++){
	arg_temp.push_back(argv[i]);
	}

	
	//get arguments
	for(unsigned int i = 1 ; i < arg_temp.size(); i++)
	{
		if(arg_temp[i].at(0) == '-')
		{
			for(unsigned int n = 1; n < arg_temp[i].size(); ++n)
			{	
				string t(1,arg_temp[i][n]);
				args.push_back(t);
			}
		}	
	}
	
	//get directory
	for(unsigned int i = 1 ; i < arg_temp.size(); i++)
	{
		if(arg_temp[i].at(0) != '-')
		{
			string t = arg_temp[i];
			directories.push_back(t);
		}
		
	}
	//Add "." as directory if directory is empty
	if(directories.size() ==0)
	{
		directories.push_back(".");
	}
	//goes to function print_dir and prints the directories passed in.
	print_dir(args,directories);


return 0;
}
