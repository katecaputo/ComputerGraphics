using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class quitGame : MonoBehaviour
{
    // quits the application and exits from the game
    public void exitGame()
    {
        Application.Quit();
    }

    public void toCredits() {
        try { SceneManager.LoadScene("credits"); }
        catch (Exception e) { print("Error: loading credits scene"); Debug.LogException(e, this); }
    }
}
