using System;
using System.Collections.Generic;
using UnityEngine;

public enum ConnectionPointType { In, Out }

public class ConnectionPoint
{
    public Rect rect;

    public ConnectionPointType type;

    public BaseNode node;

    public GUIStyle style;

    public Action<ConnectionPoint> OnClickConnectionPoint;

    public List<Connection> connections;

    public ConnectionPoint(BaseNode node, ConnectionPointType type, GUIStyle style, Action<ConnectionPoint> OnClickConnectionPoint)
    {
        this.node = node;
        this.type = type;
        this.style = style;
        this.OnClickConnectionPoint = OnClickConnectionPoint;
        rect = new Rect(0, 0, 10f, 20f);

        connections = new List<Connection>();
    }

    public void Draw()
    {
        rect.y = node.windowRect.y + (node.windowRect.height * 0.5f) - rect.height * 0.5f;

        switch (type)
        {
            case ConnectionPointType.In:
                rect.x = node.windowRect.x - rect.width + 8f;
                break;

            case ConnectionPointType.Out:
                rect.x = node.windowRect.x + node.windowRect.width - 8f;
                break;
        }

        if (GUI.Button(rect, "", style))
        {
            if (OnClickConnectionPoint != null)
            {
                OnClickConnectionPoint(this);
            }
        }

        //if this is output connection draw curves
        if(type == ConnectionPointType.Out)
        {
            foreach(Connection connection in connections)
            {
                connection.Draw();
            }
        }
    }

    public void MakeConnection(ConnectionPoint _connector, Connection _existingConnection = null)
    {
        Connection newConnect = _existingConnection;

        if (newConnect == null)
        {
            if (type == ConnectionPointType.In)
                newConnect = new Connection(this, _connector);
            else
                newConnect = new Connection(_connector, this);
        }

        connections.Add(newConnect);

        if (_existingConnection == null) //if the connection was created in this function add it to the other point in this connection
            _connector.MakeConnection(this, newConnect);
    }

    public void removeConnection(Connection _toRemove)
    {
        connections.Remove(_toRemove);
    }
}