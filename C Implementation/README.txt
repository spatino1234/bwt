bwt.c README Document

Author: Jack Owen

Contributors:
Shreya S. Nair
AJ Ristino
Serafín Patiño III
Phi Rapacz
Zev Goldhaber-Gordon

How to use:
   - Execute ./bwt in terminal
   - The program will ask for the name of a .txt file
   - Enter the name of a .txt file in the folder (e.g. genome1.txt)
   - The transform will execute and prompt the user to enter a query for alignment
   - Enter as many queries as desired, they will return the number of matches
   - Enter the query "exit" to exit
   - Enter the query "help" for more options
   	- Enter the query "fastq" then "reads.fastq" to read queries from a fastq file
   	- Enter the query "match" to output the indices where a query aligns
- genome1.txt, genome2.txt, genome3.txt
   - Sample reference genomes to run bwt.c
- reads.fastq
   - Fastq file containing sample reads for bwt.c