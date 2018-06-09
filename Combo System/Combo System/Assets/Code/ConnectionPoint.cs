using System;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public enum ConnectionPointType { In, Out }

[System.Serializable]
public class ConnectionPoint
{
    public Rect rect;

    public ConnectionPointType type;

    public BaseNode node;

    public GUIStyle style;

    public Action<ConnectionPoint> OnClickConnectionPoint;

    public List<int> connectionIDs;

    [SerializeField]
    Color lineColor;

    public ConnectionPoint(BaseNode node, ConnectionPointType type, GUIStyle style, Action<ConnectionPoint> OnClickConnectionPoint, Color _lineColor)
    {
        this.node = node;
        this.type = type;
        this.style = style;
        this.OnClickConnectionPoint = OnClickConnectionPoint;
        rect = new Rect(0, 0, 10f, 20f);

        lineColor = _lineColor;

        connectionIDs = new List<int>();
    }

    public void Draw()
    {
        rect.y = node.windowRect.y + (node.windowRect.height * 0.5f) - rect.height * 0.5f;

        switch (type)
        {
            case ConnectionPointType.In:
                rect.x = node.windowRect.x - rect.width;
                break;

            case ConnectionPointType.Out:
                rect.x = node.windowRect.x + node.windowRect.width;
                break;
        }

        if (GUI.Button(rect, "", style))
        {
            if (OnClickConnectionPoint != null)
            {
                OnClickConnectionPoint(this);
            }
        }

        ////if this is output connection draw curves
        //if(type == ConnectionPointType.Out)
        //{
        //    foreach(Connection connection in connections)
        //    {
        //        connection.Draw(lineColor);
        //    }
        //}
    }

    public Connection MakeConnection(ConnectionPoint _connector, int _newId, Action<Connection> _onClickRemoveConnection)
    {
        if (_connector.type == type || _connector.node == node)
            return null;

        Connection newConnect;

        if (type == ConnectionPointType.In)
            newConnect = new Connection(this, _connector, _onClickRemoveConnection);
        else
            newConnect = new Connection(_connector, this, _onClickRemoveConnection);
        

        newConnect.connectionId = _newId;

        connectionIDs.Add(newConnect.connectionId);

        //if (_existingConnection == null) //if the connection was created in this function add it to the other point in this connection
        //    _connector.MakeConnection(this, newConnect);

        return newConnect;
    }

    public void removeConnection(int _toRemoveID)
    {
        connectionIDs.Remove(_toRemoveID);
    }
}