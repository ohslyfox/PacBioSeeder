# pbseed
Test data and results can be found here:
https://drive.google.com/open?id=1ndBwZc6wXOE6ee5vfyamLkHmLs-ZUJxB

To run on the command line:

```./pbseed --run-type [runtype] --scheme [scheme] --kmer-length [length] --reads [readamount] --file-name [filename] --output [outfilename]```

runtype:
	
* "execute" - simply execute the scheme and output the result
* "compare" - execute the scheme and compare results to true solution

scheme:

* "max-group" - the max group scheme
* "min-score" - the min score scheme
* "min-frame" - the min frame scheme

length:

* length of k-mers to feed to the scheme

readamount:

* number of pacbio reads to test, 0 for all

filename:

* file name or path without an extension

outfilename:

* output file name without an extension (exclude this option or use "console" for console output).
	
