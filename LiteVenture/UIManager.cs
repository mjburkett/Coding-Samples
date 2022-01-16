using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour {
    public GameManager mgr;
    public Player player;

    public GameObject canvas;
    public GameObject classSelect;
    public GameObject mainMenu;
    public GameObject shop;
    public GameObject gameOverScreen;
    public Text playerStats;
    private GameObject activeEncounter;

    public List<GameObject> encounters;
    public GameObject encounterConclusion;

    public void UpdateStats() {
        Text[] toUpdate = playerStats.GetComponentsInChildren<Text>();
        toUpdate[1].text = "Health : " + player.currHealth + "/" + player.maxHealth;
        toUpdate[2].text = "Gold: " + player.gold + "g";
        toUpdate[3].text = "Strength - " + player.strenSkill;
        toUpdate[4].text = "Dexterity - " + player.dexSkill;
        toUpdate[5].text = "Magic - " + player.magSkill;
    }

    public void OpenShop() {
        shop.SetActive(true);
        mainMenu.SetActive(false);
    }

    public void CloseShop() {
        mainMenu.SetActive(true);
        UpdateStats();
        shop.SetActive(false);
    }

    public void SelectEncounter() {
        int selection = 0;
        while (true) {
            selection = Random.Range(0, encounters.Count);
            // Conditional selections
            if (selection == 2 && !mgr.golemAngry) {
                continue;
            }
            break;
        }
        BeginEncounter(encounters[selection]);
    }

    public void BeginEncounter(GameObject encounter) {
        if (activeEncounter) {
            Destroy(activeEncounter);
        }

        mainMenu.SetActive(false);
        activeEncounter = Instantiate(encounter, canvas.transform);
        activeEncounter.GetComponent<Encounter>().uMgr = this;
        activeEncounter.GetComponent<Encounter>().gMgr = mgr;
    }

    public void ConcludeEncounter(string desc) {
        if (activeEncounter) {
            BeginEncounter(encounterConclusion);
        }
        else {
            Debug.Log("UIManager.ConcludeEncounter() was run without any active encounter.");
        }

        activeEncounter.GetComponent<Encounter>().description = desc;
    }

    public void EndEncounter() {
        if (activeEncounter) {
            Destroy(activeEncounter);
        }
        else {
            Debug.Log("UIManager.EndEncounter() was run without any active encounter.");
        }

        mainMenu.SetActive(true);
        UpdateStats();

        if (player.currHealth <= 0) {
            mainMenu.SetActive(false);
            gameOverScreen.SetActive(true);
        }
    }

    public void RestartGame() {
        gameOverScreen.SetActive(false);
        classSelect.SetActive(true);
    }

    public void QuitGame() {
        Application.Quit();
    }

    public void RestPlayer() {
        if (player.gold >= 30) {
            player.currHealth = player.maxHealth;
            player.gold -= 30;
            UpdateStats();
        }
    }
}
