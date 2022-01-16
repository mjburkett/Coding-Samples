using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScholarEncounter : Encounter {
    public GameObject P2;
    public override void SelectOption(int choice) {
        Player p = gMgr.player;
        switch (choice) {
            // Help Him
            case 0:
                p.gold -= 10;
                uMgr.BeginEncounter(P2);
                return;
            // Carry On
            case 1:
                p.gold += 5;
                uMgr.ConcludeEncounter("You ignore his pleas, determined to have your time work for you alone. You gain 5 gold from your productive day.");
                break;
            default:
                Debug.Log("Invalid option selected: " + choice);
                break;
        }
    }
}
