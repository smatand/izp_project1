### IZP - Project 1 (Password filter)

[Project assignment (in CZ)](https://github.com/smatand/izp1_projekt/blob/master/izp_projekt1.pdf)

My very first school project created in language C (without bonus assignment). 

Compile `gcc -std=c99 -Wall -Wextra -Werror pwcheck.c -o pwcheck`.
Run the program `./pwcheck LEVEL PARAM [--stats]`.

Run the tests `./test.py pwcheck`. Credits goes to [Josef Kuchar](https://github.com/JosefKuchar)

**Result:** 9.6/10  
The problem was with empty input, where the `--stats` were incorrectly calculated. 
