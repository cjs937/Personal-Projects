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
        :base(position, width, height, title, nodeStyle, inPointStyle, outPointStyle, OnClickInPoint, OnClickOutPoint)
    { }

    public override void DrawWindow()
    {
        base.DrawWindow();

        input = (InputType)EditorGUILayout.EnumPopup("Input type : ", input);
    }

    public InputType getInput()
    {
        return input;
    }
}
