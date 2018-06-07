using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public enum ComboMove
{
    PUNCH_1,
    PUNCH_2,
    KICK_1,
    KICK_2,
    STAR_PLATINUM_PUNCH
}


public class ComboNode : BaseNode
{ 
    public ComboMove comboMove;

    public ComboNode(Vector2 position, float width, float height, string title, GUIStyle nodeStyle, GUIStyle inPointStyle, GUIStyle outPointStyle, Action<ConnectionPoint> OnClickInPoint, Action<ConnectionPoint> OnClickOutPoint)
        : base(position, width, height, title, nodeStyle, inPointStyle, outPointStyle, OnClickInPoint, OnClickOutPoint)
    {}

    public override void DrawWindow()
    {
        base.DrawWindow();

        GUI.Box(windowRect, windowTitle);//, style);

        comboMove = (ComboMove)EditorGUILayout.EnumPopup("Input type : ", comboMove);
    }

    //public override void MakeConnection(BaseNode _connector, bool _isInput)
    //{
    //    //prevents combo nodes from connecting to each other
    //    if(_connector as ComboNode)
    //    {
    //        return;
    //    }
    //    else
    //    {
    //        Debug.Log("fug");
    //        base.MakeConnection(_connector, _isInput);
    //    }
    //}


    //public override void DrawCurves()
    //{
    //    if (nextNode)
    //    {
    //        ComboEditor.DrawNodeCurve(windowRect, nextNode.windowRect, Color.red);
    //    }
    //}


    //public string getComboString()
    //{
    //    string comboString = "";

    //    BaseNode lastNode = prevNode;

    //    while (lastNode)
    //    {
    //        InputNode inputNode = (InputNode)lastNode;

    //        if (inputNode)
    //        {
    //            comboString.Insert(0, inputNode.getInputString() + ",");
    //        }

    //        lastNode = lastNode.prevNode;
    //    }

    //    return comboString;
    //}
}
