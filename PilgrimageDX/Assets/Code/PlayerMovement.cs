using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    public PlayerCam camera;
    public LayerMask raycastLayerMask;
    public CapsuleCollider collider;
    CapsuleCollider colliderMask;

    public float jumpForce;
    public float moveSpeed;
    public float midJumpGravity;
    public float endJumpGravity;

    public float raycastDistance;
    public bool bDebugRaycasts;
    Rigidbody rigidbody;

    float HorizontalAxis;
    float VerticalAxis;
    // Start is called before the first frame update
    void Start()
    {
        rigidbody = GetComponent<Rigidbody>();

        //colliderMask = Instantiate(collider);
        //colliderMask.name = "ColliderMask";
        //
        //colliderMask.transform.parent = transform;
    }

    // Update is called once per frame
    void Update()
    {
        HorizontalAxis = Input.GetAxis("L_Horizontal");
        VerticalAxis = Input.GetAxis("L_Vertical");

        Move(new Vector3(HorizontalAxis, 0, -VerticalAxis));

        Debug.Log(new Vector2(HorizontalAxis, VerticalAxis));
    }

    void Move(Vector3 direction)
    {
        Vector3 moveVec;

        moveVec = (camera.transform.forward * direction.z + camera.transform.right * direction.x) * moveSpeed * Time.deltaTime;
        moveVec.y = 0;

        if (!checkMovementMask(moveVec.normalized, raycastDistance))//!checkMovementRaycast(moveVec.normalized, raycastDistance))
        {
            transform.Translate(moveVec);
        }

        //rigidbody.AddForce(moveVec * moveSpeed);
    }

    bool checkMovementRaycast(Vector3 _direction, float _distance)
    {
        Vector3 leftDir = _direction;
        leftDir -= transform.right * 0.5f;
        Vector3 rightDir = _direction;
        rightDir += transform.right * 0.5f;

        Vector3 waistLvl = transform.position;
        waistLvl.y -= 0.8f;

        Ray ray = new Ray(transform.position, _direction);
        Ray rayLeft = new Ray(transform.position, leftDir);
        Ray rayRight = new Ray(transform.position, rightDir);

        Ray waistRay = new Ray(waistLvl, _direction);
        Ray waistLeft = new Ray(waistLvl, leftDir);
        Ray waistRight = new Ray(waistLvl, rightDir);

        RaycastHit hitData;

        if (Physics.Raycast(ray, out hitData, _distance, raycastLayerMask.value) ||
            Physics.Raycast(rayLeft, out hitData, _distance, raycastLayerMask.value) ||
            Physics.Raycast(rayRight, out hitData, _distance, raycastLayerMask.value))
        {
            if (bDebugRaycasts)
            {
                Debug.DrawLine(ray.origin, hitData.point, Color.red);
            }

            return true;
        }


        if (bDebugRaycasts)
        {
            Debug.DrawLine(ray.origin, ray.origin + ray.direction * _distance, Color.white);
            Debug.DrawLine(rayLeft.origin, rayLeft.origin + rayLeft.direction * _distance, Color.white);
            Debug.DrawLine(rayRight.origin, rayRight.origin + rayRight.direction * _distance, Color.white);
        }

        return false;
    }

    bool getAllowedMovement(Vector3 moveVec, ref Vector3 allowedMoveVec)
    {
        //check movement for each direction if one fails
        //call move allowed to check
        // if an allowed movement is found, the return the ref
        // if not, return false

        bool anyMoveAllowed = true;
        Vector3 direction = moveVec.normalized;
        //float checkDistance = 
        Vector3 bottom = collider.transform.TransformPoint(collider.center);
        bottom.y = bottom.y - collider.height / 2;

        Vector3 top = collider.transform.TransformPoint(collider.center);
        top.y = top.y + collider.height / 2;

        return Physics.CapsuleCast(bottom, top, collider.radius, direction, distance, raycastLayerMask.value);
    }

    //bool moveAllowed(Vector3 direction, float distance)
    //{
    //
    //}
}   //
