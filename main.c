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
    int min;
};

/*
 * function used primarily to directly save statistics data of password
 */
struct stats_t storeData(struct stats_t stats_t, char* str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        // set the position corresponding to the ASCII value of str[i] in int array to 1
        stats_t.unique[(size_t)str[i]] = 1; // use the recasted char value to index in int array
    }

    stats_t.total += len; // adds length of password to total for calculating the average length of passwords
    stats_t.count++; // increments count by every password for calculating the average length of passwords

    // compares length of current password and the last found password with minimum chars
    if ((int)len < stats_t.min) {
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

    //  function counts also the '/n' character
    return sum;
}

/*
 * replace the -substr argument with -substr of length -par from -str
 */
void my_substr(const char* str, char* substr, const size_t par, size_t index)   {
    for (size_t i = 0; i < par; i++, index++) {
        substr[i] = str[index];
    }
}

/*
 * prints stats from given parameter of struct stats_t (--stats argument)
 * a counter of unique characters is also included in
 */
void printStats(struct stats_t stats_t)    {
    for (size_t i = 0; i < 128; i++) {
        stats_t.sumOfUniq += stats_t.unique[i];
    }

    printf("Statistika:\n"
           "Ruznych znaku: %i\n"
           "Minimalni delka: %i\n"
           "Prumerna delka: %.1f\n",
           stats_t.sumOfUniq, stats_t.min, (stats_t.total / (float)stats_t.count));
}

/*
 * prints error to stderr based on given parameter
 */
void printError(int arg)    {
    switch (arg)    {
        case 1:
            fprintf(stderr, "ERROR: Invalid argument. Run the program with ./pwcheck LEVEL PARAM [--stats].\n"
                            "LEVEL must be of range [1, 4], PARAM must be any positive number [1, inf.].");
            break;
        case 2:
            fprintf(stderr, "ERROR: Program stopped working because of the password longer than %i characters.\n", MAX_CHARS);
            break;
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

    // todo for (int i = 0; str[i] <= '\0'; i++)
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
 * TODO remove parameter len and change for loop "str[i] != '\0'"
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
 * TODO remove parameter len and change for loop "str[i] != '\0'"
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
   the password is already checked for those cases in level_1
 * @param is always positive number of range [1, inf>
 * if the param is larger than 4, the function still counts it as param = 4
 * TODO remove size_t len argument
 */
bool level_2(char* str, const size_t param)    {
    if (param == 3) {
        return (level_2_3(str));
    } else if (param >= 4) {
        return (level_2_3(str) && level_2_4(str));
    }

    return true;
}

//  FILTER Repeating chars in row
//  If the filter finds any same characters of length N_PAR, then it returns false
/*
 *
 */
bool level_3(const char* str, unsigned int par)  {
    unsigned int count = 1;
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

    return true;
}

int level_4(const char *str, size_t par) {
    // if the param > length of str, then we can't create a substring with length of param and the function returns false as it
    // didn't find any repeating substring
    size_t len = my_strlen(str) - 1;    // todo why there is -1?

    if (par > len)  {
        return false;
    }

    char substr[par];

    for (size_t i = 0; i < (len - par); i++) {
        my_substr(str, substr, par, i);
        size_t temp = i+1, substrIndex = 0;

        while (str[temp] != '\0') {
            if (substrIndex == par) {
                return true;
            }

            if (substr[substrIndex] == str[temp]) {
                temp++, substrIndex++;
                continue;
            } else {
                substrIndex = 0;
                temp++;
                continue;
            }
        }
    }

    return false;
}

bool filter1(char* str, unsigned int param, size_t i) {
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


/*
 * function checks whether the 3rd argument is of length 7 as the word "--stats" has 7 characters
 * if it proceeds, then the function checks whether the word really equalts to "--stats"
 * if the 3rd argument has unexpected letters, function returns false and the whole program returns error
   of invalid input
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
 * unnecessary part of code, which checks whether the inputted arguments are valid
 * the project assignment tells about executing program with fixed input arguments where level is in
   range [1, 4] and param as any positive number
 * program returns true if any of the invalid data is found
 */
bool checkValidArgs(int argc, char** argv, int* n_lvl, int* n_param)  {
    // maximum amount of args: 4 (./pwcheck lvl param [--stats])
    if ((argc < 2) || (argc > 4))   {
        printError(1);
        return true;
    }

    // get entered arguments and recast them to int, so we can use them later
    char* endptr; // temp variable to identificate any letters in arguments

    n_lvl = strtol(argv[1], &endptr, 10);

    // if there is unwanted chars in endptr, end the program with error msg
    // if the level is out of range, end the program with error msg
    if (*endptr || (n_lvl > 4 || n_lvl < 1))    {
        printError(1);
        return true;
    }

    n_param = strtol(argv[2], &endptr, 10);
    if (*endptr || n_param == 0) {
        printError(1);
        return true;
    }

}

/*
 * as the assignment says, the program is always ran with 2 entered arguments of lvl, param + opt. [--stats]
 * it includes checking for invalid input such as unexpected characters in arguments (lvl, param don't have
   any letters, [--stats] must have only those specific characters)
 *
 */
int main(int argc, char** argv)    {
    int n_lvl = 0, n_param = 0;

    // not needed for project, but I am using it for my own needs
    if(checkValidArgs(argc, argv, &n_lvl, &n_param))  {
        return EXIT_FAILURE;
    }

    // holds the value for the [--stats] argument
    bool stats = false;
    // if the user entered anything in 3rd argument, the program expects that it holds word "--stats"
    if (argv[3] != NULL) {
        if (checkStats(argv[3], "--stats"))    {
            stats = true;
    }   else {
            printError(1);
            return EXIT_FAILURE;
        }
    }

    // create a struct which holds the values for stats
    struct stats_t stats_t = {{0}, 0, 0, 0, MAX_CHARS};

    char pw[MAX_CHARS+1];   //  +1 because of '\0'

    while (fgets(pw, sizeof pw, stdin) != NULL)   {
        // get length of str and subtract it by 1, because the program isn't set to count the '/n' char
        unsigned int length = (my_strlen(pw)-1);

        if (my_strlen(pw) >= MAX_CHARS) {
            printError(2);
            return EXIT_FAILURE;
        }

        if (length > MAX_CHARS) {
            printError(2);
            return EXIT_FAILURE;
        }

        // store data in struct of stats even if the bool stats is set to false
        stats_t = storeData(stats_t, pw, length);

        // temporary variable for whether the password does comply the rules set by user entered arg.
        bool temp = false;
        // iterating from 1 to user entered level
        for (size_t i = 1; i <= N_LVL; i++) {
            // the main function of password filter
            temp = filter1(pw, N_PAR, i);

            // todo why is there !temp?
            if (!temp)  {
                break;
            }
        }

        if (temp)
            printf("%s", pw);
    }

    //  print stats only if the argv[3] was checked true for "--stats" arg.
    if (stats)  {
        printStats(stats_t);
    }

    return EXIT_SUCCESS;
}
