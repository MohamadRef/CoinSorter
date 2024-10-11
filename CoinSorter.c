//Mohamad Refaai 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    char name[1024];
    double weight;
    double weightME;
    double diameter;
    double diameterME;
    int rollCapacity;
} CoinType;

int DetermineCoinType(CoinType *CoinTypes, double weight, double diameter, int coinAmount);

int DetermineCoinType(CoinType *CoinTypes, double weight, double diameter, int coinAmount)
{
    double minWeight = 0;
    double maxWeight = 0;
    double minDiameter = 0;
    double maxDiameter = 0;
    for (int i = 0; i < coinAmount; i++)
    { 
        bool weightMatch = false;
        bool diameterMatch = false;

        minWeight = CoinTypes[i].weight - CoinTypes[i].weightME;
        maxWeight = CoinTypes[i].weight + CoinTypes[i].weightME;
        if ((weight >= minWeight) && (weight <= maxWeight))
        {
            weightMatch = true;
        }

        minDiameter = CoinTypes[i].diameter - CoinTypes[i].diameterME;
        maxDiameter = CoinTypes[i].diameter + CoinTypes[i].diameterME;
        if ((diameter >= minDiameter) && (diameter <= maxDiameter))
        {
            diameterMatch = true;
        }

        if ((weightMatch == true) && (diameterMatch == true))
        {

            return i;
        }
    }
    // if no matches are found, must not be within any range, return other
    return 5;
}

int main(void)
{
    FILE *configFilep;
    
    int failCount = 0;
    while (failCount < 3)
    {
        char fileName[1024];
        char buffer[1024];
        printf("Please enter the name of the configuration file: ");
        fgets(buffer, 1024, stdin);
        sscanf(buffer, "%s", &fileName);

        configFilep = fopen(fileName, "r");
        if (configFilep == NULL)
        {
            fprintf(stderr, "ERROR: The opening of the configuration file, %s, failed.\n", fileName);
            failCount += 1;
        }
        else break;
    }

    if (failCount == 3)
    {
        fprintf(stderr, "ERROR: Exceeded the maximum number of tries to open the configuration file\n");
        return 1;
    }

    char buffer[1024] = {0};
    bool digitRead = false;
    int coinAmount = 0;
    while (1)
    {
        if (fgets(buffer, 1024, configFilep) != NULL) // read each line until NULL ptr
        {
            size_t length = strlen(buffer);
            for (int i = 0; i < length; i++)
            {
                char current = buffer[i];
                if (isdigit(current) != 0) // if a digit is read
                {
                    int currentAsInt = current - '0';
                    char nextChar = buffer[i+1];
                    if ((isdigit(nextChar) != 0) || (currentAsInt < 1) || (currentAsInt > 5)) // if the next character is a digit, or the digit is out of the alloted range
                    {
                        printf("ERROR; the value of number of coin types is out of range.\n");
                        fclose(configFilep);
                        return 1;
                    }
                    else // otherwise, return the digit
                    {
                        coinAmount = currentAsInt;
                        digitRead = true;
                        break; // break out of the for loop
                    }
                }
            }
            if (digitRead == true) break; // if a digit has been read stop looping
        }
        else // detect what kind of error
        {
            if (feof(configFilep))
            {
                fprintf(stderr, "ERROR: at end of file, no more data in the file.\n");
                fclose(configFilep);
                return 1;
            }
            else if (ferror(configFilep))
            {
                fprintf(stderr, "ERROR: data corrupt, could not read any digits into the variable\n");
                fclose(configFilep);
                return 1;
            }
        }   
    }

    // this block of code skips all the empty lines
    long int currentPos = 0;
    bool somethingRead = false;
    while (1)
    {
        currentPos = ftell(configFilep);
        if (fgets(buffer, 1024, configFilep) != NULL) // read each line until NULL ptr
        {
            size_t length = strlen(buffer);
            for (int i = 0; i < length; i++)
            {
                if (buffer[i] != ' ') // if non-whitespace detected
                {
                    somethingRead = true;
                    break;
                }
            }
            if (somethingRead == true) break; // if something has been read stop looping
        }
        else // detect what kind of error
        {
            if (feof(configFilep))
            {
                fprintf(stderr, "ERROR: at end of file, no more data in the file.\n");
                fclose(configFilep);
                return 1;
            }
            else if (ferror(configFilep))
            {
                fprintf(stderr, "ERROR: data corrupt, could not read any digits into the variable\n");
                fclose(configFilep);
                return 1;
            }
        }   
    }
    
    // reset file pointer to start of coin lines
    fseek(configFilep, currentPos, SEEK_SET);


    // allocate memory for cointypes
    CoinType *CoinTypes = malloc(coinAmount * sizeof(CoinType));

    int thingsRead = 0;
    for (int i = 0; i < coinAmount; i++) // count how many things are actually read
    {
        thingsRead = fscanf(configFilep, " %s %lf %lf %lf %lf %d", &CoinTypes[i].name,  &CoinTypes[i].weight, &CoinTypes[i].weightME, &CoinTypes[i].diameter, &CoinTypes[i].diameterME, &CoinTypes[i].rollCapacity);
        if (thingsRead == 1) // return errors accordingly
        {
            fprintf(stderr, "ERROR: Could not read the weight of the coin\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if (thingsRead == 2)
        {
            fprintf(stderr, "ERROR: Could not read the variance of the weight of the coin\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if (thingsRead == 3)
        {
            fprintf(stderr, "ERROR: Could not read the diameter of the coin\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if (thingsRead == 4)
        {
            fprintf(stderr, "ERROR: Could not read the variance of the diameter of the coin\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if ((CoinTypes[i].weight <= 0) || (CoinTypes[i].weight > 40))
        {
            fprintf(stderr, "ERROR: Weight of the coin is out of range\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if ((CoinTypes[i].weightME <= 0.1) || (CoinTypes[i].weightME > 4))
        {
            fprintf(stderr, "ERROR: Variance of the weight of the coin is out of range\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if ((CoinTypes[i].diameter <= 0) || (CoinTypes[i].weight > 60))
        {
            fprintf(stderr, "ERROR: Diameter of the coin is out of range\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if ((CoinTypes[i].diameterME <= 0.1) || (CoinTypes[i].diameterME > 3))
        {
            fprintf(stderr, "ERROR: Variance of the diameter of the coin is out of range\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
        if ((CoinTypes[i].rollCapacity < 5) || (CoinTypes[i].rollCapacity > 50))
        {
            fprintf(stderr, "ERROR: Roll capacity is out of range\n");
            fclose(configFilep);
            free(CoinTypes);
            return 1;
        }
    }

    FILE *inputFilep;

    failCount = 0;
    while (failCount < 3)
    {
        char fileName[1024];
        char buffer[1024];
        printf("Type the name of the coin input file: ");
        fgets(buffer, 1024, stdin);
        sscanf(buffer, "%s", &fileName);

        inputFilep = fopen(fileName, "r");
        if (inputFilep == NULL)
        {
            fprintf(stderr, "ERROR: File %s is not open for coin input\n", fileName);
            failCount += 1;
        }
        else break;
    }

    if (failCount == 3)
    {
        fprintf(stderr, "ERROR: You exceeded maximum number of tries allowed\n       for entering the coin input file name\n");
        free(CoinTypes);
        return 1;
    }

    FILE *outputFilep;

    failCount = 0;
    while (failCount < 3)
    {
        char fileName[1024];
        char buffer[1024];
        printf("Type the name of the coin output file: ");
        fgets(buffer, 1024, stdin);
        sscanf(buffer, "%s", &fileName);

        outputFilep = fopen(fileName, "w");
        if (outputFilep == NULL)
        {
            fprintf(stderr, "ERROR: File %s is not open for output\n", fileName);
            failCount += 1;
        }
        else break;
    }

    if (failCount == 3)
    {
        fprintf(stderr, "ERROR: You exceeded maximum number of tries allowed\n       while entering the output file name\n");
        free(CoinTypes);
        return 1;
    }

    // read the first non-whitespace / newline
    currentPos = 0;
    somethingRead = false;
    char firstChar = '0';
    while (1)
    {
        currentPos = ftell(inputFilep);
        if (fgets(buffer, 1024, inputFilep) != NULL) // read each line until NULL ptr
        {
            size_t length = strlen(buffer);
            for (int i = 0; i < length; i++)
            {
                if ((buffer[i] != ' ' ) && (buffer[i] != '\n')) // if non-whitespace detected
                {
                    firstChar = buffer[i];
                    somethingRead = true;
                    break;
                }
            }
            if (somethingRead == true) break; // if something has been read stop looping
        }
        else // EOF is hit and nothing relevant is caught
        {
            if (feof(inputFilep))
            {
                fprintf(stderr, "ERROR: Coin input file is empty\n");
                fclose(configFilep);
                fclose(inputFilep);
                fclose(outputFilep);
                free(CoinTypes);
                return 1;
            }
        }   
    }

    int numOfLines = 0;
    if (isdigit(firstChar) != 0) // if the first character is a digit
    {
        fseek(inputFilep, currentPos, SEEK_SET); // reset file pointer to start of line
        fscanf(inputFilep, " %d", &numOfLines); // read number as int this time
        if ((numOfLines < 1) || (numOfLines > 5000)) // return error if out of range
        {
            printf("numOfLines: %d\n", numOfLines);
            fprintf(stderr, "ERROR: The number of coins is out of range\n");
            fclose(configFilep);
            fclose(inputFilep);
            fclose(outputFilep);
            free(CoinTypes);
            return 1;
        }
    }
    else // first character is not a digit
    {
        fprintf(stderr, "ERROR: First piece of data in the coin input file is not an integer\n");
        fclose(configFilep);
        fclose(inputFilep);
        fclose(outputFilep);
        free(CoinTypes);
        return 1;
    }

    somethingRead = false; // detect if empty
    int linectr = 0;

    // keep track of various important variables
    int rollAmount[coinAmount]; // amount of coins in current roll of type
    memset(rollAmount, 0, sizeof(rollAmount));
    int rollCount[coinAmount]; // amount of rolls of current coin type
    memset(rollCount, 0, sizeof(rollCount));
    double bentWeight = 0; // weight of bent coins in current container
    int bentContainers = 0; // amount of containers of bent coins
    double bentTotalWeight = 0; // total weight of the bent coins
    int otherCount = 0; // amount of other coins in the current container
    double otherWeight = 0; // weight of other coins in the current container
    int otherContainers = 0; // amount of containers of other coins
    int otherTotal = 0; // total number of other coins
    double otherTotalWeight = 0; // total weight of the other coins
    char firstName[1024]; // for some reason, the name of the first CoinTypes struct becomes garbage after the for loop. Probably something to do with the way I handled EOF errors. In any case, this is here to store that for the summary printing
    strcpy(firstName, CoinTypes[0].name);

    for(int i = 0; i < numOfLines; i++)
    {
        char buffer[1024] = {0};
        double weight = 0; // weight of coin
        char weightBuffer[1024] = {0};
        double diameter = 0; // diameter of coin
        char diameterBuffer[1024] = {0};
        char usability[1024] = {0}; // is the coin bent or not
        char matches[1024] = {0}; // how many sides of the coin match
        char garbage[1024] = {0}; // useless stuff at end of a line

        int count = 0;
        fgets(buffer, 1024, inputFilep);
        count = sscanf(buffer," %s %s %s %s %s", weightBuffer, diameterBuffer, usability, matches, garbage);
        char *err;
        char *err2;
        weight = strtod(weightBuffer, &err);
        diameter = strtod(diameterBuffer, &err2);
        if (count != -1) // ignore blank lines
        {
            somethingRead = true;
            linectr++; // start at one, skipping the line initiating numOfLines
            if (*err != 0)
            {
                fprintf(stderr, "ERROR: Weight value read, on line %d, is not a double\nSimulation terminated early: Please correct your data file\n", linectr);
                break;
            }
            else if ((weight < 0) || (weight > 40))
            {
                fprintf(stderr, "ERROR: Weight out of range, ignoring line %d in the input file\n", linectr);
                continue;
            }
            else if (count == 1)
            {
                fprintf(stderr, "ERROR: Weight measurement only\nIgnoring line %d of the input file\n", linectr);
                continue;
            }
            else if (*err2 != 0)
            {
                fprintf(stderr, "ERROR: Diameter value read on line %d is not a double\nSimulation terminated early: Please correct your data file\n", linectr);
                break;
            }
            else if ((diameter < 0) || (diameter > 60))
            {
                fprintf(stderr, "ERROR: Diameter out of range, ignoring line %d in the input file\n", linectr);
                continue;
            }
            else if (count == 2)
            {
                fprintf(stderr, "ERROR: Weight and diameter measurements only\nIgnoring line %d of the input file\n", linectr);
                continue;
            }
            else if ((strcmp(usability, "bent") != 0) && (strcmp(usability, "usable") != 0))
            {
                fprintf(stderr, "ERROR: Result of test to determine if coin is bent at line %d is invalid\nIgnoring this line of data\n", linectr);
                continue;
            }
            else if (count == 3)
            {
                fprintf(stderr, "ERROR: Weight and diameter and bent string only\nIgnoring line %d of the input file\n", linectr);
                continue;
            }
            else if ((strcmp(matches, "BothSidesMatch") != 0) && (strcmp(matches, "OneSideMatches") != 0) && (strcmp(matches, "NoMatch") != 0))
            {
                fprintf(stderr, "ERROR: image processing result at line %d is invalid\nIgnoring this line of data\n", linectr);
                continue;
            }
            else if (count > 4)
            {
                fprintf(stderr, "ERROR: Extra data at line %d. Ignoring extra data\n", linectr);
            }
        }
        else if ((count == -1) && (feof(inputFilep) != 0)) // input file is empty past the coin number declaration
        {
            // no need to put this in order, it will always activate first if it is the sole error.
            linectr++;
            fprintf(stderr, "ERROR: No more data\nSimulation completed early before line %d of input\n", linectr);
            break;
        }
        else continue; // ignore empty lines

        int coinCategory = 0; // flag determining coin category. 6 equals bent, 5 equals other, 0 - 4 are reserved for potential coin types from the config file
        coinCategory = DetermineCoinType(CoinTypes, weight, diameter, coinAmount);
        // technically out of order, but since both match inviability and out-of-range faults evaluate to other, it should be fine

        if (strcmp(matches, "BothSidesMatch") != 0) // overwrite for unmatching sides first
        {
            coinCategory = 5;
        }

        if (strcmp(usability, "bent") == 0) // overwrite next for bent coins
        {
            coinCategory = 6;
        }

        if (coinCategory < 5) // if coin is not bent or other
        {
            if ((rollAmount[coinCategory] + 1) == CoinTypes[coinCategory].rollCapacity) // if causes overflow
            {
                rollCount[coinCategory] += 1; // increment roll by 1
                rollAmount[coinCategory] = 0; // reset roll amount to 0
                fprintf(stdout, "The Coin Sorter has sent one coin to the %s's wrapper\nThe %s wrapper is now full\nThe %s wrapper has now been replaced\n", CoinTypes[coinCategory].name, CoinTypes[coinCategory].name, CoinTypes[coinCategory].name);
                fprintf(outputFilep, "The Coin Sorter has sent one coin to the %s's wrapper\nThe %s wrapper is now full\nThe %s wrapper has now been replaced\n", CoinTypes[coinCategory].name, CoinTypes[coinCategory].name, CoinTypes[coinCategory].name);
            }
            else
            {
                rollAmount[coinCategory] += 1;
                fprintf(stdout, "The Coin Sorter has sent one coin to the %s's wrapper\nThere are now %d coins in the %s's wrapper\n", CoinTypes[coinCategory].name, rollAmount[coinCategory], CoinTypes[coinCategory].name);
                fprintf(outputFilep, "The Coin Sorter has sent one coin to the %s's wrapper\nThere are now %d coins in the %s's wrapper\n", CoinTypes[coinCategory].name, rollAmount[coinCategory], CoinTypes[coinCategory].name);
            }
        }
        else if (coinCategory == 6) // if coin is bent
        {
            if (((bentWeight + weight) >= 300))
            {
                bentContainers +=1;
                bentWeight = weight;
                fprintf(stdout, "The Coin Sorter has sent this coin to the bent coin container\nThe coin does not fit in the bent coin container\nThe bent coin container has been replaced\nThe new coin in the new bent coin container weighs %lf grams\n", bentWeight);
                fprintf(outputFilep, "The Coin Sorter has sent this coin to the bent coin container\nThe coin does not fit in the bent coin container\nThe bent coin container has been replaced\nThe new coin in the new bent coin container weighs %lf grams\n", bentWeight);
            }
            else
            {
                bentWeight += weight;
                fprintf(stdout, "The Coin Sorter has sent this coin to the bent coin container\nThe coins in the bent coin container now weigh %lf grams\n", bentWeight);
                fprintf(outputFilep, "The Coin Sorter has sent this coin to the bent coin container\nThe coins in the bent coin container now weigh %lf grams\n", bentWeight);
            }
            bentTotalWeight += weight;
        }
        else if (coinCategory == 5) // if coin is other
        {
            if ((otherWeight + weight) >= 200)
            {
                otherContainers += 1;
                otherWeight = weight;
                otherCount = 1;
                fprintf(stdout, "The Coin Sorter has sent this coin to the other coin container\nThis coin does not fit in the other coin container\nThe other coin container has been replaced\nThe coin in the other coin container now weigh %lf grams\n", otherWeight);
                fprintf(outputFilep, "The Coin Sorter has sent this coin to the other coin container\nThis coin does not fit in the other coin container\nThe other coin container has been replaced\nThe coin in the other coin container now weigh %lf grams\n", otherWeight);
            }
            else
            {
                otherWeight += weight;
                otherCount += 1;
                fprintf(stdout, "The Coin Sorter has sent this coin to the other coin container\nThe coins in the other coin container now weigh %lf grams\n", otherWeight);
                fprintf(outputFilep, "The Coin Sorter has sent this coin to the other coin container\nThe coins in the other coin container now weigh %lf grams\n", otherWeight);
            }
            otherTotal += 1;
            otherTotalWeight += weight;
        }
    }
    
    strcpy(CoinTypes[0].name, firstName); // reset name of first cointypes struct

    // real coins summary
    fprintf(stdout, "\n\n\nSUMMARY\n");
    fprintf(outputFilep, "\n\n\nSUMMARY\n");
    for (int i = 0; i < coinAmount; i++)
    {
        fprintf(stdout, "The Coin Sorter made %d rolls of %s.\n", rollCount[i], CoinTypes[i].name);
        fprintf(outputFilep, "The Coin Sorter made %d rolls of %s.\n", rollCount[i], CoinTypes[i].name);
        fprintf(stdout, "  There are %d %ss in the partially full roll.\n", rollAmount[i], CoinTypes[i].name);
        fprintf(outputFilep, "  There are %d %ss in the partially full roll.\n", rollAmount[i], CoinTypes[i].name);
    }
    // other coins summary
    fprintf(stdout, "The Coin Sorter processed %d other coins.\n  The other coins filled %d containers\n", otherTotal, otherContainers);
    fprintf(outputFilep, "The Coin Sorter processed %d other coins.\n  The other coins filled %d containers\n", otherTotal, otherContainers);
    fprintf(stdout, "There were %d other coins in the partially full container\n  The total weight of the other coins was %lf grams\n", otherCount, otherTotalWeight);
    fprintf(outputFilep, "There were %d other coins in the partially full container\n  The total weight of the other coins was %lf grams\n", otherCount, otherTotalWeight);
    // bent coins summary
    fprintf(stdout, "The Coin Sorter processed %lf g of bent coins\n", bentTotalWeight);
    fprintf(outputFilep, "The Coin Sorter processed %lf g of bent coins\n", bentTotalWeight);

    // clean up
    free(CoinTypes);
    fclose(configFilep);
    fclose(inputFilep);
    fclose(outputFilep);

    return 0;
}
