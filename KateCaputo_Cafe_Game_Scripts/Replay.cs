using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Replay : MonoBehaviour
{
   
   public void Retry()
    {
        pickupController.score = 0;
        pickupController.strikesleft = 10;
        try { SceneManager.LoadScene("mainMenu"); }
        catch (Exception e) { print("Error: loading main menu scene"); Debug.LogException(e, this); }
    }
}
