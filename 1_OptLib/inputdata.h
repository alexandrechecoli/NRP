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
        int n_days_off;                 //number of days off for the nurse (proportional to the number of days that she will work - (n_days - size(vacation_days))*frac_days_off
        int n_days_off_weekend;
        vector<int> v_vacation_days;
        vector<int> v_preferred_days_off;
        string id;
    };

    InputData();

    int n_days;                        // Number of days of the planning period
    vector<int> v_weekend;             // Weekend days, starting at 0
    int max_consecutive_work_days;     // Maximum consecutive days allowed without a day-off

    //FRACTIONS
    double frac_days_off;              // Fraction of days off (for instance, 5/30 = 5 days off on a 30 days period)
    double frac_days_off_weekend;      // Fraction of days off on weekends (for instance, 1/30 = 1 days off on weekend on a 30 days period)

    // SHIFT COVERAGE
    vector<int> v_mor_min; // Minimum coverage of morning shifts (per day)
    vector<int> v_mor_max; // Maximum coverage of morning shifts (per day)
    vector<int> v_aft_min; // Minimum coverage of afternoon shifts (per day)
    vector<int> v_aft_max; // Maximum coverage of afternoon shifts (per day)

    // Nurses
    vector<nurse> v_nurses; // Information of nurses

    void initialize_nurse_parameters();


    //Output data =======================================================
    vector<vector<int>> vv_aft_shift;
    vector<vector<int>> vv_mor_shift;
    int cost;

    //  ========  Debug data ============================================
    vector<int> v_aft_diff;
    vector<int> v_mor_diff;

    vector<int> v_shift_assigned_to_nurse;
    vector<int> v_nurse_sum_rest_days;
    vector<int> v_nurse_sum_rest_days_weekends;
    vector<int> v_nurse_extra_shifts;


    // Objective function components
    int sum_mor_diff;             // A1
    int sum_aft_diff;             // A2

    int sum_extra_shifts;         // B Number of extra shifts performed by the nurse
    int extra_shift_load_balance; // C;
    int weekend_rest_violation;   // D;
    int prefered_rest_violation;  // E;


    //  ========  Debug data ============================================

};

#endif // INPUTDATA_H
