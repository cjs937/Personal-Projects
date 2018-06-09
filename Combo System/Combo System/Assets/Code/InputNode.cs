using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class InputNode : BaseNode
{
    public enum InputType
    {
        X,
        Y,
        B,
        PAUSE_X,
        PAUSE_Y,
        PAUSE_B
    }

    InputType input;

    public InputNode(Vector2 position, float width, float height, string title, GUIStyle nodeStyle, GUIStyle inPointStyle, GUIStyle outPointStyle, Action<ConnectionPoint> OnClickInPoint, Action<ConnectionPoint> OnClickOutPoint)
        :base(position, width, height, title, nodeStyle, inPointStyle, outPointStyle, OnClickInPoint, OnClickOutPoint, NodeType.INPUT)
    {}

    public override void DrawWindow()
    {
        base.DrawWindow();

        input = (InputType)EditorGUILayout.EnumPopup("Input type : ", input);
    }

    protected override void initConnectionPoints(GUIStyle _inPointStype, GUIStyle _outPointStyle, Action<ConnectionPoint> _onClickIn, Action<ConnectionPoint> _onClickOut)
    {
        inPoint = new ConnectionPoint(this, ConnectionPointType.In, _inPointStype, _onClickIn, Color.white);
        outPoint = new ConnectionPoint(this, ConnectionPointType.Out, _outPointStyle, _onClickOut, Color.white);
    }

    public InputType getInput()
    {
        return input;
    }
}
