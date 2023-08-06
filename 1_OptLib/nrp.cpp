#include "nrp.h"

NRP::NRP()
{
    srand(10);
}

NRP::NRP(InputData *xp_dt): dt(xp_dt)
{
    srand(10);
}

void NRP::optimize()
{
    // Optimizes the problem
    Solution solution = initial_solution();
    update_pre_parsed_solution(solution); //Update dt (accesible from the outside)

}

void NRP::assign_nurse_shifts(vector<int> &v_assigned_shift, vector<int> &v_unassigned_shift, vector<int> &v_diff, int &sum_diff, int start_day, int &sum_nurse_rest)
{

    /*
     * Assign the work days and rests for the nurse on shift v_assigned_shift. This is a row of the
     * matrix. Also the row of the unassigned matrix is changed (v_unassigned_shift), all rests in
     * v_assigned_shift are also updated in v_unassigned_shift. The ds v_diff, sum_diff are also updated.
     * the sum_diff is only summed over POSITIVE elements of v_diff.
     *
     * - Start day is the selected day that should have a work, so start before that day (Start day - 1)
     * a rest is assigned (except if Start day = 0), and then a counter of consecutive work days is done
     * until it reaches the maximum permissible, than a rest os set and the process is restarted.
     *
     * - When a value is -2 on the vector, it means that the nurse is on vacation, NOT CHANGED
     * - The counter is updated only when the matrix has a -1 value.
     * - If the matrix has a -2 or 0 the counter is restarted.
     */

    int days_checked = 0;
    int n_consecutive_work_days = 0;
    int current_day = (start_day == 0)?0:start_day - 1;

    while(days_checked < dt->n_days)
    {
        // Assign the rest day: verify if the current index is not 0 nor -2
        int last_assigned_code = v_assigned_shift[current_day];
        while( (v_assigned_shift[current_day] != -1) && (days_checked < dt->n_days))
        {
            last_assigned_code = v_assigned_shift[current_day]; //check if the last day (before an assigment) is vacation
            current_day = (current_day == dt->n_days - 1)?0:current_day + 1;
            days_checked += 1;
        }

        // There shoud be at least one day of work for the nurse
        if(last_assigned_code != -2) // Start at vacation, no need to add rest right after
        {
            v_assigned_shift[current_day]   = 0;
            v_unassigned_shift[current_day] = 0;
            days_checked += 1;
            current_day = (current_day == dt->n_days - 1)?0:current_day + 1;

            sum_nurse_rest += 1;
        }


        // Assign the sequence of 1s
        n_consecutive_work_days = 0;
        while( (n_consecutive_work_days < dt->max_consecutive_work_days) && (days_checked < dt->n_days))
        {
            if(v_assigned_shift[current_day] == - 1)
            {
                v_assigned_shift[current_day]   = 1;
                v_diff[current_day] = v_diff[current_day] - 1;
                sum_diff = (v_diff[current_day] < 0) ? sum_diff:sum_diff - 1;
                n_consecutive_work_days += 1;
            }else
            {
                n_consecutive_work_days = 0;
            }
            n_consecutive_work_days = (current_day == dt->n_days - 1)?0:n_consecutive_work_days;
            current_day = (current_day == dt->n_days - 1)?0:current_day + 1;
            days_checked += 1;
        }
    }

}

void NRP::assign_extra_shift(vector<vector<int> > &vv_import_extra_shift, vector<vector<int> > &vv_export_extra_shift, vector<int> &v_import_extra_shift_diff, int &import_extra_shift_sum_diff, vector<int> &v_shift_assigned_to_nurse, vector<int> &v_nurse_extra_shift, int export_extra_shift)
{

    /*
     *  Assign extra shifts. The matrix that NEEDS the shifts is vv_import_extra_shift
     *  (is the matrix that will be changed). vv_export_extra_shift is the matrix
     *  containing a nurse that will work on vv_import_extra_shift.
     */

    size_t n_nurse   = 0;
    size_t nurses_tried = 0; // number of nurses of vv_export_extra_shift that were tried to make the e

    for(size_t i = 0; i < v_import_extra_shift_diff.size();i++)
    {
        if(v_import_extra_shift_diff[i] > 0) // at least one nurse is required on day i
        {
            nurses_tried = 0;
            while( (v_import_extra_shift_diff[i] > 0) && (nurses_tried < vv_export_extra_shift.size()))
            {
                if( (v_shift_assigned_to_nurse[n_nurse] == export_extra_shift) && (vv_export_extra_shift[n_nurse][i] == 1)) // the nurse is assigned to export_extra_shift AND is working that day
                {
                    // Put the nurse from export_extra_shift to make extra shift on
                    // import_extra_shift
                    vv_import_extra_shift[n_nurse][i] = 1;
                    v_import_extra_shift_diff[i] = v_import_extra_shift_diff[i] - 1;
                    import_extra_shift_sum_diff = import_extra_shift_sum_diff - 1;
                    v_nurse_extra_shift[n_nurse] += 1;
                }

                n_nurse = (n_nurse == vv_export_extra_shift.size() - 1)?0:n_nurse + 1;
                nurses_tried ++;
            }
        }
    }


}

void NRP::add_mising_days_off(Solution &s)
{

    /*
     * Respecting the rule of maximum worked days does not
     * necessarily achieve minimum days-off on the period.
     * This routine adds the missing days of for each nurse.
     * It searches places were there are more than the minimum
     * of nurses working.
     *
     */


    for(size_t i = 0; i < s.v_nurse_sum_rest_days.size(); i++)
    {
        while(s.v_nurse_sum_rest_days[i] < dt->v_nurses[i].n_days_off)
        {
            search_day_to_add_rest(s,i);
        }
    }


}

void NRP::calculate_days_off_on_weekend(Solution &s)
{

    /*
     * Calculate, for each nurse, the number of rest days that
     * coincides with weekends.
     */

    for(size_t i = 0; i < s.v_nurse_sum_rest_days_weekends.size(); i++) // for each nurse
    {
        for(size_t j = 0; j < dt->v_weekend.size(); j++) // for each weekend day
        {
            size_t weekend_day = dt->v_weekend[j];
            if(s.vv_aft_assig[i][weekend_day] == 0)
            {
                s.v_nurse_sum_rest_days_weekends[i] += 1;
            }
        }
    }



}

NRP::Solution NRP::initial_solution_constructive_1()
{

    /*===========================================================================
     * Creates an initial solution considering:
     * - Minimum number of weekends per nurses
     * A sum is upfated for each matrix, considering the difference
     * of demand required on the shift and the assigned. A sum  of all
     * positive values indicate the shift that needs "the most" that a
     * nurse work on it, so alocate the nurse on that shift. The allocation
     * is started at a random day (with probability according to the difference
     * vector), and assign 0 (no work) on that day, and n_permissible consecutive
     * work days after. The pattern is repeated until all days are allocated.
     * Not worked days are also assigned to the other matrix. Values with
     * -2 are not changed (vacation days).
     *
     * DO NOT ASSIGN EXTRA SHIFTS!!!!! THAT IS MADE AFTER THIS
     * ===========================================================================
     */

    Solution s;
    initialize_solution_ds(s);


    s.v_shift_assigned_to_nurse = vector<int>(dt->v_nurses.size(),-1); // 0: morning, 1: afternoon

    s.v_mor_diff = vector<int>(dt->n_days,0);
    s.v_aft_diff = vector<int>(dt->n_days,0);
    s.sum_mor_diff = 0;
    s.sum_aft_diff = 0;

    for(size_t i = 0; i < s.v_mor_diff.size(); i++)
    {
        s.v_mor_diff[i] = dt->v_mor_min[i];
        s.v_aft_diff[i] = dt->v_aft_min[i];
        s.sum_mor_diff  += s.v_mor_diff[i];
        s.sum_aft_diff  += s.v_aft_diff[i];
    }

    for(size_t i = 0; i < dt->v_nurses.size(); i++)
    {
        if(s.sum_mor_diff > s.sum_aft_diff) // Assigns to the morning shift (0)
        {
            s.v_shift_assigned_to_nurse[i] = 0;
            vector<double> v_diff_freq = get_rel_freq_vector(s.v_mor_diff);
            int start_day = select_index_from_freq_vector(v_diff_freq);
            assign_nurse_shifts(s.vv_mor_assig[i],s.vv_aft_assig[i],s.v_mor_diff,s.sum_mor_diff,start_day,s.v_nurse_sum_rest_days[i]);

        }
        else // Assigns to the afternoon shift shift (1)
        {
            s.v_shift_assigned_to_nurse[i] = 1;
            vector<double> v_diff_freq = get_rel_freq_vector(s.v_aft_diff);
            int start_day = select_index_from_freq_vector(v_diff_freq);
            assign_nurse_shifts(s.vv_aft_assig[i],s.vv_mor_assig[i],s.v_aft_diff,s.sum_aft_diff,start_day,s.v_nurse_sum_rest_days[i]);
        }
    }


    add_mising_days_off(s);

    // relocate_rest(s.vv_mor_assig, s.v_mor_diff, s.sum_mor_diff, s.v_shift_assigned_to_nurse, 0); // morning shift
    // relocate_rest(s.vv_aft_assig, s.v_aft_diff, s.sum_aft_diff, s.v_shift_assigned_to_nurse, 1); // afternoon shift

    calculate_days_off_on_weekend(s);
    update_cost(s);

    return s;


}

NRP::Solution NRP::initial_solution()
{
    Solution s;
    Solution s_current;

    double max_cost = -MAX_DOUBLE;
    double min_cost =  MAX_DOUBLE;
    for(int i = 0; i < 100; i++)
    {
        if(i == 10)
        {
            int a;
            a = 10;
        }
        s_current = Solution();
        s_current = initial_solution_constructive_1();

        //DEBUG ===
        if(!check_solution_cost(s_current))
        {
            cout << i << "\n";
        }
        //DEBUG ===

        if(s_current.cost < min_cost)
        {
            min_cost = s_current.cost;
            s = s_current;
        }
        if(s_current.cost > max_cost) max_cost = s_current.cost;
    }


    // Assign extra shifts on the best construction solution found
    //assign_extra_shift(s.vv_mor_assig,s.vv_aft_assig,s.v_mor_diff,s.sum_mor_diff,s.v_shift_assigned_to_nurse, s.v_nurse_extra_shifts,1);
    //assign_extra_shift(s.vv_aft_assig,s.vv_mor_assig,s.v_aft_diff,s.sum_aft_diff,s.v_shift_assigned_to_nurse,s.v_nurse_extra_shifts,0);

    update_cost(s);

    cout << min_cost << " - " << max_cost;
    return s;
}

void NRP::initialize_solution_ds(Solution &s)
{

    /*
     * Initialize all data structures of the solution:
     *  - Matrices vv_aft_assign, vv_mor_assig
     */

    // Initialize the matrices with -1
    s.vv_aft_assig = vector<vector<int>>(dt->v_nurses.size(), vector<int>(dt->n_days,-1));
    s.vv_mor_assig = vector<vector<int>>(dt->v_nurses.size(), vector<int>(dt->n_days,-1));

    s.v_nurse_sum_rest_days = vector<int>(dt->v_nurses.size(),0);
    s.v_nurse_sum_rest_days_weekends = vector<int>(dt->v_nurses.size(),0);


    s.v_nurse_extra_shifts = vector<int>(dt->v_nurses.size(),0);

    // Assign vacation days for the nurses as -2
    for(size_t i = 0; i < dt->v_nurses.size(); i++ )
    {
        for(size_t j = 0; j < dt->v_nurses[i].v_vacation_days.size(); j++)
        {
            int vacation_day = dt->v_nurses[i].v_vacation_days[j];
            s.vv_aft_assig[i][vacation_day] = -2;
            s.vv_mor_assig[i][vacation_day] = -2;
        }
    }


}

void NRP::relocate_rest(vector<vector<int> > &vv_assign, vector<int>  &v_diff, int &sum_diff, vector<int> &v_shift_assigned_to_nurse, int shift)
{

    /*
     * Try to relocate the rest days in order to minimize
     * the positive values in v_diff (positive values indicates that the minimum
     * of nurses are not yet achieved by that period of the day)
     * vv_assign is one of the two matrices (morning or afternoon)
     * "v_shift_assigned_to_nurse" -> the shift (0 or 1) assigned to that nurse
     * " shift" 0> the shift (0 or 1) that is being considered)
     */


    for(size_t i = 0; i < v_diff.size();i++)
    {
        if(v_diff[i] > 0) // at least one nurse is required on day i
        {
            size_t n_nurse = 0;
            while( (n_nurse < vv_assign.size()) && (v_diff[i] > 0))
            {
                if( (v_shift_assigned_to_nurse[n_nurse] == shift) && (vv_assign[n_nurse][i] == 0)) // If the nurse is assigned to that shift AND she is not working
                {
                    relocate_nurse_rest(vv_assign[n_nurse], v_diff, sum_diff, i);
                }
                n_nurse++;
            }
        }
    }




}

void NRP::relocate_nurse_rest(vector<int> &v_assign, vector<int> &v_diff, int &sum_diff, int day)
{

    /*
     * The nurse is not working at "day". "Day" is currently missing nurses to
     * work, so this routine tries to relocate the rest from day to another day that
     * has an excess of nurses working (v_diff > 0). Tha is tried before and after
     * "day". The new allocation of the rest will increase the consecutive work
     * days of one of the sides. A feasibility check must be made to ensure
     * that the maximum consecutive work days is not excedeed
     */


    // Days with excess of nurses (possibility of rest shifting)
    vector<int> v_left_excess;
    vector<int> v_right_excess;


    int left_count = 0;   // consecutive days until previous rest (0) OR vacation (-2), starting at "day"
    int right_count = 0;  // consecutive days until next rest (0) OR vacation (-2), starting at "day"

    // Left of day
    int cont = day;
    if(cont != 0)
    {
        cont --;
        while( (cont >= 0) && (v_assign[cont] == 1))
        {
            if(v_diff[cont] < 0) // there is an excess of nurses on this day
            {
                v_left_excess.push_back(cont);
            }
            cont--;
            left_count++;

        }
    }

    // Right of day
    cont = day;
    if(cont < static_cast<int>(v_assign.size()) - 1)
    {
        cont ++;
        while( (cont < static_cast<int>(v_assign.size())) && (v_assign[cont] == 1))
        {
            if(v_diff[cont] < 0) // there is an excess of nurses on this day
            {
                v_right_excess.push_back(cont);
            }
            cont++;
            right_count ++;
        }
    }


    // Try to make the relocation on the left side
    bool relocation_success = false;
    if(v_left_excess.size() != 0)
    {
        cont = 0; // start with the nearest element from "day"
        while((static_cast<size_t>(cont) < v_left_excess.size()) && (relocation_success == false) )
        {
            int gap_new_rest_position = day - v_left_excess[cont];
            // gap_new_rest_position the number of work days that will increase on the
            // right side of days
            int consecutive_work_days_right_side = right_count + gap_new_rest_position;
            if(consecutive_work_days_right_side <= dt->max_consecutive_work_days)
            {
                // Make the change
                int new_rest_index = v_left_excess[cont];
                v_assign[new_rest_index] = 0;
                v_diff[new_rest_index]   = v_diff[ new_rest_index ] + 1;

                v_assign[day] = 1;
                v_diff[day]   = v_diff[day] - 1;
                relocation_success = true;

                // If the relocation was successful, the sum_dif will reduce
                // by one unit
                sum_diff--;
            }
            cont++;
        }
    }

    // Try to make the relocation on the right side

    if( (v_right_excess.size() != 0) && (relocation_success ==  false))
    {
        cont = 0; // start with the nearest element from "day"
        while((static_cast<size_t>(cont) < v_right_excess.size()) && (relocation_success == false) )
        {
            int gap_new_rest_position = day - v_right_excess[cont];
            // gap_new_rest_position the number of work days that will increase on the
            // right side of days
            int consecutive_work_days_left_side = left_count + gap_new_rest_position;
            if(consecutive_work_days_left_side <= dt->max_consecutive_work_days)
            {
                // Make the change
                int new_rest_index = v_right_excess[cont];
                v_assign[new_rest_index] = 0;
                v_diff[new_rest_index]   = v_diff[ new_rest_index ] + 1;

                v_assign[day] = 1;
                v_diff[day]   = v_diff[day] - 1;
                relocation_success = true;

                // If the relocation was successful, the sum_dif will reduce
                // by one unit
                sum_diff--;
            }
            cont++;
        }
    }









}

void NRP::search_day_to_add_rest(Solution &s, int nurse)
{

    /*
     * Assign a rest to a nurse. There is a hierarchy to try that:
     *
     *  1 -  try to assign on a (WEEKEND) AND (THERE IS AN EXCESS OF NURSES WORKING THAT DAY).
     *  2 -  assign to the minimum value of diff (if the minimum is negative, there is an excess
     *  of nurse, if not, it is missing, but the minimum would give the day that needs the "least"
     *  a new nurse.
     *
     *   DOES NOT UPDATE THE V_REST_WEEKEND, THAT IS UPDATED AFTER!
     */


        // 1 - Try to add on a weekend, if the weekend has an excess of nurses (v_sum_diff > 0) ==============================================
        if(s.v_shift_assigned_to_nurse[nurse] == 0) // Check wether to look on morning or afternoon v_diff_vectors
        {
            for(size_t i = 0; i < dt->v_weekend.size();i++)
            {
                size_t weekend_day = dt->v_weekend[i];
                if(s.v_mor_diff[weekend_day] < 0) // There is an excess of nurses
                {
                    s.vv_mor_assig[nurse][weekend_day] = 0;
                    s.vv_aft_assig[nurse][weekend_day] = 0;
                    s.v_mor_diff[weekend_day] += 1;
                    s.v_nurse_sum_rest_days[nurse] += 1;
                    return;
                    // Sum mor diff does not change, because the diff only counts positive values (here, at most it becomes 0).
                }
            }
        }else
        {
            for(size_t i = 0; i < dt->v_weekend.size();i++)
            {
                size_t weekend_day = dt->v_weekend[i];
                if(s.v_aft_diff[weekend_day] < 0) // There is an excess of nurses
                {
                    s.vv_aft_assig[nurse][weekend_day] = 0;
                    s.vv_mor_assig[nurse][weekend_day] = 0;
                    s.v_aft_diff[weekend_day] += 1;
                    s.v_nurse_sum_rest_days[nurse] += 1;
                    return;
                    // Sum mor diff does not change, because the diff only counts positive values (here, at most it becomes 0).
                }
            }
        }


        // 2 -  assign to the minimum value of diff
        if(s.v_shift_assigned_to_nurse[nurse] == 0) // Check wether to look on morning or afternoon v_diff_vectors
        {
            long int min_diff = MAX_LONG_INT;
            size_t index_min_diff = -1;
            for(size_t i = 0; i < s.v_mor_diff.size();i++)
            {
                if(s.v_mor_diff[i] < min_diff)
                {
                    min_diff       = s.v_mor_diff[i];
                    index_min_diff = i;
                }
            }

            s.vv_mor_assig[nurse][index_min_diff] = 0;
            s.vv_aft_assig[nurse][index_min_diff] = 0;
            s.v_mor_diff[index_min_diff]          += 1;
            s.v_nurse_sum_rest_days[nurse]        += 1;

            // check if the v_diff is now > 0, if it is, the sum_diff should be updated
            s.sum_mor_diff = (s.v_mor_diff[index_min_diff] > 0)? s.sum_mor_diff + 1:s.sum_mor_diff;

            // Check if the assigned day is at a weekend
            for(size_t i = 0; i < dt->v_weekend.size();i++)
            {
                size_t weekend_day = dt->v_weekend[i];
                if(index_min_diff == weekend_day) // There is an excess of nurses
                {
                    s.v_nurse_sum_rest_days_weekends[nurse] += 1;
                    return;
                }
            }


            return;
            // Sum mor diff does not change, because the diff only counts positive values (here, at most it becomes 0).

        }else
        {
            long int min_diff = MAX_LONG_INT;
            size_t index_min_diff = -1;
            for(size_t i = 0; i < s.v_aft_diff.size();i++)
            {
                if(s.v_aft_diff[i] < min_diff)
                {
                    min_diff       = s.v_aft_diff[i];
                    index_min_diff = i;
                }
            }

            s.vv_mor_assig[nurse][index_min_diff] = 0;
            s.vv_aft_assig[nurse][index_min_diff] = 0;
            s.v_aft_diff[index_min_diff]          += 1;
            s.v_nurse_sum_rest_days[nurse]        += 1;

            // check if the v_diff is now > 0, if it is, the sum_diff should be updated
            s.sum_aft_diff = (s.v_aft_diff[index_min_diff] > 0)? s.sum_aft_diff + 1:s.sum_aft_diff;

            return;
          }

}

void NRP::update_pre_parsed_solution(Solution &s)
{

    dt->vv_aft_shift = s.vv_aft_assig;
    dt->vv_mor_shift = s.vv_mor_assig;

    dt->v_mor_diff = s.v_mor_diff;
    dt->v_aft_diff = s.v_aft_diff;


    dt->v_shift_assigned_to_nurse      = s.v_shift_assigned_to_nurse;
    dt->v_nurse_sum_rest_days          = s.v_nurse_sum_rest_days;
    dt->v_nurse_sum_rest_days_weekends = s.v_nurse_sum_rest_days_weekends;
    dt->v_nurse_extra_shifts           = s.v_nurse_extra_shifts;

    // DEBUG - OBJECTIVE FUNCTION COMPONENTS
    dt->sum_mor_diff             = s.sum_mor_diff;             // A1
    dt->sum_aft_diff             = s.sum_aft_diff;             // A2

    dt->sum_extra_shifts         = s.sum_extra_shifts;         // B Number of extra shifts performed by the nurse
    dt->extra_shift_load_balance = s.extra_shift_load_balance; // C;
    dt->weekend_rest_violation   = s.weekend_rest_violation;   // D;
    dt->prefered_rest_violation  = s.prefered_rest_violation;  // E;
    dt->cost = s.cost;



}

void NRP::update_cost(Solution &s)
{

    /*
     * Update the cost of the solution, the cost is made of:
     * A1: Sum of differences of number of nurses required and assigned (only positive values) from morning.
     * A2: Sum of differences of number of nurses required and assigned (only positive values) from afternoon.
     * B:  Sum of extra shifts performed by the nurses.
     * C:  Considering V as the v_extra_shifts:
     *     sum(max(V) - V_i))  -> measures the workload of extra shifts, the more distributed the lesser
     *     the value C will be.
     * D: Considering V as the v_nurse_sum_rest_weekends, and nw the number of rests on weekends that every nurse
     *    should have.
     *    sum (V_i - nw), if (V_i - nw) > 0
     * E: Number of days that a nurse wanted to rest but is not resting.
     *
     *
     *  cost = A1 + A2 + B + C + D + E (with weights on each term, to be added...)
     *
     *
     * If (A1 > 0) OR (A2 > 0), it means that the solution is NOT FEASIBLE: even with extra shifts, some days
     * have not met the minimum number of nurses.
     */

    // Get the maximum of v_nurse_extra_shift
    long int max_extra_shift = -MAX_LONG_INT;
    for(size_t i = 0; i < s.v_nurse_extra_shifts.size(); i++)
    {
        if(s.v_nurse_extra_shifts[i] > max_extra_shift) max_extra_shift = s.v_nurse_extra_shifts[i];
    }

    // A1 and A2 are already calculated (sum_mor_diff AND sum_aft_diff)

    s.sum_extra_shifts         = 0; // B
    s.extra_shift_load_balance = 0; // C
    s.weekend_rest_violation   = 0; // D
    s.prefered_rest_violation  = 0; // E
    for(size_t nurse = 0; nurse < s.v_nurse_extra_shifts.size(); nurse++)
    {
        // B
        s.sum_extra_shifts         +=  s.v_nurse_extra_shifts[nurse];

        // C
        s.extra_shift_load_balance += (max_extra_shift - s.v_nurse_extra_shifts[nurse]);

        // D
        int weekend_assigned_difference = dt->v_nurses[nurse].n_days_off_weekend - s.v_nurse_sum_rest_days_weekends[nurse];
        s.weekend_rest_violation = weekend_assigned_difference > 0? s.weekend_rest_violation + weekend_assigned_difference:s.weekend_rest_violation;

        // E
        for(size_t i = 0; i < dt->v_nurses[nurse].v_preferred_days_off.size(); i++)
        {
            int preferred_day_off = dt->v_nurses[nurse].v_preferred_days_off[i];
            if(s.vv_aft_assig[nurse][preferred_day_off] != 0)
            {
                s.prefered_rest_violation += 1;
            }
        }

    }

    s.cost = s.sum_mor_diff + s.sum_aft_diff + s.sum_extra_shifts + s.extra_shift_load_balance + s.weekend_rest_violation + s.prefered_rest_violation;
}

bool NRP::check_solution_cost(Solution &s)
{

    int sum_aft_diff = 0;
    int sum_mor_diff = 0;

    for(int i = 0; i < s.v_aft_diff.size();i++)
    {
        if(s.v_aft_diff[i] > 0)
        {
            sum_aft_diff += s.v_aft_diff[i];
        }
        if(s.v_mor_diff[i] > 0)
        {
            sum_mor_diff += s.v_mor_diff[i];
        }

    }

    if(sum_aft_diff != s.sum_aft_diff | sum_mor_diff != s.sum_mor_diff)
    {
        return false;
    }
    return true;


}
