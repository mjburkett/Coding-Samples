using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Kill : MonoBehaviour
{
    public bool isHeavy = false;
    public bool contactHurts = false;
    public bool movesCharacter = false;
    public bool oneTimeUse = false;
    private bool canFunction = true;
    private CharDeath characterFunctions;

    private void OnCollisionEnter2D(Collision2D col)
    {
        if (canFunction)
        {
            if (col.gameObject.tag == "Characters")
            {
                col.gameObject.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.None;
            }
            characterFunctions = (CharDeath)col.gameObject.GetComponent(typeof(CharDeath));
            if (col.gameObject.tag == "Characters" && movesCharacter == true)
            {
                col.gameObject.GetComponent<Rigidbody2D>().AddForce((col.gameObject.transform.position - transform.position).normalized * 500);
                if (oneTimeUse)
                {
                    canFunc(false);
                }
            }
            if (col.gameObject.tag == "Characters" && isHeavy == true && (GetComponent<Rigidbody2D>().velocity.y != 0 || GetComponent<Rigidbody2D>().velocity.x != 0))
            {
                characterFunctions.perish(false);
                if (oneTimeUse)
                {
                    canFunc(false);
                }
            }
            if (col.gameObject.tag == "Characters" && contactHurts == true)
            {
                characterFunctions.perish(true);
                if (oneTimeUse)
                {
                    canFunc(false);
                }
            }
        }
    }

    public void canFunc(bool f)
    {
        canFunction = f;
    }
}
