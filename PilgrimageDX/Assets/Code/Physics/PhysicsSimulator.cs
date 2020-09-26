using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PhysicsSimulator : MonoBehaviour
{
    public float gravity;
    public float mass;

    public Vector3 linearAcceleration;
    public Vector3 linearVelocity;
    public Vector3 linearForce;

    public Vector3 angluarAcceleration;
    public Vector3 angularVelocity;

    public List<Collider> 

    Rigidbody rigidbody;

    private void Start()
    {
        rigidbody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        //AddForceDirect(new Vector3(0, GetForceOfGravity(), 0));

        if (Input.GetKey(KeyCode.A))
            AddForceDirect(new Vector3(0, 0, -5.0f));
    }

    private void FixedUpdate()
    {
        ConvertForce();

        IntegrateEulerKinematic(Time.deltaTime);

        linearForce = Vector3.zero;
    }
    void IntegrateEulerKinematic(float dt)
    {
        Vector3 tempAccel;
        Vector3 tempVelocity;

        //Position(X)	
        //Acceleration(A) * 1/2 * dt^2
        float accelScalar = 0.5f * dt * dt;
        tempAccel = linearAcceleration * accelScalar;

        //Velocity(V) * dt
        tempVelocity = linearVelocity * dt;

        //V + A
        tempVelocity += tempAccel;

        //X + V
        //transform.position += (Vector3)tempVelocity;

        //Set V
        rigidbody.velocity = (linearVelocity += linearAcceleration * dt);

        Debug.Log(rigidbody.velocity);
    }

    public void AddForceDirect(Vector3 force)
    {
        linearForce += force;
    }

    public void OverrideLinearForce(Vector3 force)
    {
        linearForce = force;
    }

    public float GetForceOfGravity()
    {
        return gravity * mass;
    }

    void ConvertForce()
    {
        if (mass != 0)
            linearAcceleration = linearForce / mass;
        else
            linearAcceleration = Vector3.zero;
    }
}
