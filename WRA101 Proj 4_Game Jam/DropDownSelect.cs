using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DropDownSelect : MonoBehaviour{

    private string teammateName;
    public GameObject panel;
    public Canvas cnvs;
    public Dropdown dropdown;
    private TeammateManager mgr;

    void Start() {
        mgr = cnvs.GetComponent<TeammateManager>();
        PopulateList();
    }

    public void PopulateList() {
        List<string> names = new List<string>();
        for(int i = 1; i <= 4; i++) {
            names.Add(mgr.getName(i));
        }
        dropdown.AddOptions(names);
        HandleInputData(0);
    }

    public void HandleInputData(int val) {
        panel.GetComponent<TimeShop>().setSelection(val);
    }
}
