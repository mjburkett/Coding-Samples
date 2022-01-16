using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScholarEncounterP2 : Encounter {
    public override void SelectOption(int choice) {
        Player p = gMgr.player;
        switch (choice) {
            // Go With Him
            case 0:
                p.gold -= 5;
                if (p.SkillCheck(SkillType.magic, 8)) {
                    p.magSkill += 1;
                    uMgr.ConcludeEncounter("Inspired by his teachings, you were able to glean some wisdom and knowledge of the arcane from the " +
                        "scholar, though this use of your time has cost you 15 gold.");
                }
                else {
                    uMgr.ConcludeEncounter("Unfortunately, your crude mind was unable to comprehend what he had to teach you. Despite your " +
                        "graciousness and generocity, you leave unrewarded. Your time has cost you 15 gold.");
                }
                break;
            // Demand Payment
            case 1:
                p.gold += 25;
                uMgr.ConcludeEncounter("In spite of your momentary rudeness, he offers 15 gold in payment, certainly more than what you would " +
                    "have earned otherwise.");
                break;
            default:
                Debug.Log("Invalid option selected: " + choice);
                break;
        }
    }
}