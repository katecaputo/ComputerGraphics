using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine;
using UnityEngine.UI;

public class tableScript : MonoBehaviour
{
    public int tableID;
    public Text floatingText;
    public Transform playerLoc;

    public void Start()
    {
        // places the table number text above each table
        floatingText.text = "" + tableID; 
    }


    // in the future, populate this method with code to make the table light up,
    // make a noise, or otherwise indicate that it is the one where the drink should go
    public void IndicateTable() { }


    // rotates the number above each table to face the player as they move around the scene for easy reading
    public void Update() {
        Vector3 temp = new Vector3(playerLoc.position.x, 1.4f, playerLoc.position.z);
         floatingText.transform.LookAt(temp);
         floatingText.transform.Rotate(0.0f, 180.0f, 0.0f, Space.Self);
    }
}
