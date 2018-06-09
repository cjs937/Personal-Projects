using System;
using UnityEngine;
using UnityEditor;

public enum NodeType
{
    DEFAULT = -1,
    INPUT,
    COMBO
}

public class BaseNode : ScriptableObject
{
    //public BaseNode prevNode;
    //public BaseNode nextNode;
    public Rect windowRect;
    public GUIStyle style;
    public bool hasInputs = false;

    public ConnectionPoint inPoint;
    public ConnectionPoint outPoint;

    public string windowTitle = "";

    public NodeType type;
    //public Action<BaseNode> OnRemoveNode;

    public BaseNode(Vector2 position, float width, float height, string title, GUIStyle nodeStyle, GUIStyle inPointStyle, GUIStyle outPointStyle, Action<ConnectionPoint> OnClickInPoint, 
        Action<ConnectionPoint> OnClickOutPoint, NodeType type = NodeType.DEFAULT)
    {
        windowRect = new Rect(position.x, position.y, width, height);
        style = nodeStyle;
        windowTitle = title;

        initConnectionPoints(inPointStyle, outPointStyle, OnClickInPoint, OnClickOutPoint);
    }

    public virtual void DrawWindow()
    {
        windowTitle = EditorGUILayout.TextField("Title", windowTitle);
    }

    protected virtual void initConnectionPoints(GUIStyle _inPointStype, GUIStyle _outPointStyle, Action<ConnectionPoint> _onClickIn, Action<ConnectionPoint> _onClickOut)
    {
        inPoint = new ConnectionPoint(this, ConnectionPointType.In, _inPointStype, _onClickIn, Color.white);
        outPoint = new ConnectionPoint(this, ConnectionPointType.Out, _outPointStyle, _onClickOut, Color.white);
    }

    public virtual void NodeDeleted(BaseNode _node)
    { }

    public virtual void OnClick(Vector2 _position)
    {}

    public void Drag(Vector2 delta)
    {
        windowRect.position += delta;
    }
}
