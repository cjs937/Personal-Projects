using System.Collections;
using UnityEngine;
using UnityEditor;

public class BaseNode : ScriptableObject
{
    public BaseNode prevNode;
    public BaseNode nextNode;
    public Rect windowRect;
    public GUIStyle style;
    public bool hasInputs = false;

    public string windowTitle = "";
    
    public virtual void DrawWindow()
    {
        windowTitle = EditorGUILayout.TextField("Title", windowTitle);
    }

    public virtual void DrawCurves()
    {
        if (nextNode)
        {
            ComboEditor.DrawNodeCurve(windowRect, nextNode.windowRect, Color.black);
        }
    }

    public virtual void MakeConnection(BaseNode _connector, bool _isInput)
    {
        if (_isInput) //connection is incoming
        {
            prevNode = _connector;
            _connector.nextNode = this;
        }
        else //connection is outgoing
        {
            nextNode = _connector;
            _connector.prevNode = this;
        }
    }

    public virtual void NodeDeleted(BaseNode _node)
    { }

    public virtual void OnClick(Vector2 _position)
    {}
}
