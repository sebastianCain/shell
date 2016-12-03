FRB-SC SHELL
by Felix Rieg-Baumhauer and Sebastian Cain

Features:
	Executes simple commands, ie (ls or ls -l), done by forking,
	Executes commands even if there is unforseen whitespace, ie (ls    -l)
	Executes cd and exit, done without forking
	Execues multiple commands on one line, separted by ";",  ie (ls ; ls -l)
	Executes simple redirection, one set of redirects only
		-stdin <
		-stout >
		-stdout append >>
	Executes simple pipes, ie (ls | wc), one set of pipes only
	Has a "descriptive" prompt that describes to the user in which approximate directory they are
	
	
Attempted:
	Complex redirection
	Compex pipes-the surrent pipe structure that we use is not well suited to being used recursivelly
	Attempted to show the username of the user in the prompt, before hostname, this did not work. 
	
	
Bugs:


Files & Function Headers:

------------------------------------------------------------

main.c
	-only runs executor.c

------------------------------------------------------------

stringext.h:

	char * promptString();
	     
	     -creates the prompt
	     -returns a malloced piece of memory

	int split_semis(char dest[], char * command[]);

	    -splits command along the semicolons
	    
	void split_spaces(char dest[], char * local_command[]);

	     -splits local_command along spaces
	     -the recursib=velly goes through local_command, if any pointer poits to an empty string "", which can occur if two spces next to each other are parsed, the "" is overwritten by the next element
	     
	int find(char * haystack[], char * needle);

	    -finds needl in haystack,
	    -return the positon if sucesful,
	    -returns -1 if not found

	int repackage(char * dest[], char * src[], int n);

	    -repackages n contents from src to dest,
	    -dest is usually an empty array

---------------------------------------------------------------------

executor.h:

	int std_out(char * command[], int pos);
	
	    -Does the std_out redirecting, ">" which over writes the file in question
	    -removes the file if it exists
	    -opens the file with approriate tags and permissions
	    -uses dup and sup2 to get the file in spot 1, where STD_OUT is normally
	    -forks of a proccess,
	    -in the child, the command is executed
	    -in the parent, the file table is reverted to its old state, file closed


	int std_out_append(char * command[], int pos);
	
	    	    -Does the std_OUT redirecting, ">>", but with appending, not overwritting 
	    -opens the file with approriate tags and permissions
	    -uses dup and sup2 to get the file in spot 1, where STD_OUT is normally
	    -forks of a proccess,
	    -in the child, the command is executed
	    -in the parent, the file table is reverted to its old state, file closed
	int std_in(char * command[], int pos);
	
	    -Does the std_in redirecting, "<" 
	    -opens the file with approriate tags and permissions
	    -uses dup and sup2 to get the file in spot 0, where STD_IN is normally
	    -forks of a proccess,
	    -in the child, the command is executed
	    -in the parent, the file table is reverted to its old state, file closed

	void mykill();

	     -our own kill function
	     -called when we want to use exit
	     -uses kill(), flag 9, to end the process
	     
	int mypipe(char * command[], int pos);

	    -our own pipe function, "|", can only handle simple pipes
	    -uses pipe() function
	    -by using dup and dup2, along with the pipe() function and the array that it returns, STD_OUT goes to STD_IN,
	    -fork once, run the process
	    -reset  STD_OUT 
	    -fork again, run the other process,
	    -revert file table to nomral
	    
	int exec();
	    -the file that runs everything
	    -starts by entering a forever while loop, while(1)
	    -prints the prompt
	    -takes in inout with fgets
	    -splits the resulting array along semicolons
	    -uses a while loop to "run" each command that is seperated by a semicolon. ie if ls ; ls -l is inputed, ls is first run through the while loop, then ls -l is.
	    -IN THE WHILE LOOP
	    -the string is split along spaces
	    -fork of a child

	    -CHILD
	    -test for empty command, ie only spaces or only retun was entered
	    -test for exit, call mykil()
	    -test for cd, exit loop, this is dealt with in the parent, not child
	    -test >, <, >>, call appropriate functions
	    -test | call mypipe,
	    -END OF CHILD

	    -PARENT
	    -test for cd, use chdir to change directory
	    -END OF PARENT
	
	
	
