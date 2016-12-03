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
	
	
	
