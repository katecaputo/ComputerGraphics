using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SnapshotControls : MonoBehaviour
{
    public AudioMixerSnapshot cafe_up;
    public AudioMixerSnapshot waves_up;
    public Transform playerTrans;

    public void Update()
    {
        float playerZ = playerTrans.position.z;
            if (playerZ < 5)
            {
                cafe_up.TransitionTo(.5f);
            }
            else
            {
                waves_up.TransitionTo(.5f);
            }
    }
}
