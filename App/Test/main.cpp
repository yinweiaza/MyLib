#include    "myMath.h"
#include    "mySort.h"
#include	<assert.h>
using namespace lmtc;
int main(int argc, char *argv[])
{
 	std::vector<int> seq;
 	std::vector<double> val;
 	for( int i = 0 ; i < 100 ; i++ )
 	{
 		seq.push_back(i);
 		double randomVal = averageRandom(0,100);
 		val.push_back(randomVal);
 	}
 	if( !sort(seq,val) ) 
 	{
 		assert(0);
 		return -1;
 	}
	
	return 0;
}
