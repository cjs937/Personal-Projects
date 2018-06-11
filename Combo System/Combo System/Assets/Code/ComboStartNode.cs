using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class ComboStartNode : BaseNode
{

    public ComboStartNode(Vector2 position, float width, float height, string title, GUIStyle nodeStyle, GUIStyle inPointStyle, GUIStyle outPointStyle, Action<ConnectionPoint> OnClickInPoint, Action<ConnectionPoint> OnClickOutPoint)
    : base(position, width, height, title, nodeStyle, inPointStyle, outPointStyle, OnClickInPoint, OnClickOutPoint, NodeType.INPUT)
    { }
}
