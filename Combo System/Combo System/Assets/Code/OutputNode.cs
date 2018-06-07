using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class OutputNode : BaseNode
{
    string result = "";

    BaseInputNode inputNode;
    Rect inputNodeRect;

    public OutputNode()
    {
        windowTitle = "Output Node";

        hasInputs = true;
    }

    public override void DrawWindow()
    {
        base.DrawWindow();

        Event evt = Event.current;

        string input1Title = "None";

        if(inputNode)
        {
            //input1Title = inputNode.getResult();
        }

        GUILayout.Label("input 1: " + input1Title);

        if(evt.type == EventType.Repaint)
        {
            inputNodeRect = GUILayoutUtility.GetLastRect();
        }

        GUILayout.Label("Result: " + result);
    }

    public override void DrawCurves()
    {
        if(inputNode)
        {
            Rect rect = windowRect;

            rect.x += inputNodeRect.x;
            rect.y += inputNodeRect.y + inputNodeRect.height / 2;

            rect.width = 1.0f;
            rect.height = 1;

            ComboEditor.DrawNodeCurve(inputNode.windowRect, rect, Color.black);
        }
    }

    public override void NodeDeleted(BaseNode _node)
    {
        if(_node.Equals(inputNode))
        {
            inputNode = null;
        }
    }

    //public override void SetInput(BaseInputNode _input, Vector2 _clickPos)
    //{
    //    _clickPos.x -= windowRect.x;
    //    _clickPos.y -= windowRect.y;

    //    if(inputNodeRect.Contains(_clickPos))
    //    {
    //        inputNode = _input;
    //    }
    //}
}
