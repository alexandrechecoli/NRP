#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <vector>
#include <string>

using namespace std;

class InputData
{
public:

    // Structs
    struct nurse
    {
        vector<int> v_vacation_days;
        vector<int> v_preferred_days_off;
        string id;
    };

    InputData();

    int n_days;                // Number of days of the planning period
    vector<int> v_weekend;     // Weekend days, starting at 0
    double frac_days_off;      // Fraction of days off (for instance, 5/30 = 5 days off on a 30 days period)
    int max_consecutive_work_days; // Maximum consecutive days allowed without a day-off

    // SHIFT COVERAGE
    vector<int> v_mor_min; // Minimum coverage of morning shifts (per day)
    vector<int> v_mor_max; // Maximum coverage of morning shifts (per day)
    vector<int> v_aft_min; // Minimum coverage of afternoon shifts (per day)
    vector<int> v_aft_max; // Maximum coverage of afternoon shifts (per day)

    // Nurses
    vector<nurse> v_nurses; // Information of nurses


};

#endif // INPUTDATA_H
