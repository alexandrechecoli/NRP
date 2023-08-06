#ifndef NRP_H
#define NRP_H

#include <map>
#include <list>
#include <iostream>
#include <climits>
#include <ctime>
#include <chrono>
#include <vector>


#include "inputdata.h"
#include "numerical.h"

using namespace  std;

class JsonParser;


class NRP
{

    protected:
        class Solution
        {
            public:
                vector<vector<int>> vv_mor_assig;
                vector<vector<int>> vv_aft_assig;
                vector<int> v_mor_diff;
                vector<int> v_aft_diff;
                vector<int> v_shift_assigned_to_nurse;

                // === Cost parts (see udate_cost() foe the letters) ===
                int sum_mor_diff;             // A1
                int sum_aft_diff;             // A2

                int sum_extra_shifts;         // B Number of extra shifts performed by the nurse
                int extra_shift_load_balance; // C;
                int weekend_rest_violation;   // D;
                int prefered_rest_violation;  // E;

                vector<int> v_nurse_sum_rest_days;
                vector<int> v_nurse_sum_rest_days_weekends;
                vector<int> v_nurse_extra_shifts;  //Number of extra shifts performed by the nurse

                double cost;

        };

    public:
        NRP();
        NRP(InputData *xp_dt);
        void optimize();

     private:
        // DATA ---------------------------------------
        InputData * dt;


        // FUNCTIONS -----------------------------------
        void assign_nurse_shifts(vector<int> &v_assigned_shift, vector<int> &v_unassigned_shift, vector<int> &v_diff, int &sum_diff, int start_day, int &sum_nurse_rest);
        void assign_extra_shift(vector<vector<int>> &vv_import_extra_shift, vector<vector<int>> &vv_export_extra_shift, vector<int> &v_import_extra_shift_diff, int &import_extra_shift_sum_diff,
                                vector<int> &v_shift_assigned_to_nurse,vector<int> &v_nurse_extra_shift, int export_extra_shift);
        void add_mising_days_off(Solution &s);
        void calculate_days_off_on_weekend(Solution &s);
        Solution initial_solution_constructive_1();
        Solution initial_solution();
        void initialize_solution_ds(Solution &s);
        void relocate_rest(vector<vector<int>> &vv_assign, vector<int> &v_diff, int &sum_diff, vector<int> &v_shift_assigned_to_nurse, int shift);
        void relocate_nurse_rest(vector<int> &v_assign, vector<int> &v_diff, int &sum_diff, int day);
        void search_day_to_add_rest(Solution &s, int nurse);
        void update_pre_parsed_solution(Solution &s);
        void update_cost(Solution &s);

        // DEBUG
        bool check_solution_cost(Solution &s);

};

#endif // NRP_H

















