using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectGrabbable : MonoBehaviour
{
    // the id of the drink corresponds table it should be placed on
    public int drinkID;

    private GameObject[] allTables;

    // Start is called before the first frame update
    void Start()
    {
        // finds the total number of tables in the scene to ensure that, if there's more drinks than
        // tables, a drink is not assiged an ID higher than the possible table ID
        allTables = GameObject.FindGameObjectsWithTag("Table");
        int maxRange = allTables.Length;
        drinkID = Random.Range(0, maxRange - 1);
    }
}
