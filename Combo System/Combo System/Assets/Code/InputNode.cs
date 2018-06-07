using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class InputNode : BaseInputNode
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

    public InputNode()
    {
        windowTitle = "Input Node";
        prevNode = null;
    }

    public override void DrawWindow()
    {
        base.DrawWindow();

        input = (InputType)EditorGUILayout.EnumPopup("Input type : ", input);
    }

    public override string getInputString()
    {
        return input.ToString();
    }

    public InputType getInput()
    {
        return input;
    }
}
