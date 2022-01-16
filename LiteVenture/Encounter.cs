using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/**
 * This class is designed to act as the base class for an encounter system.
 * An encounter should have a description/prompt and selectable options.
 **/
public class Encounter : MonoBehaviour {
    public UIManager uMgr;
    public GameManager gMgr;
    public string description;
    public List<string> playerOptions;
    public Button choiceButtonPrefab;
    private GameObject viewportContent;

    private int optionsAdded = 0;

    // Start is called before the first frame update
    void Start() {
        Button option;
        //A messy way of setting the text.
        GetComponentInChildren<ContentSizeFitter>().gameObject.GetComponentInChildren<Text>().text = description;
        for (int i = 0; i < playerOptions.Count; i++) {
            AddOption(playerOptions[i]);
        }
    }

    protected void AttachListener(Button btn, int i) {
        btn.onClick.AddListener(delegate { SelectOption(i); });
    }

    public void AddOption(string opt) {
        Button btn = Instantiate(choiceButtonPrefab);
        int insertIndex = optionsAdded;
        optionsAdded += 1;
        //The button is being instantiated under the viewport's content for appropriate scaling within the window.
        //option.transform.parent = GetComponentInChildren<ContentSizeFitter>().transform;
        btn.transform.SetParent(GetComponentInChildren<ContentSizeFitter>().transform);
        //When the button is pressed, the encounter script will be notified with the index corresponding to the choice made.
        //option.onClick.AddListener(delegate { SelectOption(i); });
        AttachListener(btn, insertIndex);
        //Lastly, the button's text is set to the given player option string.
        btn.GetComponentInChildren<Text>().text = opt;
    }

    public virtual void SelectOption(int choice) { }
    public void EndEncounter() {
        uMgr.EndEncounter();
    }
}
