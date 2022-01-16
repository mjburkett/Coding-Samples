using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInteract : MonoBehaviour
{
    public GameObject[] levers;
    public GameObject[] souls;
    public GameObject[] characters;
    public GameObject feather;
    public GameObject branch;
    public GameObject ripper;
    public GameObject door;
    public GameObject door2;
    public GameObject wires;
    public Sprite brokenWires;
    public GameObject light1;
    public GameObject light2;
    public Sprite lightOff;
    public GameObject worm;
    public GameObject wormLoc;
    public float wormMoveSpeed = 0f;
    private bool wormWalking = false;
    public GameObject windowBoy;
    private PushableObject pushable;
    public float interactDistance = 0f;
    private bool holdFeather = false;

    // Start is called before the first frame update
    void Start()
    {
        pushable = (PushableObject) ripper.GetComponent(typeof(PushableObject));
    }

    // Update is called once per frame
    void Update()
    {
        // For each person, check...
        foreach (GameObject soul in souls)
        {
            Transform soulTrans = soul.transform;

            // if SOUL is in range and timmy presses e on his keyboard
            if (Vector3.Distance(transform.position, soulTrans.position) <= interactDistance && Input.GetKeyUp("e"))
            {
                SoulScript.soulValue += 1;
            }
        }

        Transform featherTrans = feather.transform;
        Transform branchTrans = branch.transform;

        // if FEATHER is in range and timmy presses e on his keyboard
        if (Vector3.Distance(transform.position, featherTrans.position) <= interactDistance && Input.GetKeyUp("e"))
        {
            holdFeather = true;
        }
        // while holding feather
        if (holdFeather)
        {
            // when moving, feather moves with player
            if (transform.localScale.x < 0 && featherTrans.localScale.x >=0 )
            {
                featherTrans.localScale = new Vector3(0 - featherTrans.localScale.x, featherTrans.localScale.y, featherTrans.localScale.z);
                featherTrans.SetPositionAndRotation(new Vector3(transform.position.x - 1f, transform.position.y), featherTrans.rotation);
            }
            else if(transform.localScale.x < 0)
            {
                featherTrans.SetPositionAndRotation(new Vector3(transform.position.x - 1f, transform.position.y), featherTrans.rotation);
            }
            else
            {
                featherTrans.localScale = new Vector3(Mathf.Abs(featherTrans.localScale.x), featherTrans.localScale.y, featherTrans.localScale.z);
                featherTrans.SetPositionAndRotation(new Vector3(transform.position.x + 1f, transform.position.y), featherTrans.rotation);
            }
            // when interacting with player with feather
            foreach (GameObject character in characters)
            {
                Transform charTrans = character.transform;
                if (Vector3.Distance(transform.position, charTrans.position) <= interactDistance && Input.GetKeyUp("e"))
                {
                    /*
                     * ADD ANIMATION HERE LATER
                     */
                    holdFeather = false;
                    feather.SetActive(false);
                    if(transform.localScale.x < 0 && charTrans.localScale.x >= 0)
                    {
                        charTrans.localScale = new Vector3(0 - charTrans.localScale.x, charTrans.localScale.y, charTrans.localScale.z);
                        //ADD RUNNING
                    }
                }
            }
        }
        // if BRANCH is in range and timmy presses e on his keyboard
        Vector3 actualBranchPos = new Vector3(branchTrans.position.x - 2.5f, branchTrans.position.y, branchTrans.position.z);
        if (Vector3.Distance(transform.position, actualBranchPos) <= interactDistance && Input.GetKeyUp("e"))
        {
            branch.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.None;
        }
        // if ROOF RIPPER is in range and timmy presses e on his keyboard
        if (Vector3.Distance(transform.position, ripper.transform.position) <= interactDistance && Input.GetKeyUp("e"))
        {
            pushable.Push(transform);
        }
        if (Vector3.Distance(transform.position, door.transform.position) <= interactDistance && Input.GetKeyUp("e"))
        {
            if(door.GetComponent<EdgeCollider2D>().enabled == true)
            {
                door.GetComponent<Animator>().Play("Door");
            }
            else
            {
                door.GetComponent<Animator>().Play("DoorClose");
            }
            door.GetComponent<EdgeCollider2D>().enabled = !door.GetComponent<EdgeCollider2D>().enabled;
        }

        if (Vector3.Distance(transform.position, door2.transform.position) <= interactDistance && Input.GetKeyUp("e"))
        {
            if (door2.GetComponent<EdgeCollider2D>().enabled == true)
            {
                door2.GetComponent<Animator>().Play("Door");
            }
            else
            {
                door2.GetComponent<Animator>().Play("DoorClose");
            }
            door2.GetComponent<EdgeCollider2D>().enabled = !door2.GetComponent<EdgeCollider2D>().enabled;
        }
        if (Vector3.Distance(transform.position, windowBoy.transform.position) <= interactDistance && Input.GetKeyUp("e") && holdFeather == true)
        {
            CharDeath characterFunctions = (CharDeath)windowBoy.GetComponent<CharDeath>();
            windowBoy.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.None;
            characterFunctions.perish(false);
            windowBoy.GetComponent<Rigidbody2D>().AddForce(new Vector3(500, 0, 0));
            feather.SetActive(false);
        }
        if (Vector3.Distance(transform.position, wires.transform.position) <= interactDistance && Input.GetKeyUp("e"))
        {
            wires.GetComponent<SpriteRenderer>().sprite = brokenWires;
            light1.GetComponent<SpriteRenderer>().sprite = lightOff;
            light2.GetComponent<SpriteRenderer>().sprite = lightOff;
            worm.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.None;
            worm.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.FreezeRotation;
            wormWalking = true;
        }

        foreach (GameObject lever in levers)
        {

            Transform leverTrans = lever.transform;

            // if LEVER is in range and timmy presses e on his keyboard
            if (Vector3.Distance(transform.position, leverTrans.position) <= interactDistance && Input.GetKeyUp("e"))
            {

            }
        }
        if(wormWalking == true && Mathf.Abs(worm.transform.position.x - wormLoc.transform.position.x + 2) >= 1)
        {
            //haha worm control
            CharacterController2D wormControl = (CharacterController2D)worm.GetComponent<CharacterController2D>();
            wormControl.Move(-1 * wormMoveSpeed, true, false);
        }
    }
}