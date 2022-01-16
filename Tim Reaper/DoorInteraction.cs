using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class DoorInteraction : MonoBehaviour
{
    public GameObject player;
    public float linearDist;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyUp("e") && Mathf.Abs(player.transform.position.x - transform.position.x) <= linearDist && Mathf.Abs(player.transform.position.y - transform.position.y) <= linearDist)
        {
            //This can be replaced with code to change the sprite of the cabin to the interior sprite.
            SceneManager.LoadScene(sceneName: "InCabinTest");
        }
    }
}
