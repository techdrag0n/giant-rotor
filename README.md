# cpu giant Rotor

This is a modified version of a reppoitory that no longer exists


this takes a public key from the Secp256k1 eliptical curve and adds points to it while looking for a qualifing point to save in the database. 
 currently this is done single thread on the CPU. once a point is found it jumps by a predeturmined amount myblocksize. if thisisbeing used for
 babysetpgiantstep searching this should be the same size as the block of babystep.  The babystep part is split into two solutions.
 CUbabystep and CLbabystep, CU is a CUDA version and wont work on anything other than NVIDIA GPUs. CL is opencl and will work on more devices.variables.
 







#  todo: 
 
 ** add myblocksize to CLI
 ** add filter to CLI
 
 ** log starttime, end time
** remove checking/creating giant_START.bat

** cleanup /remove
	extra variables being passed ie compressed, xvalue, threads? 
	more possible junk variables ??? giantup, compMode, maxFound, nbit2, next, zet, ;

	
	
	unroll/ inline funtions??? 

 
#  // DONE 5/14/25 
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
	