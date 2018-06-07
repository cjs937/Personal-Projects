using System;
using UnityEngine;
using UnityEditor;
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
    //public Action<BaseNode> OnRemoveNode;

    public BaseNode(Vector2 position, float width, float height, string title, GUIStyle nodeStyle, GUIStyle inPointStyle, GUIStyle outPointStyle, Action<ConnectionPoint> OnClickInPoint, Action<ConnectionPoint> OnClickOutPoint)
    {
        windowRect = new Rect(position.x, position.y, width, height);
        style = nodeStyle;
        inPoint = new ConnectionPoint(this, ConnectionPointType.In, inPointStyle, OnClickInPoint);
        outPoint = new ConnectionPoint(this, ConnectionPointType.Out, outPointStyle, OnClickOutPoint);
        windowTitle = title;
    }

    public virtual void DrawWindow()
    {
        windowTitle = EditorGUILayout.TextField("Title", windowTitle);
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
