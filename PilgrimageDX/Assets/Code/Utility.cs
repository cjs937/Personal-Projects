using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Utility : MonoBehaviour
{
    public static Vector2 getMousePosition()
    {
        return Camera.main.ScreenToWorldPoint(Input.mousePosition);
    }

    public static Vector2 getScreenMin(Camera _camera = null)
    {
        Camera screen;

        if (_camera == null)
            screen = Camera.main;
        else
            screen = _camera;

        return screen.ScreenToWorldPoint(Vector2.zero);
    }

    public static Vector2 getScreenMax(Camera _camera = null)
    {
        Camera screen;

        if (_camera == null)
            screen = Camera.main;
        else
            screen = _camera;

        return screen.ScreenToWorldPoint(new Vector2(screen.pixelWidth, screen.pixelHeight));
    }

    public static Vector2 convertToCameraSpace(Camera _camera, Vector2 _position)
    {
        float x = _camera.transform.position.x + _position.x; //+ (_camera.transform.position.x - _position.x);

        float y = _camera.transform.position.y + _position.y; //+ (_camera.transform.position.y - _position.y);

        return new Vector2(x, y);
    }

    public static bool isOffScreen(Vector2 _position)
    {
        Vector2 screenMin = getScreenMin();
        Vector2 screenMax = getScreenMax();

        if (_position.x < screenMin.x || screenMax.x < _position.x)
        {
            return true;
        }

        if (_position.y < screenMin.y || screenMax.y < _position.y)
        {
            return true;
        }

        return false;
    }

    public static Vector2 mutliplyVectors(Vector2 _vectorA, Vector2 _vectorB)
    {
        return new Vector2(_vectorA.x * _vectorB.x, _vectorA.y * _vectorB.y);
    }

    public static Vector3 Midpoint(Vector3 _A, Vector3 _B)
    {
        float x = (_A.x + _B.x) / 2;
        float y = (_A.y + _B.y) / 2;
        float z = (_A.z + _B.z) / 2;

        return new Vector3(x, y, z);
    }

    public static bool HasReachedTarget(NavMeshAgent _agent)
    {
        if (_agent.isOnNavMesh && !_agent.pathPending)
        {
            if (_agent.remainingDistance <= _agent.stoppingDistance)
            {
                if (!_agent.hasPath || _agent.velocity.sqrMagnitude == 0f)
                {
                    return true;
                }
            }
        }

        return false;
    }

    public static Vector3 GetRandomPointByRadius(Vector3 _center, float _radius)
    {
        return GetRandomPointByRadius(_center, _radius, Vector3.one);
    }

    public static Vector3 GetRandomPointByRadius(Vector3 _center, float _radius, Vector3 _directions)
    {
        Vector3 randPoint = Vector3.zero;

        randPoint.x = Random.Range(-_radius, _radius) * _directions.x;
        randPoint.y = Random.Range(-_radius, _radius) * _directions.y;
        randPoint.z = Random.Range(-_radius, _radius) * _directions.z;

        return _center + randPoint;
    }

    public static bool IsLayerInMask(LayerMask _mask, int _layer)
    {
        return _mask == (_mask | 1 << _layer);
    }

    public static bool IsVisible(Renderer renderer, Camera _cam)
    {
        Plane[] planes = GeometryUtility.CalculateFrustumPlanes(_cam);
        return (GeometryUtility.TestPlanesAABB(planes, renderer.bounds)) ? true : false;
    }
}

public struct Tuple3<A, B, C>
{

    public A first
    {
        get;
        private set;
    }

    public B second
    {
        get;
        private set;
    }

    public C third
    {
        get;
        private set;
    }

    public Tuple3(A first, B second, C third)
    {
        this.first = first;
        this.second = second;
        this.third = third;
    }

}