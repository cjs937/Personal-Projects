using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class ComboNode : BaseNode
{
    public enum ComboMove
    {
        PUNCH_1,
        PUNCH_2,
        KICK_1,
        KICK_2,
        STAR_PLATINUM_PUNCH
    }

    public ComboMove comboMove;

    public ComboNode()
    {
        windowTitle = "Combo Node";
        prevNode = null;
    }

    public override void DrawWindow()
    {
        base.DrawWindow();

        comboMove = (ComboMove)EditorGUILayout.EnumPopup("Input type : ", comboMove);
    }

    public override void MakeConnection(BaseNode _connector, bool _isInput)
    {
        //prevents combo nodes from connecting to each other
        if(_connector as ComboNode)
        {
            return;
        }
        else
        {
            Debug.Log("fug");
            base.MakeConnection(_connector, _isInput);
        }
    }


    public override void DrawCurves()
    {
        if (nextNode)
        {
            ComboEditor.DrawNodeCurve(windowRect, nextNode.windowRect, Color.red);
        }
    }


    public string getComboString()
    {
        string comboString = "";

        BaseNode lastNode = prevNode;

        while (lastNode)
        {
            InputNode inputNode = (InputNode)lastNode;

            if (inputNode)
            {
                comboString.Insert(0, inputNode.getInputString() + ",");
            }

            lastNode = lastNode.prevNode;
        }

        return comboString;
    }
}
