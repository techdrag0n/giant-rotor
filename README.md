# cpu giant Rotor

This is a modified version of a reppoitory that no longer exists


todo: 
 
** remove checking/creating giant_START.bat

** cleanup /remove
	extra variables being passed ie compressed, xvalue, threads? 
	more possible junk variables ??? giantup, compMode, maxFound, nbit2, next, zet, ;

	
	
	unroll/ inline funtions??? 

 
// DONE 5/14/25 
// ***   will need to change to comparing y point
void Rotor::checkSingleXPoint(bool compressed, Int key, int i, Point p1)

// DONE 5/20/25


 ** change DEBUG to 0
	this will change number of loops to 1024
	remove pt.x from output file.
**  change comparison to use filter  &  filter2 

** set startpoint, jumpsize, and filter from CLI
	startpoint = myx, myy
	jumpsize = myblocksize ??
	
	** cleanup /remove
	timing ifs
	