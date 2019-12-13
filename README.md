# Cheaters
//READ_ME.txt - Savannah Schmidt, SLS6425

Extract zip file into directory. Command "make". May need to module load gcc first 
before compiling. Creates executable: plagiarismCatcher.

Execute as defined in assignment protocol:

./plagiarismCatcher path/to/file (n-gram size) (threshold number of shared n-grams)

ex:  ./plagiarismCatcher path/to/file 6 300

The above example will pull the files out of the given directory and report back, in
descending order, the number of collisions between pairs of files for file pairs with 
at least 300 shared 6-word sequences.

A word sequence is counted every time it is detected in either document. The final 
numeric output of this program is the product of the word sequence occurences in each
document. The higher your desired n-gram and theshold size, the lower the number of 
document pairs will be output (and inversely with lower n-gram and threshold).

This program runs with both the small and medium data sets, but cannot handle the big set.

