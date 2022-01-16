using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TaskData : MonoBehaviour{

    //I decided to use the Task object in the later parts of making this script. This can be made more efficient by getting rid of the set(desc, cost, and skill) functions and using the task object for this data.
    private string description;
    private int cost;
    private string skill;
    private TimeShop shop;
    private Task thisTask;
    private GameObject shopPanel;

    public void setDesc(string desc) {
        description = desc;
    }
    public void setCost(int c) {
        cost = c;
    }
    public void setSkill(string s) {
        skill = s;
    }
    public void setPanel(GameObject p) {
        shopPanel = p;
        shop = shopPanel.GetComponent<TimeShop>();
    }
    public void setTask(Task t) {
        thisTask = t;
    }

    public void displayData() {
        Text[] textBoxes = GetComponentsInChildren<Text>();
        textBoxes[0].text = description;
        textBoxes[1].text = "Cost: " + cost + " hour";
        if (skill == "a") {
            textBoxes[2].text = "Skill: Art";
        }
        else if (skill == "t") {
            textBoxes[2].text = "Skill: Technical";
        }
        else if (skill == "d") {
            textBoxes[2].text = "Skill: Design";
        }
    }
    
    public void consume() {
        shop.consumeButton(thisTask);
        Destroy(gameObject);
    }
}
