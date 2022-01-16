using UnityEngine;

public class CameraMovement : MonoBehaviour
{

    public Transform target;

    public float smoothSpeed = 0.125f;
    public Vector3 offset;

    void FixedUpdate()
    {
        Vector3 desiredPosition = target.position + offset;
        if(desiredPosition.x <= -1.7)
        {
            desiredPosition = new Vector3(-1.7f, desiredPosition.y, desiredPosition.z);
        }
        else if(desiredPosition.x >= 66)
        {
            desiredPosition = new Vector3(66f, desiredPosition.y, desiredPosition.z);
        }
        if (desiredPosition.y <= -2.7)
        {
            desiredPosition = new Vector3(desiredPosition.x, -1.5f, desiredPosition.z);
        }
        Vector3 smoothedPosition = Vector3.Lerp(transform.position, desiredPosition, smoothSpeed);
        transform.position = smoothedPosition;

        //transform.LookAt(target);
    }

}