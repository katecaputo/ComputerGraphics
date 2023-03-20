using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.Audio;

public class pickupController : MonoBehaviour
{
    [Header("Pickup Settings")] 
    //where the player will spawn if they fall off the world
    public Transform spawnPoint;
    // position in front of the player where the object will be held
    [SerializeField] Transform holdArea;
    // the object that we will create
    private GameObject heldObj;
    // same object but accessed through body component so we can reuse it
    private Rigidbody heldObjRB;
    // used for the physics of picking up the object
    private float pickupForce = 150.0f;


    [Header("Score and Audio")]
    // fields to keep track of gameplay, score, health, and connected audio clips
    [SerializeField] public static int score = 0;
    [SerializeField] public static int strikesleft = 5;
    [SerializeField] public int winScore = 10;
    [SerializeField] public AudioSource aud;
    [SerializeField] public AudioClip placeItem;
    [SerializeField] public AudioClip pickupItem;
    [SerializeField] public AudioClip loseStrike;

    [Header("Text")]
    // the text appearing on screen to indicate which table the drink should be brought to
    [SerializeField] public Text currentTableTarget;
    // text on the screen to prive the user with their score and health
    [SerializeField] public Text scoreText;
    [SerializeField] public Text strikeText;

    // an array of all the tables in the cafe
    private GameObject[] allTables;

    // keeps track of the index of the next drink to be delivered
    private int nextDrink = 0;


    void Start()
    {
        makeUpdates();
        populateTables();
        Cursor.visible = true;
    }

    void Update()
    {
        // if the escape key is pressed, quit the game
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit();
        }

        // the "E" key is used to pick up and place drinks
        if (Input.GetKeyDown(KeyCode.E))
        {
            // cast a ray out from the player in the direction they are facing
            RaycastHit hit;
            Vector3 posnToCastFrom = transform.position + new Vector3(0.0f, 1.0f, 0.0f);
            if (Physics.Raycast(posnToCastFrom, transform.TransformDirection(Vector3.forward), out hit))
            {
                // prints to console what has been hit; debugging purposes
                print("ray hit: " + hit.transform.gameObject);
            }
            // 'else' case is necessary so that an Error is not thrown when nothing (sky) is hit
            else {}

            // Check: are we holding an object?
            if (heldObj == null)
            {
                PickupObject(hit.transform.gameObject);
            }
            // if we are holding an object, in order to put it down, we must hit the correct table
            else {
                // use tag to check if hit object is even a table
                if (hit.transform.gameObject.tag == "Table")
                {
                    PutOnTable(hit.transform.gameObject, heldObj);
                }
                else
                {
                    print("can't drop, not in front of table");
                }
            }
        }

        // when E is not pressed and the player is holding an object, it must move with the player
        if (heldObj != null) {
            MoveObject();
        }

        // respawns the player if they fall off the world and gives a strike
        if (gameObject.transform.position.y < -5) {
            gameObject.transform.position = spawnPoint.transform.position;
            strikesleft--;
        }

        // otherwise, continually check for score and health updates, update screen accordingly
        makeUpdates();
    }


    // a function to pick up a drink
    void PickupObject(GameObject pickObj)
    {
        // ray cast must hit the encompassing, tagged area
        if (pickObj.tag == "PickUpArea")
        {
            // get rigidbody component of the next drink in the queue
            try { heldObjRB = pickObj.GetComponent<Spawn>().allDrinks[nextDrink].GetComponent<Rigidbody>(); }
            catch (Exception e) { print ("Error: check that all drinks are connected"); Debug.LogException(e, this); }
            
            heldObjRB.useGravity = false;
            heldObjRB.drag = 10; // showing drag; optional
            heldObjRB.constraints = RigidbodyConstraints.FreezeRotation; // not going to rotate; optional

            // parent the drink rigidbody to the players holding area, and match their transforms
            heldObjRB.transform.parent = holdArea;
            heldObj = pickObj.GetComponent<Spawn>().allDrinks[nextDrink];
            heldObj.transform.position = holdArea.transform.position;

            // the ID of the drink will correspond to the table it should be placed at, therefore we need to access it
            int curID = heldObj.GetComponent<ObjectGrabbable>().drinkID;

            // show text on screen of which table the drink should go to
            currentTableTarget.text = "bring to table " + curID;
            
            // use the below function if the IndicateTable method is enabled in the future
            //allTables[curID].GetComponent<tableScript>().IndicateTable();

            pickObj.GetComponent<Spawn>().decNumOnTableNow();

            aud.clip = pickupItem;
            aud.Play();
        }
    }


    // a function to move the held drink wiht the player
    void MoveObject()
    {
        if (Vector3.Distance(heldObj.transform.position, holdArea.position) > 0.1f)
        {
            // math to work out how far the object needs to move
            Vector3 moveDirection = (holdArea.position - heldObj.transform.position);
            heldObjRB.AddForce(moveDirection * pickupForce);
        }
    }


    // a function to put the drink on the corresponding table
    void PutOnTable(GameObject table, GameObject drink)
    {
        try {
            // retrieve the IDs of the drinks and tables, as they are necessary to know if the drink is going to the right place
            int tableID = table.GetComponent<tableScript>().tableID;
            int drinkID = drink.GetComponent<ObjectGrabbable>().drinkID;
        
            // if the player tries to put the drink on the CORRECT table
            if (tableID == drinkID)
            {
                Vector3 tablePosn = table.transform.position;
                // unparent the drink from the players holding area
                heldObjRB.transform.parent = null;
                // the player is no longer holding an object
                heldObj = null;
                // place the drink above the table using position transforms only
                drink.GetComponent<Rigidbody>().isKinematic = true;
                drink.GetComponent<Rigidbody>().detectCollisions = false;
                drink.GetComponent<BoxCollider>().enabled = false;
                drink.transform.position = new Vector3(tablePosn.x, tablePosn.y + 1.0f, tablePosn.z);

                // increase the score, move onto the next drink, then destroy the compledted one
                score++;
                nextDrink++;
                currentTableTarget.text = "";
                
                aud.clip = placeItem;
                aud.Play();

                StartCoroutine(hideDrink(drink));

            }
            // if the player tries to put the drink on the INCORRECT table
            else
            {           
                StartCoroutine(Coroutine(2, drinkID));
            }
        }
        catch (Exception e)
        {
            print("Error: placing on table, check table and drink tags and array");
            Debug.LogException(e, this);
        }
    }

    // continusly runs to check the win and lose conditions, and update the visual feedback for score and health on screen
    void makeUpdates()
    {
        if (score == winScore) {
            try { SceneManager.LoadScene("winScene"); }
            catch (Exception e) { print("Error: loading win scene"); Debug.LogException(e, this); }
        }

        if (strikesleft == 0) {
            try { SceneManager.LoadScene("loseScene"); }
            catch (Exception e) { print("Error: loading lose screen"); Debug.LogException(e, this); }
        }

        scoreText.text = "score: " + score;
        strikeText.text = "strikes left: " + strikesleft;
    }

    // finds all the tables in the scene and adds them to our internal list
    void populateTables()
    {
        if (allTables == null) { allTables = GameObject.FindGameObjectsWithTag("Table"); }
    }

    // provides textual feedback that the wrong table has been chosen; incorporates a 'wait'
    IEnumerator Coroutine(int sec, int drinkID) {
        currentTableTarget.text = "wrong table!!";
        yield return new WaitForSeconds(sec);
        currentTableTarget.text = "bring to table " + drinkID;
    }

    // a function used to decrease the amount of strikes a user has left
    public void decreaseStrikes() {
        strikesleft = strikesleft - 1;
        
        aud.clip = loseStrike;
        aud.Play();
    }

    // hides the drink after its been placed on a table
    IEnumerator hideDrink(GameObject d) {
        yield return new WaitForSeconds(5);
        d.SetActive(false);
    }
}