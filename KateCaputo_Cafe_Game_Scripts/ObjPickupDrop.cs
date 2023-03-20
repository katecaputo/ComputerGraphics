using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class ObjPickupDrop : MonoBehaviour
{
    [SerializeField] private Transform playerCamTrans;
    [SerializeField] private LayerMask pickUpLayerMask;
    public AudioSource pickUpSound;

    private void Update()
    {
        pickUpSound.Play();
        if(Input.GetKeyDown(KeyCode.E))
        {
            float pickupDistance = 2f;
            if (Physics.Raycast(playerCamTrans.position, playerCamTrans.forward, out RaycastHit raycastHit, pickupDistance))
            {
                if(raycastHit.transform.TryGetComponent(out ObjectGrabbable objGrabbable))
                {
                    Debug.Log(objGrabbable);
                }
            }
        }
    }
}
