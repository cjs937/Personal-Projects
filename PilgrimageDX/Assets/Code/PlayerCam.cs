using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//using Cinemachine;

[System.Serializable]
public class CameraPosition
{
    public PlayerCam.CameraState state;
    public Vector3 distance;
}

public class PlayerCam : MonoBehaviour
{

    public enum CameraState
    {
        IN_COMBAT,
        OUT_OF_COMBAT,
        LOCKED_ON
    }

    public GameObject rigPos;
    private GameObject lockOnRig;

    [HideInInspector]
    public Camera camera;

    //[Header("Camer Positions")]
    //public CameraPosition currentCamera;
    //public CameraPosition inCombat;
    //public CameraPosition outOfCombat;
    //public CameraPosition lockOnCam;

    [Header("Camera Properties")]

    public Vector3 camDistance;

    [Tooltip("How smooth the camera moves into place")]
    public float smoothing = 8.0f;
    public float rotationSmoothing = 8.0f;
    
    [Header("Panning camera up/down")]
    public float minPanUp = -30;
    public float maxPanUp = 40;   

    [Header("Player to follow")]
    public Transform target; //the target the camera follows

    [Header("Layer(s) to include")]
    public LayerMask CamOcclusion;  //the layers that will be affected by collision

    bool lockedOn = false;

    public float camRotateSpeed = 180f;
    public Vector3 camPosition;
    public Vector3 followMask;

    private float HorizontalAxis;
    private float VerticalAxis;

    GameObject cameraMask;

    public Vector2 defaultPosition;

    public float horizontalPan = 0;
    public float verticalPan = 0;

    float horizontalPanLockOn = 0;
    float verticalPanLockOn = 0;

    bool deIncrement = false;

    // Use this for initialization
    void Start()
    {
        camera = GetComponent<Camera>();

        cameraMask = new GameObject("CameraMask");
        cameraMask.transform.SetParent(rigPos.transform);
        cameraMask.transform.position = getFKPosition(rigPos.transform, 0);//camDistance.z);//currentCamera.distance;//rigPos.transform.position + currentCamera.distance;
        //currentCamera = outOfCombat;

        lockOnRig = new GameObject("LockOnRig");
        lockOnRig.transform.position = rigPos.transform.position;
        lockOnRig.transform.SetParent(rigPos.transform.parent);
        horizontalPan = defaultPosition.x;
        verticalPan = defaultPosition.y;
    }
   
    void LateUpdate()
    {
        //get input
        HorizontalAxis = Input.GetAxis("R_Horizontal");
        VerticalAxis = Input.GetAxis("R_Vertical");

        if (Mathf.Abs(HorizontalAxis) < 0.4f)
            HorizontalAxis = 0;

        if (Mathf.Abs(VerticalAxis) < 0.4f)
            VerticalAxis = 0;

        if (HorizontalAxis != 0 || VerticalAxis != 0)
            deIncrement = false;

       //if (PlayerInstance.GetInstance().combatScript.lockedOn)
       //    lockOn();
       //else
            freeLook();

        //if (deIncrement)
        //    deIncrementRotations();
    }

    void smoothCamMethod()
    {
        transform.position = Vector3.Lerp(transform.position, cameraMask.transform.position, Time.deltaTime * smoothing);
        transform.rotation = Quaternion.Lerp(transform.rotation, cameraMask.transform.rotation, Time.deltaTime * rotationSmoothing);
    }

    void occludeRay(Vector3 targetFollow, ref Vector3 mask)
    {
        #region prevent wall clipping
        //declare a new raycast hit.
        RaycastHit wallHit = new RaycastHit();

        //linecast from your player (targetFollow) to your cameras mask (camMask) to find collisions.
        if (Physics.Linecast(targetFollow, mask, out wallHit))//, CamOcclusion))
        {
            //the smooth is increased so you detect geometry collisions faster.
            //smoothing = 10f;
            //the x and z coordinates are pushed away from the wall by hit.normal.
            //the y coordinate stays the same.
            mask = new Vector3(wallHit.point.x + wallHit.normal.x * 0.5f, wallHit.point.y + wallHit.normal.y * 0.5f, wallHit.point.z + wallHit.normal.z * 0.5f);
            Debug.DrawLine(transform.position, wallHit.point, Color.red);
        }
        else
            Debug.DrawLine(targetFollow, transform.position, Color.blue);

        #endregion
    }

    void freeLook()
    {
        //get position that camera should be from target (player)
        //Vector3 targetOffset = rigPos.transform.position + currentCamera.distance;//target.position + currentCamera.distance;//distanceAway;

        //Quaternion rotation = Quaternion.Euler(verticalPan + currentCamera.distance.y, horizontalPan, 0);

        //Converts to vec3 position
        //Vector3 vectorMask = Vector3.one;
        //get orbit position around player
        //Quaternion orbitPos = Quaternion.Euler(horizontalPan, verticalPan, panZ); //(currentCamera.cameraHeight + cameraPanUp, rotateAround, cameraPan);

        //transform.position = targetOffset;

        //Vector3 orbitVector = Vector3.zero;//.targetOffset;// Vector3.zero;//orbitPos * vectorMask;
        //float radius = Vector3.Distance(targetOffset, target.transform.position);//Mathf.Abs(targetOffset.x - target.transform.position.x);

        //increment rotations based on player input
        horizontalPan += HorizontalAxis * camRotateSpeed * Time.deltaTime;

        verticalPan += (VerticalAxis * camRotateSpeed * Time.deltaTime);
        //verticalPan = Mathf.Clamp(verticalPan, minPanUp, maxPanUp);

        #region wrap the cam orbit rotation

        if (horizontalPan > 360)
        {
            horizontalPan -= 360;
        }
        else if (horizontalPan < 0f)
        {
            horizontalPan += 360;
        }

        if (verticalPan > 175)
        {
            verticalPan = 175;//-= 180;
        }
        else if (verticalPan < 0f)
        {
            verticalPan = 0;//+= 180;
        }
        #endregion

        Vector3 rotateVector = new Vector3(verticalPan, horizontalPan);//rotation * vectorMask;
        //orbitVector = rotateVector * radius;

        ////this determines where the camera will be

        rigPos.transform.rotation = Quaternion.Euler(rotateVector);//rotation = Quaternion.Euler(rotateVector);

        camPosition = getFKPosition(rigPos.transform, camDistance.z);//rigPos.transform.position + currentCamera.distance;//cameraMask.transform.position;
        Vector3 mask = camPosition;

        //perform raycast and keep cam from clipping
        occludeRay(target.transform.position, ref mask);

        cameraMask.transform.position = mask;

        smoothCamMethod();

        //point cam towards target
        cameraMask.transform.LookAt(target);
    }

    //void lockOn()
    //{
    //    if (PlayerInstance.GetInstance().combatScript.currentTarget == null)
    //        return;
    //
    //    horizontalPan += HorizontalAxis * camRotateSpeed * Time.deltaTime;
    //
    //    verticalPan += (VerticalAxis * camRotateSpeed * Time.deltaTime);
    //    //verticalPan = Mathf.Clamp(verticalPan, minPanUp, maxPanUp);
    //
    //    #region wrap the cam orbit rotation
    //
    //    if (horizontalPan > 360)
    //    {
    //        horizontalPan -= 360;
    //    }
    //    else if (horizontalPan < 0f)
    //    {
    //        horizontalPan += 360;
    //    }
    //
    //    if (verticalPan > 175)
    //    {
    //        verticalPan = 175;//-= 180;
    //    }
    //    else if (verticalPan < 0f)
    //    {
    //        verticalPan = 0;//+= 180;
    //    }
    //    #endregion
    //
    //    Vector3 rotateVector = new Vector3(verticalPan, horizontalPan);//rotation * vectorMask;
    //    //orbitVector = rotateVector * radius;
    //
    //    ////this determines where the camera will be
    //
    //    lockOnRig.transform.rotation = Quaternion.Euler(rotateVector);//rotation = Quaternion.Euler(rotateVector);
    //
    //    camPosition = getFKPosition(lockOnRig.transform, currentCamera.distance.z);//rigPos.transform.position + cu
    //
    //    //get point between target and enemy
    //    Vector3 midPoint = Utility.Midpoint(target.transform.position, PlayerInstance.GetInstance().combatScript.currentTarget.transform.position);
    //
    //    //Vector3 diff = PlayerInstance.GetInstance().combatScript.currentTarget.transform.position - target.transform.position;
    //
    //    Vector3 mask = camPosition;// (lockOnRig.transform, currentCamera.distance.z);//(Vector3.up * currentCamera.distance.y) + target.position - (diff.normalized * currentCamera.distance.z);
    //    
    //    //prevent clipping
    //    occludeRay(target.position, ref mask);
    //
    //    cameraMask.transform.position = mask;
    //
    //    //point towards midpoint
    //    cameraMask.transform.LookAt(midPoint);
    //
    //    smoothCamMethod();
    //}

    //public void beginLockOn()
    //{
    //    PlayerCombat player = PlayerInstance.GetInstance().combatScript;
    //
    //    if (player.currentTarget == null)
    //        return;
    //
    //    currentCamera = lockOnCam;
    //    cameraMask.transform.SetParent(lockOnRig.transform);
    //}
    //
    //public void endLockOn()
    //{
    //    currentCamera = inCombat;
    //
    //    rigPos.transform.rotation = lockOnRig.transform.rotation;
    //    cameraMask.transform.SetParent(rigPos.transform);
    //}

    Vector3 getFKPosition(Transform rig, float distance)
    {
        return rig.position - (rig.up * distance); //(rigPos.transform.forward * currentCamera.distance.z) + (rigPos.transform.up * currentCamera.distance.y);
    }
}
