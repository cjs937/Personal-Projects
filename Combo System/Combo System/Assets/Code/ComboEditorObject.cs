using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ComboEditorObject : MonoBehaviour
{
    public Vector2 offset;
    public List<BaseNode> windows = null;
    public List<Connection> connections = null;

    public void initIfNew()
    {
        if (windows == null)
            windows = new List<BaseNode>();

        if (connections == null)
            connections = new List<Connection>();
    }
}
