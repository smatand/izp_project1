/*
 * @name Projekt 1 - Overovanie sily hesiel (Praca s textom)
 * @author Andrej Smatana <xsmata03@stud.fit.vutbr.cz>
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CHARS 100

struct stats_t
{
    int unique[128];
    int sumOfUniq;
    int total;
    int count;
    size_t min;
};

/*
 * function used primarily to directly save statistics data of an entered password
 */
struct stats_t storeData(struct stats_t stats_t, const char* str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        // set the position corresponding to the ASCII value of str[i] in int array to 1
        stats_t.unique[(size_t)str[i]] = 1; // use the recasted char value to index in int array
    }

    stats_t.total += len; // adds length of password to total for calculating the average length of passwords
    stats_t.count++; // increments count by every password for calculating the average length of passwords

    // compares length of current password and the last found password with minimum chars
    if (len < stats_t.min) {
        stats_t.min = len; // sets new minimum value
    }

    return stats_t;
}

/*
 * my own implementation function similar to strlen() from <string.h>
 */
size_t my_strlen(const char* str)  {
    int sum = 0;
    while (*str++)  {
        sum++;
    }

    return sum;
}

/*
 * function checks whether the 3rd argument is of length 7 because the word "--stats" has 7 characters
 * if it proceeds, then the function checks whether the word is same as "--stats"
 * if the 3rd argument has unexpected letters, function returns false and the whole program returns error
 * because of invalid input
 */
bool checkStats(const char* str, const char* stats)    {
    if (my_strlen(str) != 7)    {
        return false;
    }

    for (size_t i = 0; i < 7; i++)  {
        if (str[i] == stats[i]) {
            continue;
        } else  {
            return false;
        }
    }

    return true;
}

/*
 * prints stats from given parameter of struct stats_t (--stats argument)
 * function includes counter of unique characters
 */
void printStats(struct stats_t stats_t)    {
    for (size_t i = 0; i < 128; i++) {
        stats_t.sumOfUniq += stats_t.unique[i];
    }

    printf("Statistika:\n"
           "Ruznych znaku: %i\n"
           "Minimalni delka: %li\n"
           "Prumerna delka: %.1f\n",
           stats_t.sumOfUniq, stats_t.min, (stats_t.total / (float)stats_t.count));
}

/*
 * function for checking the length of str
 * the program expects newline character in str
 * if there is no newline character in given str, the str has probably more chars than allowed
 */
bool findNewLine(const char* str)   {
    for (size_t i = 0; str[i] != '\0'; i++)  {
        if (str[i] == '\n') {
            return true;
        }
        // for situations when the newline character is on position str[MAX_CHARS+1]
        if (str[i] == '\0')   {
            return true;
        }
    }

    // if it returns false, the program executes error
    return false;
}

/*
 * prints error to stderr based on given parameter
 */
void printError(int arg)    {
    switch (arg)    {
        case 1:
            fprintf(stderr, "ERROR: Invalid argument. Run the program with ./pwcheck LEVEL PARAM [--stats].\n");
            break;
        case 2:
            fprintf(stderr, "ERROR: Program stopped working because of the password longer than %i characters.\n"
                    , MAX_CHARS);
            break;
        case 3:
            fprintf(stderr, "ERROR: There is only 1 optional argument (--stats).\n");
        default:
            break;
    }
}

/*
 * searches for upper and lower cases in the password
 * returns true if both bool variables will equal to true
 * otherwise it returns false
 */
bool level_1(const char* str)    {
    bool upper=false, lower=false;

    // iterating through each case in char array
    for (size_t i=0; str[i] != '\0'; i++)   {
        if ((str[i] >= 'A' && str[i] <= 'Z'))  {
            upper = true;
        }   else if (str[i] >= 'a' && str[i] <= 'z') {
            lower = true;
        }

        if (upper == true && lower == true) {
            return true;
        }
    }

    return false;
}

/*
 * subfunction of function level_2, which searches for numbers in the password
 * returns true immediately after 1st occurence of the number
 */
bool level_2_3(const char* str)  {
    for (int i=0; str[i] != '\0'; i++)   {

        if ((str[i] >= '0' && str[i] <= '9'))   {
            return true;
        }
    }
    return false;
}

/*
 * subfunction of function level_2, which searches for special characters in the password
 * returns true immediately after 1st occurence of the special character
 */
bool level_2_4(const char* str) {
    for (int i=0; str[i] != '\0'; i++)   {
        // comparing char with values of special characters, which holds int values
        if (str[i] >= ' ' && str[i] <= '/') {
            return true;
        }
        else if (str[i] >= ':' && str[i] <= '@') {
            return true;
        }
        else if (str[i] >= '[' && str[i] <= '`') {
            return true;
        }
        else if (str[i] >= '{' && str[i] <= '~') {
            return true;
        }
    }

    return false;
}

/*
 * function with subfunctions level_2_3 & level_2_4
 * function doesn't check for upper and lower letter (1st and 2nd group) , because
 * the password is already checked for those cases in level_1
 * if the param is larger than 4, the function still counts it as param = 4
 */
bool level_2(char* str, const size_t param)    {
    if (param == 3) {
        return (level_2_3(str));
    } else if (param >= 4) {
        return (level_2_3(str) && level_2_4(str));
    }

    return true;
}

/*
 * filters whether the password has any repeating characters in row of length -par
 * returns false if it has found any repeating characters
 */
bool level_3(const char* str, const int par)  {
    int count = 1;

    // it could be implemented in 2 for loops, but this is good enough and it works
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (count == par) {
            return false;
        }
        if (str[i] == str[i + 1]) {
            count++;
        }   else    {
            count = 1;
        }
    }

    // if it didn't find anything, continue on next level
    return true;
}

/*
 * subfunction of level_4
 * replaces the -substr argument with -substr of length -par from -str
 */
void my_substr(const char* str, char* substr, const size_t par, size_t index)   {
    for (size_t i = 0; i < par; i++, index++) {
        substr[i] = str[index];
    }
}

/*
 * function with subfunction my_substr
 * searches for same sequences of length -par
 * if it finds any other sequence, it returns true
 */
int level_4(const char *str, size_t par) {
    // if the param > length of str, then we can't create a substring with length of param and the function returns false as it
    // didn't find any repeating substring
    size_t len = my_strlen(str);
    if (par > len)  {
        return false;
    }

    char substr[par];

    for (size_t i = 0; i < (len - par); i++) {
        my_substr(str, substr, par, i); // change the value of substr by -par characters from str
        size_t temp = i+1; // iterator, which starts from the next character
        size_t substrIndex = 0; // same as count variable, but it's indexing through -substr

        // comparing each character of str with character of substr
        while (str[temp] != '\0') {
            // returns true if any of the same substring was found
            if (substrIndex == par) {
                return true;
            }

            // if it finds any same characters, continue next iteration
            // if it does not, reset the value of substrIndex to 0 and continue
            if (substr[substrIndex] == str[temp]) {
                temp++, substrIndex++; //
                continue;
            } else {
                substrIndex = 0;
                temp++;
                continue;
            }
        }
    }
    // if it returns false, the password should be printed to stdout
    return false;
}

/*
 * subfunction of filter used for returning boolean values
 * -i is used as index of iterator
 */
bool filter_subf(char* str, const int param, size_t i) {
    if (i == 1) {
        return (level_1(str));
    }
    else if (i == 2)    {
        return (level_2(str, param));
    }
    else if (i == 3)    {
        return (level_3(str, param));
    }
    else if (i == 4)    {
        return (!(level_4(str, param)));
    }

    return false;
}

bool filter(char* str, const int lvl, const int param) {
    // temporary variable for whether the password does comply the rules set by user entered args
    bool temp = false;

    // iterating from 1 to user entered level
    for (size_t i = 1; i <= (size_t)lvl; i++) {
        // the main function of password filter
        temp = filter_subf(str, param, i);

        if (!temp) {
            return false;
        }
    }

    return true;
}

/*
 * as the assignment says, the program is always executed with 2 entered arguments of lvl, param and
 * an only optional argument --stats
 * main function includes checking for correctly typed arguments (program expects only numbers and --stats)
 */
int main(int argc, char** argv)    {
    // ./pwcheck LEVEL PARAM [--stats] - 4 arguments, first three are necessary, [--stats] is optional
    if (argc <3 || argc>4)  {
        printError(1);
        return EXIT_FAILURE;
    }

    char *endptr;
    const int N_LVL = strtol(argv[1], &endptr, 10);
    // if something is in endptr, the program ends with error message of invalid input
    // if the entered level is out of range [1, 4], the program ends with error message of invalid input
    if (*endptr || (N_LVL > 4 || N_LVL < 1))    {
        printError(1);
        return EXIT_FAILURE;
    }

    const int N_PARAM = strtol(argv[2], &endptr, 10);
    // the same situation as a few lines above
    if (*endptr || N_PARAM == 0) {
        printError(1);
        return EXIT_FAILURE;
    }

    // creates a struct which holds the values for stats
    struct stats_t stats_t = {{0}, 0, 0, 0, MAX_CHARS};

    bool stats = false; // holds the value for the --stats argument
    // if the user entered anything in 3rd argument, the program expects "--stats"
    // if the argument was typed incorrectly, the program stops working
    if (argv[3] != NULL) {
        if (checkStats(argv[3], "--stats"))    {
            stats = true;
        }   else {
            printError(3);
            return EXIT_FAILURE;
        }
    }

    char pw[MAX_CHARS+2];   //  +2 because of '\n' and '\0'

    while (fgets(pw, sizeof pw, stdin) != NULL)   {
        // get length of str and subtract it by 1, because the program isn't set to count the '/n' char
        size_t length = my_strlen(pw)-1;

        // checks whether the password is longer than allowed
        if (!findNewLine(pw))    {
            printError(2);
            return EXIT_FAILURE;
        }

        // if the --stats argument is true, store the data in struct stats_t
        if (stats) {
            stats_t = storeData(stats_t, pw, length);
        }

        // the main part of program which prints password if it complies the rules
        if (filter(pw, N_LVL, N_PARAM)) {
            printf("%s", pw);
        }
    }

    //  print stats only if the argv[3] was checked true for "--stats" arg.
    if (stats)  {
        printStats(stats_t);
    }

    return EXIT_SUCCESS;
}
