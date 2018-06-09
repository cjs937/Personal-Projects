using System;
using UnityEditor;
using UnityEngine;

[System.Serializable]
public class Connection
{
    public ConnectionPoint inPoint;
    public ConnectionPoint outPoint;
    public Action<Connection> onClickRemoveConnection;

    public int connectionId;

    public Connection(ConnectionPoint inPoint, ConnectionPoint outPoint, Action<Connection> _onClickRemoveConnection)
    {
        this.inPoint = inPoint;
        this.outPoint = outPoint;

        onClickRemoveConnection = _onClickRemoveConnection;
    }

    public void Draw(Color _color)
    {
        Handles.DrawBezier(
            inPoint.rect.center,
            outPoint.rect.center,
            inPoint.rect.center + Vector2.left * 50f,
            outPoint.rect.center - Vector2.left * 50f,
            _color,
            null,
            2f
        );

        if (Handles.Button((inPoint.rect.center + outPoint.rect.center) * 0.5f, Quaternion.identity, 4, 8, Handles.RectangleHandleCap))
        {
            inPoint.removeConnection(connectionId);
            outPoint.removeConnection(connectionId);

            onClickRemoveConnection(this);

            //remove();
        }
    }

    void remove()
    {
        //inPoint.removeConnection(this);
        //outPoint.removeConnection(this);

    }
}