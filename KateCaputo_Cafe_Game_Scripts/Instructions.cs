using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.Audio;


public class Instructions : MonoBehaviour
{
    // stores both the textual instructions, and the visual box they are contained in
    public Text instructions;
    public Image containerBox;
    public AudioSource buttonSound;

    void Start() {
        CanvasGroup instrGrp = containerBox.GetComponent<CanvasGroup>();
        instrGrp.alpha = 0;
        instrGrp.blocksRaycasts = true;
    }

    public void runInstructions()
    {
        // score and strikes to zero
        StartCoroutine(Coroutine());
    }

    // coroutine function allows the 'wait' functionality
    IEnumerator Coroutine() {
        
        GameObject[] toVanish = GameObject.FindGameObjectsWithTag("ToVanish");
        foreach(GameObject g in toVanish) {
            CanvasGroup canvasGroup = g.GetComponent<CanvasGroup>();
            canvasGroup.alpha = 0;
            canvasGroup.blocksRaycasts = true;
        }

        CanvasGroup instrGrp = containerBox.GetComponent<CanvasGroup>();

        instrGrp.alpha = 1;
        instrGrp.blocksRaycasts = false;
        buttonSound.Play();
        instructions.text = "use the arrows to navigate to the counter where drinks will appear!";
        yield return new WaitForSeconds(3);
        buttonSound.Play();
        instructions.text = "use the 'e' button to pick up a drink";
        yield return new WaitForSeconds(3);
        buttonSound.Play();
        instructions.text = "bring the drink to the right table and press 'e' to place it";
        yield return new WaitForSeconds(3);
        buttonSound.Play();
        instructions.text = "keep delivering drinks until you've served 10 drinks";
        yield return new WaitForSeconds(3);
        buttonSound.Play();
        instructions.fontSize = 180;
        instructions.text = "make sure there's never more than 3 drinks waiting at once or you'll get a strike!";
        yield return new WaitForSeconds(3);

        SceneManager.LoadScene("game1");
    }
}
