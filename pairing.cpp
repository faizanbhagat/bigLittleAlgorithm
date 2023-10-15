//
//  pairing.cpp
//  blAlgorithm
//
//  Created by Faizan Bhagat on 6/23/23.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <cmath>
#include <limits>
#include <numeric>
#include <unordered_map>

using namespace std;

/*
 Struct that holds big little pairings.
 Consisting of a big, a little, and their weight
 */
struct bigLittlePairing
{
    string big;
    string little;
    int weightAmount;

    void set(string bigIn, string littleIn, int weightAmountIn)
    {
        big = bigIn;
        little = littleIn;
        weightAmount = weightAmountIn;
    }
};

vector<string> allBigs;
vector<string> allLittles;
vector<string> bigs;
vector<string> littles;

vector<vector<string> > bigToLittle;
vector<vector<string> > littleToBig;

vector<bigLittlePairing> finalPairing;
vector<bigLittlePairing> oneToOnes;
vector<vector<bigLittlePairing> > allFinalPairings;

unordered_map<string, unordered_map<string, int> > bigLittleDist;
unordered_map<string, unordered_map<string, int> > littleBigDist;

int bestAmount = 1000;
int currAmount = -1;

int amountOfTwins = 0;
int counter = 0;

/*
 Requires : finalPairing
 Modifies : Nothing
 Effects : Prints out one finalPairing by printing all of the
    one to one pairings followed by all other pairings created by the algorithm.
    Also tallies and prints the totalWeight of the pairing.
*/

bool compareByWeight(const bigLittlePairing &a, const bigLittlePairing &b)
{
    return a.weightAmount <= b.weightAmount;
}

void printOneFinalPairing(vector<bigLittlePairing> finalPairing)
{
    sort(finalPairing.begin(), finalPairing.end(), compareByWeight);
    int totalWeightCounter = 0;
    double totalWeightAverage;
    cout << "One to One Pairings: " << endl;
    for (int i = 0; i < oneToOnes.size(); i++)
    {
        cout << "Big: " << oneToOnes[i].big << ", Little: " << oneToOnes[i].little << endl;
        totalWeightCounter += 2;
    }

    cout << endl;
    cout << "All Other Pairings: " << endl;

    for (int i = 0; i < finalPairing.size(); i++)
    {
        cout << "Big: " << finalPairing[i].big << ", Little: " << finalPairing[i].little << ", Weight Amount: " << finalPairing[i].weightAmount << endl;
        totalWeightCounter += bigLittleDist[finalPairing[i].big][finalPairing[i].little];
        totalWeightCounter += littleBigDist[finalPairing[i].little][finalPairing[i].big];
    }

    // weighted average divides total weight by total number of people in pairing
    // (amount of littles is how many pairings there are)

    totalWeightAverage = (totalWeightCounter * 1.0) / (allLittles.size() * 2.0);

    cout << endl
         << "Total weight of solution: " << totalWeightCounter << endl;
    cout << "Average weight of solution per person: " << totalWeightAverage << endl;
}

/*
 Requires : allFinalPairings
 Modifies : Nothing
 Effects : Prints all finalPairings that have equivalent weights
*/
void printData()
{
    if (allFinalPairings.size() == 0)
    {
        cout << "No possible solutions, tough luck!" << endl;
        return;
    }
    cout << "Number of Optimal Solutions: " << allFinalPairings.size() << endl
         << endl;
    for (int i = 0; i < allFinalPairings.size(); i++)
    {
        cout << "Optimal Solution #" << i + 1 << ": " << endl;
        printOneFinalPairing(allFinalPairings[i]);
        cout << endl;
    }
}

/*
 Requires : Two strings :
    bigToLittleInput : Big's rankings of their littles
    littleToBigInput : Little's rankings of their bits
 Modifies : bigToLittle, littleToBig, amountOfTwins
 Effects : Reads in all of the data into 2 2d vectors. Also sets the number of twins in the pairing set.
*/

void readData(string bigToLittleInput, string littleToBigInput)
{
    fstream file(bigToLittleInput, ios::in);
    string line;
    string word;

    vector<string> row;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ','))
            {
                row.push_back(word);
            }
            bigToLittle.push_back(row);
        }
    }

    fstream lFile(littleToBigInput, ios::in);

    if (lFile.is_open())
    {
        while (getline(lFile, line))
        {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ','))
            {
                row.push_back(word);
            }
            littleToBig.push_back(row);
        }
    }

    if (bigToLittle.size() > littleToBig.size())
    {
        cout << "ERROR: Amount of bigs is greater than amount of littles, no solution possible" << endl;
        exit(1);
    }

    // gets rid of \r at the last column of .csv files
    for (int i = 0; i < littleToBig.size(); i++)
    {
        littleToBig[i][littleToBig[0].size() - 1].erase(littleToBig[i][littleToBig[0].size() - 1].size() - 1);
    }

    for (int i = 0; i < bigToLittle.size(); i++)
    {
        bigToLittle[i][bigToLittle[0].size() - 1].erase(bigToLittle[i][bigToLittle[0].size() - 1].size() - 1);
    }

    amountOfTwins = static_cast<int>(littleToBig.size() - bigToLittle.size());
}

/*
 Requires : bigToLittle, littleToBig
 Modifies : littleBigDist, bigLittleDist
 Effects : Sets 2 2d hashmaps that map each pairing to their weight based on the rankings given.
    If Big A ranks Little D 3rd, bigLittleDist[A][D] = 3
*/

void setDistanceMatrix()
{
    for (int i = 0; i < littleToBig.size(); i++)
    {
        littleBigDist[littleToBig[i][0]];
        for (int j = 1; j < littleToBig[i].size(); j++)
        {
            littleBigDist[littleToBig[i][0]][littleToBig[i][j]] += j;
        }
    }

    for (int i = 0; i < bigToLittle.size(); i++)
    {
        bigLittleDist[bigToLittle[i][0]];
        for (int j = 1; j < bigToLittle[i].size(); j++)
        {
            bigLittleDist[bigToLittle[i][0]][bigToLittle[i][j]] += j;
        }
    }
}

/*
 Requires : bigToLittle, littleToBig
 Modifies : allBigs, allLittles
 Effects : Sets 2 vectors to have all of the bigs names and all of the littles names in a convenient storage
*/

void setPCLists()
{
    for (int i = 0; i < bigToLittle.size(); i++)
    {
        allBigs.push_back(bigToLittle[i][0]);
    }
    for (int j = 0; j < littleToBig.size(); j++)
    {
        allLittles.push_back(littleToBig[j][0]);
    }
}

/*
 Requires : bigToLittle, littleToBig, littleBigDist, bigLittleDist
 Modifies : oneToOnes
 Effects : Goes through entire data set and gathers all of the one to one rankings and stores them into vector, oneToOnes.
    For our ranking methodology, these one to rankings are GUARANTEED matches and are taken out of the rest of the pairing
*/
void setOneToOne()
{
    string currBig;
    string currLittle;

    for (int i = 0; i < allBigs.size(); i++)
    {
        currBig = bigToLittle[i][0];
        currLittle = bigToLittle[i][1];
        if (littleBigDist[currLittle].find(currBig) != littleBigDist[currLittle].end())
        {
            if (bigLittleDist[currBig].find(currLittle) != bigLittleDist[currBig].end())
            {
                if (littleBigDist[currLittle][currBig] == 1 && bigLittleDist[currBig][currLittle] == 1)
                {
                    bigLittlePairing temp;
                    temp.set(currBig, currLittle, 2);
                    oneToOnes.push_back(temp);
                }
            }
        }
    }
}

/*
 Requires : allBigs, allLittles, oneToOnes
 Modifies : littles, bigs
 Effects : Removes all of the one to ones from the dataset as they no longer need to be paired.
    Accurately updates vectors (bigs, littles) to not contain anyone that was a one to one ranking.
*/

void removeOneToOnes()
{
    bool bigPresent = false;

    for (int i = 0; i < allBigs.size(); i++)
    {
        string currBig = allBigs[i];
        for (int j = 0; j < oneToOnes.size(); j++)
        {
            if (currBig == oneToOnes[j].big)
            {
                bigPresent = true;
            }
        }
        if (!bigPresent)
        {
            bigs.push_back(currBig);
        }
        bigPresent = false;
    }

    bool littlePresent = false;
    for (int i = 0; i < allLittles.size(); i++)
    {
        string currLittle = allLittles[i];
        for (int j = 0; j < oneToOnes.size(); j++)
        {
            if (currLittle == oneToOnes[j].little)
            {
                littlePresent = true;
            }
        }
        if (!littlePresent)
        {
            littles.push_back(currLittle);
        }
        littlePresent = false;
    }
}

/*
 Requires : Many things
 Modifies : finalPairing
 Effects : Runs a branch and bound algorithm that finds the mathematically most efficient big little pairing.
    Uses an algorithm to permutate all possible combinations of big little rankings
    and prunes branches that are worse than the current best
*/
void generatePairings(const vector<string> &littles, const vector<string> &bigs,
                      vector<pair<string, string> > &pairings, vector<bool> &used, int index, int i)
{
    if (index == bigs.size())
    {
        // checks if better than curent best solution
        if (currAmount <= bestAmount)
        {
            for (int i = 0; i < pairings.size(); i++)
            {
                bigLittlePairing temp;
                temp.set(pairings[i].second, pairings[i].first, -1);
                finalPairing[i] = temp;
            }
            // if currAmount is BETTER than best amount, clears all other rankings and uses thie one
            // if they are equal, then keeps all other rankings and adds this pairing to allFinalPairings
            if (currAmount < bestAmount)
            {
                allFinalPairings.clear();
            }
            allFinalPairings.push_back(finalPairing);
            bestAmount = currAmount;
        }
        return;
    }

    // prunes if the current attempt is worse than the best
    if (currAmount > bestAmount)
    {
        return;
    }

    for (int i = 0; i < littles.size(); ++i)
    {
        if (!used[i])
        {
            int tempWeight = 0;
            if (littleBigDist[littles[i]].find(bigs[index]) == littleBigDist[littles[i]].end())
            {
                tempWeight = 5000;
            }
            else
            {
                tempWeight += littleBigDist[littles[i]][bigs[index]];
            }
            if (bigLittleDist[bigs[index]].find(littles[i]) == bigLittleDist[bigs[index]].end())
            {
                tempWeight = 5000;
            }
            else
            {
                tempWeight += bigLittleDist[bigs[index]][littles[i]];
            }

            used[i] = true;

            pairings[index] = make_pair(littles[i], bigs[index]);
            currAmount += tempWeight;
            generatePairings(littles, bigs, pairings, used, index + 1, i);
            currAmount -= tempWeight;
            used[i] = false;
        }
    }
}

// function that calls generatePairings to perform permuation, RME is the same
void generateAllPairings(const vector<string> &littles, const vector<string> &bigs)
{
    if (littles.size() != bigs.size())
    {
        cout << "Cannot generate pairings. Groups have different sizes." << std::endl;
        exit(2);
    }

    vector<bool> used(littles.size(), false);                   // Track used people from littles
    vector<std::pair<string, string> > pairings(littles.size()); // Store the pairings
    finalPairing.resize(bigs.size());

    generatePairings(littles, bigs, pairings, used, 0, 1);
}

/*
 Requires : bigLittleDist, littleBigDist, allFinalPairings
 Modifies : allFinalPairings
 Effects : Iterates through allFinalPairings and sets the weight of each pairing for user to know
    If Big C ranked Little E 3rd and Little E ranked Big C 4th, their weight would be 7 (3 + 4)
*/
void setPairingWeights()
{
    for (int j = 0; j < allFinalPairings.size(); j++)
    {
        for (int i = 0; i < finalPairing.size(); i++)
        {
            int weightAmount = littleBigDist[allFinalPairings[j][i].little][allFinalPairings[j][i].big];
            weightAmount += bigLittleDist[allFinalPairings[j][i].big][allFinalPairings[j][i].little];
            allFinalPairings[j][i].weightAmount = weightAmount;
        }
    }
}

/*
 Requires : Many things
 Modifies : finalPairings
 Effects : If big list is smaller than littles (twins are required), this function iterates through all bigs and
    makes every possible twin combination to pass into generateAllPairings. This is to ensure mathematically optimal solution
    because any bigs (even one to ones) can have twins so all must be tested in permutation.
*/
void combinationsHelper(vector<string> &items, vector<string> &combination, int start, int n)
{
    if (combination.size() == n)
    {
        counter++;
        vector<string> pairingBigInput = bigs;
        pairingBigInput.resize(littles.size());

        for (int i = 0; i < combination.size(); i++)
        {
            pairingBigInput[i + bigs.size()] = combination[i];
        }

        generateAllPairings(littles, pairingBigInput);

        return;
    }

    for (int i = start; i < items.size(); ++i)
    {
        combination.push_back(items[i]);
        combinationsHelper(items, combination, i + 1, n);
        combination.pop_back();
    }
}

void combinations(vector<string> &items, int n)
{
    std::vector<string> combination;
    combinationsHelper(items, combination, 0, n);
}

void userInput()
{
    string bigToLittleFilename;
    string littleToBigFilename;

    cout << "Welcome to the Big Little Ranking Algorithm!" << endl
         << "Please refer to the README.md for instructions on usage and required input types." << endl
         << endl;

    cout << "Please enter the big to little ranking filename: ";
    cin >> bigToLittleFilename;

    cout << "Please enter the little to big ranking filename: ";
    cin >> littleToBigFilename;
    cout << endl;
    readData(bigToLittleFilename, littleToBigFilename);
}

/*
 Requires : File names in arguments
 Modifies : Everything
 Effects : Finds mathematically optimal big little pairings :)
*/
int main(int argc, char *argv[])
{
    int x = 3;
    int y = 45;
    int z = x + y;

    userInput();

    setPCLists();
    setDistanceMatrix();

    setOneToOne();
    removeOneToOnes();

    combinations(allBigs, amountOfTwins);

    setPairingWeights();
    printData();
    return 0;
}
