
let nurse_index = 1;
function add_nurse()
{
    cont_enfermeiras = document.getElementById("container-nurses");
    let div = document.createElement("div");
    div.id = "div_nurse" + nurse_index;

    //ID
    let label_nurse_id = document.createElement("label");
    label_nurse_id.innerHTML = "Nome " + nurse_index + " :";
    let text_nurse_id = document.createElement("input");
    text_nurse_id.type = "text";
    text_nurse_id.size = "1";

    // Preferred days off
    let label_nurse_days_off= document.createElement("label");
    label_nurse_days_off.innerHTML = "Dias de folga :  ";
    let text_nurse_days_off = document.createElement("input");
    text_nurse_days_off.type = "text";
    text_nurse_days_off.size = "1";
    text_nurse_days_off.id = "preferred_days_off";

    // Vacation days
    let label_nurse_vacation = document.createElement("label");
    label_nurse_vacation.innerHTML = "Férias : ";
    let text_nurse_vacation = document.createElement("input");
    text_nurse_vacation.type = "text";
    text_nurse_vacation.size = "5";

    div.appendChild(label_nurse_id);
    div.appendChild(text_nurse_id);
    div.appendChild(label_nurse_days_off);
    div.appendChild(text_nurse_days_off);
    div.appendChild(label_nurse_vacation);
    div.appendChild(text_nurse_vacation);


    cont_enfermeiras.appendChild(div);


    nurse_index ++;
}

function remove_nurse()
{
    if(nurse_index != 1 )
    {
        let cont_nurses = document.getElementById('container-nurses');
        cont_nurses.removeChild(cont_nurses.lastChild);
        nurse_index --;
    }
}

function generate_instance()
{
    let root = new Object();
    let input = new Object();
    let data = new Object();

    data.n_days = parseInt(document.getElementById("n_days").value);
    data.frac_days_off = parseFloat(document.getElementById("frac_days_off").value);
    data.frac_days_off_weekend = parseFloat(document.getElementById("frac_days_off_weekend").value);
    data.max_consecutive_work_days = parseInt(document.getElementById("max_consecutive_work_days").value);




    // FDS
    let v_weekend = [];
    if(document.getElementById("weekend").value != "")
    {
        v_weekend = document.getElementById("weekend").value.split(",").map(Number);
    }
    data.weekend = v_weekend;

    //SHIFT COVERAGE
    let shift_coverage = new Object();
    shift_coverage.mor_min = [];
    shift_coverage.mor_max = [];
    shift_coverage.aft_min = [];
    shift_coverage.aft_max = [];

    if(document.getElementById("cob_min_manha").value != "")
    {
        shift_coverage.mor_min = document.getElementById("cob_min_manha").value.split(",").map(Number);
    }
    if(document.getElementById("cob_max_manha").value != "")
    {
        shift_coverage.mor_max = document.getElementById("cob_max_manha").value.split(",").map(Number);
    }
    if(document.getElementById("cob_min_tarde").value != "")
    {
        shift_coverage.aft_min = document.getElementById("cob_min_tarde").value.split(",").map(Number);
    }
    if(document.getElementById("cob_max_tarde").value != "")
    {
        shift_coverage.aft_max = document.getElementById("cob_max_tarde").value.split(",").map(Number);
    }

    

    
    // Enfermeiras
    let nurses = [];
    for(let i = 0; i  < document.getElementById("container-nurses").children.length;i++)
    {
        let children = document.getElementById("container-nurses").children[i];
        let nurse = new Object();

        //ID 
        nurse.id = children.getElementsByTagName("input")[0].value;

        //Dias de folga
        nurse.preferred_days_off = [];
        if(children.getElementsByTagName("input")[1].value != "")
        {
            nurse.preferred_days_off = children.getElementsByTagName("input")[1].value.split(",").map(Number);
        }
        
        //Férias
        nurse.vacation_days = [];
        if(children.getElementsByTagName("input")[2].value != "")
        {
            nurse.vacation_days = children.getElementsByTagName("input")[2].value.split(",").map(Number);
        }
        
        
        nurses.push(nurse);
    }

    data.shift_coverage = shift_coverage;
    data.nurses = nurses
    input.data = data 
    root.input = input;
    

   export2txt(root);
   //log("opening file...");
   //file.open(); 
   //log("writing file..");
   //file.writeline(str);
   //file.close();

}

function add_cte_manha()
{
    const min_manha = document.getElementById("cte_min_manha").value;
    const max_manha = document.getElementById("cte_max_manha").value;

    const n_dias = parseInt(document.getElementById("n_days").value)

    console.log(generate_string_cte_values(n_dias, min_manha));
    document.getElementById("cob_min_manha").value = generate_string_cte_values(n_dias, min_manha);
    document.getElementById("cob_max_manha").value = generate_string_cte_values(n_dias, max_manha);

}

function generate_string_cte_values(n_dias, cte)
{
    let str_const_min_manha = ""
    for(let i = 0; i < n_dias; i++)
    {
        if(i == n_dias - 1)
        {
            str_const_min_manha += cte;
        }else
        {
            str_const_min_manha += cte + ",";
        }
    }
    return str_const_min_manha
}

function add_cte_tarde()
{

    const min_tarde = document.getElementById("cte_min_tarde").value;
    const max_tarde = document.getElementById("cte_max_tarde").value;

    const n_dias = parseInt(document.getElementById("n_days").value);

    document.getElementById("cob_min_tarde").value = generate_string_cte_values(n_dias, min_tarde);
    document.getElementById("cob_max_tarde").value = generate_string_cte_values(n_dias, max_tarde);
}

function export2txt(originalData) 
{
    const a = document.createElement("a");
    a.href = URL.createObjectURL(new Blob([JSON.stringify(originalData, null, 2)], {
      type: "text/plain"
    }));
    a.setAttribute("download", "instance.json");
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
  }