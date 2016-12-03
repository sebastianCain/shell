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

	
	
