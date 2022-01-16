using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TeammateManager : MonoBehaviour {

    private Teammate[] tmmt = new Teammate[4];

    public class Teammate{
        public Dictionary<string, string> skills = new Dictionary<string, string>();
        private string[] maleNames = new string[] { "Liam", "Noah", "William", "James", "Oliver", "Ben", "Elijah", "Lucas" };
        private string[] femaleNames = new string[] { "Emma", "Olivia", "Ava", "Isabella", "Sophia", "Karen", "Mia", "Amelia" };
        public string name;
        private int art=0;//private int art2d;//private int art3d;//private int artaudio;
        private int design=0;//private int designConcepts;//private int designMechanics;//private int designLevel;    //"Level Design"
        private int technical=0;//private int techEngine;     //"Game Engine"//private int techProgramming;//private int techSoftware;   //"Other software"
        private int allocatablePoints = 100;
        private string gen;

        public Teammate(string gender) {
            float artCheck = 1f;
            float designCheck = 1f;
            float techCheck = 1f;
            for (int i = 0; i < allocatablePoints; i++) {
                float randNum = Random.Range(0, 3);
                if (randNum < (artCheck + (4 * art / allocatablePoints))) {
                    art++;
                }
                else if(randNum < (artCheck + techCheck + (4 * technical / allocatablePoints))){
                    technical++;
                }
                else{
                    design++;
                }
            }
            if (gender == "m") {
                name = maleNames[Random.Range(0, maleNames.Length)];
            }
            else {
                name = femaleNames[Random.Range(0, femaleNames.Length)];
            }
            gen = gender;
        }
        public int getArt() {
            return art;
        }
        public int getTech() {
            return technical;
        }
        public int getDesign() {
            return design;
        }
        public string getName() {
            return name;
        }
        public void changeName() {
            if (gen == "m") {
                name = maleNames[Random.Range(0, maleNames.Length)];
            }
            else {
                name = femaleNames[Random.Range(0, femaleNames.Length)];
            }
        }
    }

    void Start(){
        Teammate tM1 = new Teammate("m");
        Teammate tM2 = new Teammate("m");
        Teammate tM3 = new Teammate("f");
        Teammate tM4 = new Teammate("f");
        tmmt[0] = tM1;
        tmmt[1] = tM2;
        tmmt[2] = tM3;
        tmmt[3] = tM4;
        for(int i = 0; i < tmmt.Length; i++) {
            foreach (Teammate t in tmmt) {
                if (t != tmmt[i] && t.getName() == tmmt[i].getName()) {
                    tmmt[i].changeName();
                    i = 0;
                    break;
                }
            }
        }
    }

    public List<Teammate> getTeammates() {
        List<Teammate> retTeam = new List<Teammate>();
        foreach(Teammate t in tmmt) {
            retTeam.Add(t);
        }
        return retTeam;
    }

    public int getValues(int skillChoice, int tmmtNum) {
        tmmtNum--;
        if(skillChoice == 0) {
            return tmmt[tmmtNum].getArt();
        }
        else if(skillChoice == 1) {
            return tmmt[tmmtNum].getTech();
        }
        else if(skillChoice == 2) {
            return tmmt[tmmtNum].getDesign();
        }
        else {
            Debug.Log("Bug in TeammateManager under getValues(). skillChoice out of range.");
            return 0;
        }
    }

    public string getName(int tmmtNum) {
        tmmtNum--;
        return tmmt[tmmtNum].getName();
    }
    
    // Update is called once per frame
    void Update()
    {
        
    }
}
