using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawn : MonoBehaviour
{
    // all the drinks in the scene
    public GameObject[] allDrinks;

    // keeps track of how many drinks are currently on the table and what number we area in the queue
    private int numOnTableNow;
    private int curDrink = 0;

    // keeps track of timing for spawning
    private float timeForSpawn = 0.0f;
    // time between spawns MUST BE CHANGED IN UNITY INSPECTOR
    public float period = 8.0f;
    // are there still more hidden drinks to appear?
    private bool moreDrinks = true;
    // player field to be able to access and alter strikes
    private GameObject player;


    void Start()
    {
        // populate list of drinks with all those tagged in scene
        if (allDrinks == null) { allDrinks = GameObject.FindGameObjectsWithTag("Pickup"); }

        // at beginning of game, diable all drinks
        foreach(GameObject d in allDrinks)
        {
            d.SetActive(false);
            d.GetComponent<Rigidbody>().isKinematic = true;
        }

        // find the player in the scene SHOULD ONLY BE ONE
        try{
            GameObject[] players = GameObject.FindGameObjectsWithTag("Player");
            if (players.Length > 1) {
                print("WARNING: more than one player in scene");
            }
            player = players[0];
        }
        catch (Exception e) { print("Error: could not find player, check tag"); Debug.LogException(e, this); } 
    
    }

   
    void Update()
    {
        // if time period for spawning has passed and there are more drinks to be placed
        if (Time.time > timeForSpawn && moreDrinks)
        {
            timeForSpawn += period;
            if (curDrink < allDrinks.Length)
            {
                // activate the next drink
                try { allDrinks[curDrink].SetActive(true); }
                catch (Exception e) { print("Error: check drink index"); Debug.LogException(e, this); }
                
                curDrink++;
                numOnTableNow++;
            }
            else
            {
                // no more drinks to activate flag, stops loop
                moreDrinks = false;
            }

            // if there are more than 5 drinks on the table at any time, give a strike every [period] seconds
            if (numOnTableNow > 3)
            {
                player.GetComponent<pickupController>().decreaseStrikes();
            }
        }

        // this block ensures that strikes are still given every [period] seconds, even after no more drinks are left to activate
        if (Time.time > timeForSpawn && !moreDrinks)
        {
            timeForSpawn += period;
            if (numOnTableNow > 3)
            {
                player.GetComponent<pickupController>().decreaseStrikes();
            }
        }
    }

    public void decNumOnTableNow() {
        numOnTableNow--;
    }
}
