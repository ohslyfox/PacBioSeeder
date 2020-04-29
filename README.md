# PacBioSeeder
Sims can be found here:
https://drive.google.com/open?id=1ndBwZc6wXOE6ee5vfyamLkHmLs-ZUJxB

To run on the command line:

```./pbseed --run-type [runtype] --scheme [scheme] --file-name [filename] --kmer-length [length]```

runtype:
	* "execute" - simply execute the scheme and output the result
	* "compare" - execute the scheme and compare true solution

scheme:
	* "max-group" - the max group scheme
	* "min-score" - the min score scheme
	* "min-frame" - the min frame scheme

filename:
	* file name or path without an extension

length:
	* length of k-mers to feed to the scheme
	