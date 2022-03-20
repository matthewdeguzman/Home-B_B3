#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#define DEFAULT_SIZE 20
using namespace std;

// verifies if the sum of labelings are larger than n
bool isLarger(unordered_map<int, int> &s, int n)
{
    // keeps track of sum
    int sum = 0;

    // calculates the sum by adding the labels and their frequencies
    // if we find the sum to be larger than n, we return true
    for(auto const& [label, freq] : s)
    {
        sum += label * freq;
        if(sum > n) return true;
    }

    // the sum was not found to be larger than n
    return false;
}

// verifies if the sum of labelings equal n
bool isEqual(unordered_map<int,int> &s, int n)
{
    int sum = 0;

    // iterates through the map and calculates the sum
    // in the same fashion as the isLarger method
    for(auto const& [label, freq] : s)
        sum += label * freq;

    return sum == n;
}

// constructs a vector from the elements of the ordered map
vector<int> *constructVector(unordered_map<int, int> &s)
{
    // creates a new vector vec with the default size of
    // set of labelings (20)
    vector<int> *vec = new vector<int>(DEFAULT_SIZE);

    // holds the index of vec
    int index = 0;

    // iterates through s and places each label into vec
    // according to its frequency
    for(auto const& [label, freq] : s)
    {
        for (int i = 0; i < freq; i++)
            vec->at(index + i) = label;

        // increments index according to the labeling's frequency
        index += freq;
    }

    return vec;

}
vector<int> *findSeq(int n)
{
    // creates a hashmap where the key will be the label
    // and the value will be the frequency of label
    unordered_map<int,int> s;

    // intializes s with the minimum labeling
    for(int i = 1; i <= 5; i++)
        s[i] = 4;

    // if the minimum case is larger than the given n,
    // it is not possible to construct such a set of labelings
    // and the nulllptr (empty set) is returned
    if(isLarger(s, n)) return nullptr;

    // we rearrange s until we find a set of labels that
    // are equal to n

    // keeps track of the largest vertex which is
    // at the "end" of s
    int end = 5;
    while(!isEqual(s, n))
    {
        // once the frequency of end has reached 4, another vertex of
        // end cannot be legally labeled, so we create a new labeling
        // that is end += 1 and set the frequency to 1
        if(s[end] == 4)
        {
            s[end]--;
            end++;
            s[end] = 1;
        }

        // else we update the two largest vertices, incrementing the larger
        // and decrementing the shorter to add 1 to the total sum
        // we remove s[end-1] from the map for memory optimization once it reaches 0
        else
        {
            s[end]++;
            s[end-1]--;
            if(s[end-1] == 0)
                s.erase(end-1);
        }
    }

    // once the labelings add to n, we return
    // the map as a vector
    return constructVector(s);
}
vector<int> *findSeq(vector<int> *set, int n)
{
    unordered_map<int, int> s;
    int end = set->at(0);
    for(int i = 0; i < set->size(); i++)
    {
        if(!s.count(set->at(i)))
            s[set->at(i)] = 1;
        else s[set->at(i)]++;

        end = max(end, set->at(i));
    }

    while(!isEqual(s, n))
    {
        // once the frequency of end has reached 4, another vertex of
        // end cannot be legally labeled, so we create a new labeling
        // that is end += 1 and set the frequency to 1
        if (s[end] == 4) {
            s[end]--;
            end++;
            s[end] = 1;
        }

            // else we update the two largest vertices, incrementing the larger
            // and decrementing the shorter to add 1 to the total sum
            // we remove s[end-1] from the map for memory optimization once it reaches 0
        else {
            s[end]++;
            s[end - 1]--;
            if (s[end - 1] == 0)
                s.erase(end - 1);
        }
    }

    // deletes the previous set to prevent memory leaks
    delete set;
    return constructVector(s);
}

// displays contents of a vector
void display(vector<int> *set)
{
    // if the set is a nullptr, we were not able to find a set
    // with legal labelings that added to n
    if(set == nullptr)
        cout << "Not achievable with legal labelings";

        // else we display each element in s
    else
        for(int i = 0; i < set->size(); i++)
            cout << set->at(i) << " ";

    cout << endl;
}
void writeOneHundredThousand() {

    ofstream file;
    file.open("hundred-thousand.txt");

    // use the fact that it is not possible to produce a sum less than 60
    // to say that the posints 1 through 59 are unachievable
    for(int i = 1; i < 60; i++)
    {
        vector<int> *set = findSeq(i);
        if(set == nullptr)
        {
            file << i << ":\n";
            file << "Not achievable with legal labelings\n";
            file << "------------------------------------------------------\n";
        }
        delete set;
    }

    // find and write the first solution
    vector<int> *set = findSeq(60);
    for(int i = 0; i < set->size(); i++)
        file << set->at(i) << " ";
    file << "\n------------------------------------------------------\n";

    // writes the next solutions up to 100k by expanding
    // off of the previous solution
    for(int n = 61; n <= 100000; n++)
    {
        set = findSeq(set, n);
        file << n << ":\n";
        for(int i = 0; i < set->size(); i++)
            file << set->at(i) << " ";
        file << "\n------------------------------------------------------\n";
    }

    file.close();
}
int main() {

    // displays a set of legal vertices
    // add to n
    int n = 403;
    display(findSeq(n));

    // writes all labelings if they exist for positive integers
    // one to one hundred thousand to a text file
    writeOneHundredThousand();

    return 0;
}
