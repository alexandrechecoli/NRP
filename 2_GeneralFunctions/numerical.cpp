#include "numerical.h"


int get_int_random_between(int xlower, int xupper)
{

    int temp =-1;
    if(xlower <= xupper)
    {
        temp = rand() % (xupper - xlower + 1) + xlower;
    }

    return temp;
}

bool is_equal(double xa, double xb)
{
    if(fabs(xa-xb) <= EPS) return true;
    return false;
}

double get_random_0_1()
{
    return rand() / double(RAND_MAX);
}

bool is_A_less_than_B(double A, double B)
{

    double delta = A - B;

    if(delta < -EPS) return true;
    else return false;

}

vector<double> get_rel_freq_vector(vector<int> &v)
{

     /*
     * Calculates the relative frequence of a vector of
     * integers (v), assigning the results on a vector
     * of double (v_rel_freq):
     *  EX: v          = [1,3,2,0,4]
     *      v_rel_freq = [0.1,0.3,0.2,0.0,0.4]
     */


    vector<double> v_rel_freq(v.size());
    int sum = sum_vector_elements(v);

    for(size_t i = 0; i < v.size(); i++)
    {
        v_rel_freq[i] = static_cast<double>(v[i])/sum;
    }

    return v_rel_freq;
}

double sum_vector_elements(vector<int> &v)
{
    /*
     * Sum the elements of the vector
     */

    double sum = 0;
    for(int i = 0; i < v.size(); i++)
    {
        sum += v[i];
    }
    return sum;
}

double sum_vector_elements(vector<double> &v)
{
    /*
     * Sum the elements of the vector
     */

    double sum = 0;
    for(int i = 0; i < v.size(); i++)
    {
        sum += v[i];
    }
    return sum;
}

int select_index_from_freq_vector(vector<double> &v_freq_vector)
{

    /*
     * Given a vector of relative frequences (sum must equal 1),
     * selects an index of the vector, with some probability
     * according to the frequences.
     */


    double prob     = get_random_0_1();
    double cum_prob = v_freq_vector[0];
    size_t index    = 0;


    // It may occur rounding error, for example if prob = 1. cum_prob might not be exactly 1, so the loop
    // would run forever. Ergo the "is_equal" condition. If the number are equal the loop stops.
    while((cum_prob < prob) & !is_equal(cum_prob,prob))
    {
        index++;
        cum_prob += v_freq_vector[index];
    }

    return static_cast<int>(index);
}
